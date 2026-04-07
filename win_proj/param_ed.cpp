#include "param_ed.h"

#include <QVector>
#include <algorithm>

namespace {

static double digitPlaceDeltaFloat(int digitIdx, const QString &s, const QVector<int> &digitPos)
{
	const int dot = s.indexOf(QLatin1Char('.'));
	const int ch = digitPos[digitIdx];

	if (dot < 0) {
		const int n = digitPos.size();
		const int p = n - 1 - digitIdx;
		double d = 1.0;
		for (int i = 0; i < p; ++i)
			d *= 10.0;
		return d;
	}

	if (ch < dot) {
		int nLeft = 0;
		for (int pos : digitPos) {
			if (pos < dot)
				nLeft++;
		}
		int rank = 0;
		for (int pos : digitPos) {
			if (pos >= dot)
				break;
			if (pos == ch) {
				const int exp = nLeft - 1 - rank;
				double d = 1.0;
				for (int i = 0; i < exp; ++i)
					d *= 10.0;
				return d;
			}
			rank++;
		}
	} else {
		int fracRank = 0;
		for (int pos : digitPos) {
			if (pos <= dot)
				continue;
			if (pos == ch) {
				double d = 1.0;
				for (int i = 0; i <= fracRank; ++i)
					d *= 0.1;
				return d;
			}
			fracRank++;
		}
	}
	return 1.0;
}

} // namespace

static bool paramEdLessByScreenPos(param_ed *a, param_ed *b)
{
	const QPoint ca = a->mapToGlobal(a->rect().center());
	const QPoint cb = b->mapToGlobal(b->rect().center());
	if (ca.y() != cb.y())
		return ca.y() < cb.y();
	return ca.x() < cb.x();
}

void param_ed::navigateToAdjacentField(bool goUp)
{
	QWidget *container = parentWidget();
	if (!container)
		return;

	QList<param_ed *> all = container->findChildren<param_ed *>(QString(), Qt::FindDirectChildrenOnly);
	if (all.size() < 2)
		all = container->findChildren<param_ed *>();
	if (all.size() < 2)
		return;

	std::sort(all.begin(), all.end(), paramEdLessByScreenPos);

	const int idx = all.indexOf(this);
	if (idx < 0)
		return;

	if (goUp) {
		if (idx > 0)
			all[idx - 1]->setFocus(Qt::TabFocusReason);
	} else {
		if (idx < all.size() - 1)
			all[idx + 1]->setFocus(Qt::TabFocusReason);
	}
}

void param_ed::restoreCursorAfterDigitAdjust(int savedCursorPos)
{
	const int len = text().length();
	setCursorPosition(qBound(0, savedCursorPos, len));
}

bool param_ed::adjustDigitAtCursor(int dir)
{
	if (!any_elem || dir == 0)
		return false;

	const QString s = text();
	const int cp = cursorPosition();
	QVector<int> digitPos;
	for (int i = 0; i < s.size(); ++i) {
		if (s[i].isDigit())
			digitPos.append(i);
	}
	if (digitPos.isEmpty())
		return false;

	int digitIdx = digitPos.size() - 1;
	for (int k = 0; k < digitPos.size(); ++k) {
		if (cp <= digitPos[k]) {
			digitIdx = k;
			break;
		}
	}

	const int savedCursorPos = cp;

	auto writeStorage = [this](int itmp) {
		itmp = qBound(i_min, itmp, i_max);
		if (p_sdata)
			*p_sdata = static_cast<unsigned short>(itmp);
		else if (p_ldata)
			*p_ldata = static_cast<unsigned long>(itmp);
		else if (p_cdata)
			*p_cdata = static_cast<unsigned char>(qBound(0, itmp, 255));
	};

	if (num_dights > 0) {
		bool ok = false;
		const double f = s.toFloat(&ok);
		if (!ok)
			return false;
		const double delta = digitPlaceDeltaFloat(digitIdx, s, digitPos);
		double newF = f + static_cast<double>(dir) * delta;
		const double minF = (static_cast<double>(i_min) + shift) / static_cast<double>(coeff);
		const double maxF = (static_cast<double>(i_max) + shift) / static_cast<double>(coeff);
		newF = qBound(minF, newF, maxF);
		int newRaw = qRound(newF * static_cast<double>(coeff)) - shift;
		newRaw = qBound(i_min, newRaw, i_max);
		writeStorage(newRaw);
		emit param_changed();
		show_par();
		restoreCursorAfterDigitAdjust(savedCursorPos);
		return true;
	}

	bool ok = false;
	const int disp = s.toInt(&ok);
	if (!ok)
		return false;

	const int n = digitPos.size();
	int pow10 = 1;
	for (int i = 0; i < n - 1 - digitIdx; ++i)
		pow10 *= 10;

	const int minShow = i_min + shift;
	const int maxShow = i_max + shift;
	int newDisp = disp + dir * pow10;
	newDisp = qBound(minShow, newDisp, maxShow);

	int newStored = newDisp - shift;
	newStored = qBound(i_min, newStored, i_max);
	writeStorage(newStored);
	emit param_changed();
	show_par();
	restoreCursorAfterDigitAdjust(savedCursorPos);
	return true;
}

 
param_ed::param_ed(QWidget *parent) :
					QLineEdit(parent),

					i_max(0),
					i_min(0),
					i_inc_step(1),
					p_cdata(nullptr),
					p_sdata(nullptr),
					p_ldata(nullptr),

					any_elem(false),

					num_dig(2),
					coeff(0),
					num_dights(0),
					shift(0),
					ena_enter_on_off(false),
					ena_edit(true)
{
}

