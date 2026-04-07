
#include "TwoPositionSwitch.h"
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPaintEvent>

TwoPositionSwitch::TwoPositionSwitch(QWidget *parent)
    : QWidget(parent), m_checked(false)
{
    setFocusPolicy(Qt::StrongFocus);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setCursor(Qt::PointingHandCursor);
}

QSize TwoPositionSwitch::sizeHint() const
{
    return QSize(52, 26);
}

QSize TwoPositionSwitch::minimumSizeHint() const
{
    return QSize(44, 22);
}

void TwoPositionSwitch::setChecked(bool on)
{
    if (m_checked == on)
        return;
    m_checked = on;
    update();
    emit toggled(m_checked);
}

void TwoPositionSwitch::flip()
{
    setChecked(!m_checked);
}

void TwoPositionSwitch::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    const QRect r = rect().adjusted(1, 1, -1, -1);
    const QColor trackOff = palette().color(QPalette::Mid);
    const QColor trackOn = palette().color(QPalette::Highlight);
    const QColor thumb = palette().color(QPalette::Button);

    const int radius = qMax(4, r.height() / 2);
    p.setPen(Qt::NoPen);
    p.setBrush(m_checked ? trackOn : trackOff);
    p.drawRoundedRect(r, radius, radius);

    const int margin = 3;
    const int d = qMax(4, r.height() - 2 * margin);
    const int x0 = r.left() + margin;
    const int x1 = r.right() - margin - d;
    const int x = m_checked ? x1 : x0;
    const int y = r.center().y() - d / 2;

    p.setBrush(thumb.lighter(m_checked ? 102 : 108));
    p.drawEllipse(QRect(x, y, d, d));
}

void TwoPositionSwitch::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && rect().contains(event->pos()))
        flip();
    QWidget::mouseReleaseEvent(event);
}

void TwoPositionSwitch::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space || event->key() == Qt::Key_Return) {
        flip();
        return;
    }
    QWidget::keyPressEvent(event);
}