int param_ed::GetMinimalWidth() const
{
	QString test_str("_");
	for(int i = 0; i < num_dig; i++)
		test_str += "_";

	return fontMetrics().boundingRect(test_str).width();
}


QSize param_ed::minimumSizeHint() const
{
	if(num_dig == 0)
		return QLineEdit::minimumSizeHint();

	QSize curr_size = QLineEdit::minimumSizeHint();
	curr_size.setWidth( GetMinimalWidth() );
	return curr_size;
}


QSize param_ed::sizeHint() const
{
	if(num_dig == 0)
		return QLineEdit::sizeHint();

	QSize curr_size = QLineEdit::sizeHint();
	curr_size.setWidth( GetMinimalWidth() );
	return curr_size;
}



void param_ed::mouseDoubleClickEvent(QMouseEvent* pe)
{
	if(ena_enter_on_off)
		emit on_off();
}

void param_ed::set_min_max(int imin,int imax)
{
	i_max=imax;
	i_min=imin;
}

void param_ed::set_ena_edit(bool iena_edit)
{
	ena_edit=iena_edit;
}

void param_ed::set_inc_step(int istep)
{
	i_inc_step=istep;
}

void param_ed::set_num_dig(int inum_dig)
{
	num_dig=inum_dig;
	if(num_dights == 0)
		setMaxLength(inum_dig);
	else
		setMaxLength(inum_dig+1);
}

void param_ed::set_coeff(int icoeff)
{
	coeff=icoeff;
	if(coeff >= 10)
	{
		num_dights = log10(static_cast<float>(coeff));
		setMaxLength(num_dig+1);
	}
	else
	{
		num_dights = 0;
		setMaxLength(num_dig);
	}
}


void param_ed::set_shift(int ishift)
{
	shift = ishift;
}


void param_ed::set_data(unsigned char *p_data)
{
	p_cdata=p_data;
	any_elem = p_cdata ? true : false;
	p_sdata=nullptr;
	p_ldata=nullptr;
}

void param_ed::set_data(unsigned short *p_data)
{
	p_sdata=p_data;
	any_elem = p_sdata ? true : false;
	p_cdata=nullptr;
	p_ldata=nullptr;
}

void param_ed::set_data(unsigned long *p_data)
{
	p_ldata=p_data;
	any_elem = p_ldata ? true : false;
	p_cdata=nullptr;
	p_sdata=nullptr;
}

void param_ed::set_colors(const char *col)
{
	if(strcmp("rgb(0, 0, 0)", col) == 0)
		setStyleSheet("");
	else
	{
		QString tstr;
		tstr.asprintf("color:%s",col);
		setStyleSheet(tstr);
	}
}

void param_ed::set_ena_enter_on_off(bool ena)
{
	ena_enter_on_off=ena;
}

void param_ed::inc_par(const int count)
{
	if(!any_elem)
		return;

	if(p_sdata)
		inc_par_val(p_sdata, count);
	else if(p_ldata)
		inc_par_val(p_ldata, count);
	else if(p_cdata)
		inc_par_val(p_cdata, count);

	emit param_changed();
}

void param_ed::dec_par(const int count)
{
	if(!any_elem)
		return;

	if(p_sdata)
		dec_par_val(p_sdata, count);
	else if(p_ldata)
		dec_par_val(p_ldata, count);
	else if(p_cdata)
		dec_par_val(p_cdata, count);

	emit param_changed();
}

void param_ed::show_par()
{
	if(!any_elem)
		return;

	QString tstr;
	
	if(p_sdata)
	{
		if(num_dights > 0)
			tstr = QString::number((static_cast<float>(*p_sdata + shift) / coeff),'f', num_dights);
		else
			tstr = QString::number(*p_sdata + shift, 10);
	}
	else if(p_ldata)
	{
		if(num_dights > 0)
			tstr = QString::number((static_cast<float>(*p_ldata + shift) / coeff),'f', num_dights);
		else
			tstr = QString::number(*p_ldata + shift, 10);
	}
	else if(p_cdata)
	{
		if(num_dights > 0)
			tstr = QString::number((static_cast<float>(*p_cdata + shift) / coeff),'f', num_dights);
		else
			tstr = QString::number(*p_cdata + shift, 10);
	}

	setText(tstr);
}

void param_ed::apply_current()
{
	ApplyNewText();
	show_par();
}

void param_ed::ApplyNewText()
{
	QString tstr=text();

	if(num_dights > 0)
	{
		int itmp = tstr.toFloat() * coeff + 0.5f - shift;

		if(itmp > i_max)
			itmp = i_max;
		else if(itmp < i_min)
			itmp = i_min;

		if(any_elem)
		{
			if(p_sdata)
				*p_sdata=itmp;
			else if(p_ldata)
				*p_ldata=itmp;
			else if(p_cdata)
				*p_cdata=itmp;
		}
	}
	else
	{
		int itmp = tstr.toInt() - shift;

		if(itmp > i_max)
			itmp = i_max;
		else if(itmp < i_min)
			itmp = i_min;

		if(any_elem)
		{
			if(p_sdata)
				*p_sdata=itmp;
			else if(p_ldata)
				*p_ldata=itmp;
			else if(p_cdata)
				*p_cdata=itmp;
		}
	}
}

void param_ed::keyPressEvent(QKeyEvent * pe)
{
	switch (pe->key()) 
	{
	case Qt::Key_Up:
	case 0x42e:
		navigateToAdjacentField(true);
		break;
	case Qt::Key_Down:
	case 0x411:
		navigateToAdjacentField(false);
		break;
	case Qt::Key_Plus:
	case Qt::Key_Equal:
	case Qt::Key_Greater:
		if (ena_edit) {
			if (!any_elem || !adjustDigitAtCursor(1)) {
				if (any_elem) {
					inc_par();
					show_par();
				}
			}
		}
		break;
	case Qt::Key_Minus:
	case Qt::Key_Underscore:
	case Qt::Key_Less:
		if (ena_edit) {
			if (!any_elem || !adjustDigitAtCursor(-1)) {
				if (any_elem) {
					dec_par();
					show_par();
				}
			}
		}
		break;
	case Qt::Key_Enter:
	case Qt::Key_Return:

		show_par();

		if(ena_edit)
			emit param_changed();

		if(ena_enter_on_off)
			emit on_off();
		break;

	case Qt::Key_Comma:
		if(ena_edit)
		{
			QString tstr=text();
			if(num_dights != 0 && !tstr.contains('.'))
			{
				tstr+=".";
				setText(tstr);
			}
			else
			{
				if (!any_elem || !adjustDigitAtCursor(-1)) {
					if (any_elem) {
						dec_par();
						show_par();
					}
				}
			}
		}
		break;

	case Qt::Key_Period:
		if(ena_edit)
		{
			QString tstr=text();
			if(num_dights == 0 || tstr.contains('.'))
			{
				if (!any_elem || !adjustDigitAtCursor(1)) {
					if (any_elem) {
						inc_par();
						show_par();
					}
				}
			}
			else
				QLineEdit::keyPressEvent(pe);
		}
		break;

	case Qt::Key_0:
	case Qt::Key_1:
	case Qt::Key_2:
	case Qt::Key_3:
	case Qt::Key_4:
	case Qt::Key_5:
	case Qt::Key_6:
	case Qt::Key_7:
	case Qt::Key_8:
	case Qt::Key_9:
	case Qt::Key_Backspace:
	case Qt::Key_Delete:

		if(ena_edit)
		{
			QLineEdit::keyPressEvent(pe);
			ApplyNewText();
		}
		break;

	case Qt::Key_Left:
	case Qt::Key_Right:

		QLineEdit::keyPressEvent(pe);
		break;

	}
}
