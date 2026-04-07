#include "generator.h"

generator::generator(QWidget *parent):
    QDialog(parent, Qt::Window)
    ,ui(), _x(DEF_LENGTH), _y(DEF_LENGTH), plot_arr_length(DEF_LENGTH)
	, dev_freq(COEF_DEV_FREQ)		// 1.0
	, gen_changed_param(0)
	
{
	ui.setupUi(this);
	_pCustomPlot = ui.widget_setted;

///	plotter.ConnectToWidget(ui.widget_setted);
///	ui.widget_setted->SetPlotter(&plotter);

///	ui.comboBox_rej->addItem("XIL",XIL_REJ);
///	ui.comboBox_rej->addItem("DAC",DAC_REJ);
	ui.ed_a_imp->set_num_dig(NUM_DIG_AIMP);
	ui.ed_a_imp->set_data(reinterpret_cast<unsigned short*>(&par_gen.Aimp));
	ui.ed_a_imp->set_min_max(MIN_AIMP, MAX_AIMP);
	ui.ed_a_imp->show_par();
	connect(ui.ed_a_imp, SIGNAL(param_changed()), this, SLOT(Aimp_changed()));
	connect(ui.ed_a_imp, SIGNAL(param_changed()), parent, SLOT(change_gen_par()));

	ui.ed_num_periods->set_num_dig(NUM_DIG_QUINT16);
	ui.ed_num_periods->set_data(reinterpret_cast<unsigned short*>(&par_gen.num_periods));
	ui.ed_num_periods->set_min_max(MIN_NUM_PER, MAX_NUM_PER);
	ui.ed_num_periods->show_par();
	connect(ui.ed_num_periods, SIGNAL(param_changed()), this, SLOT(NumPeriods_changed()));
	connect(ui.ed_num_periods, SIGNAL(param_changed()), parent, SLOT(change_gen_par()));

	ui.ed_t_imp_offset->set_num_dig(NUM_DIG_QUINT16);
	ui.ed_t_imp_offset->set_data(reinterpret_cast<unsigned short*>(&par_gen.Timp_offset));
	ui.ed_t_imp_offset->set_min_max(MIN_TDELAY, MAX_TDELAY);
	ui.ed_t_imp_offset->show_par();
	connect(ui.ed_t_imp_offset, SIGNAL(param_changed()), this, SLOT(Timp_offset_changed()));
	connect(ui.ed_t_imp_offset, SIGNAL(param_changed()), parent, SLOT(change_gen_par()));

	dev_freq = COEF_DEV_FREQ * par_gen.frequency;

	ui.ed_freq->set_num_dig(NUM_DIG_DEV_FREQ);
	ui.ed_freq->set_coeff(COEF_DEV_FREQ);
///	ui.ed_freq->set_data(reinterpret_cast<unsigned long*>(&par_gen.frequency));
	ui.ed_freq->set_data(reinterpret_cast<unsigned long*>(&dev_freq));

	ui.ed_freq->set_min_max(MIN_DEV_FREQ, MAX_DEV_FREQ);
	ui.ed_freq->show_par();
	connect(ui.ed_freq, SIGNAL(param_changed()), this, SLOT(Freq_changed()));
	connect(ui.ed_freq, SIGNAL(param_changed()), parent, SLOT(change_gen_par()));

	ui.checkBox_gauss->setChecked(par_gen.gaus_enable);
	connect(ui.checkBox_gauss, SIGNAL(clicked()), this, SLOT(GaussClicked()));
	connect(ui.checkBox_gauss, SIGNAL(clicked()), parent, SLOT(change_gen_par()));

	ui.ed_kus->set_num_dig(NUM_DIG_GAIN);
	ui.ed_kus->set_data(reinterpret_cast<unsigned short*>(&par_gen.gain));
	ui.ed_kus->set_min_max(MIN_GAIN, MAX_GAIN);
	ui.ed_kus->show_par();
	connect(ui.ed_kus, SIGNAL(param_changed()), this, SLOT(gen_gain_changed()));
	connect(ui.ed_kus, SIGNAL(param_changed()), parent, SLOT(change_gen_par()));

	ui.ed_offs->set_num_dig(NUM_DIG_OFFS);
	ui.ed_offs->set_data(reinterpret_cast<unsigned short*>(&par_gen.offs));
	ui.ed_offs->set_min_max(MIN_OFFS, MAX_OFFS);
	ui.ed_offs->show_par();
	connect(ui.ed_offs, SIGNAL(param_changed()), this, SLOT(offs_changed()));
	connect(ui.ed_offs, SIGNAL(param_changed()), parent, SLOT(change_gen_par()));

	ui.checkBox_dds->setChecked(par_gen.dds_enable);
	connect(ui.checkBox_dds, SIGNAL(clicked()), this, SLOT(dds_clicked()));
	connect(ui.checkBox_dds, SIGNAL(clicked()), parent, SLOT(change_gen_par()));

	ui.checkBox_inv->setChecked(par_gen.inv_enable);
	connect(ui.checkBox_inv, SIGNAL(clicked()), this, SLOT(InvClicked()));
	connect(ui.checkBox_inv, SIGNAL(clicked()), parent, SLOT(change_gen_par()));

	connect(ui.pushButt_test, SIGNAL(clicked()), this, SLOT(test_clicked()));

	setupPlotter();
///	fillTestData();
///	UpdatePlotter();
	plot_Array.fill(0);
	plot_Array.resize(plot_arr_length);
///	ImpulseToPlot();
///========================================
	loadSettings();
/*
	DigitCounter* counter = new DigitCounter(4, this); // 4 цифры
	counter->setRange(0, 9999);
	counter->setValue(1234);
*/
#if 0
	connect(counter, &DigitCounter::valueChanged, [&](int val) {
		qDebug() << "Текущее значение:" << val;
		});

#endif
	show_all_par();
	ImpulseToPlot();
	UpdatePlotter();

}

void generator::show_all_par(void) {
///	ui.lineEdit_ip->show_par();
	ui.ed_a_imp->show_par();
	ui.ed_num_periods->show_par();
	ui.ed_t_imp_offset->show_par();

	ui.ed_freq->show_par();
	ui.ed_kus->show_par();
	ui.ed_offs->show_par();
///	par_gen.dds_enable
	///========================================
///	par_gen.dds_enable = 
		ui.checkBox_dds->setChecked(par_gen.dds_enable);
		ui.checkBox_auto->setChecked(par_gen.auto_enable);
		ui.checkBox_gauss->setChecked(par_gen.gaus_enable);
		ui.checkBox_inv->setChecked(par_gen.inv_enable);
		if (par_gen.auto_enable) {
			ui.ed_osc_length->show();
			ui.ed_t_cycle->show();
			ui.ed_t_def->show();
			ui.ed_t_imp_len->show();
			ui.label_len->show();
			ui.label_t_imp->show();
			ui.label_t_cycle->show();
			ui.label_t_def->show();
		}
		else {
			ui.ed_osc_length->hide();
			ui.ed_t_cycle->hide();
			ui.ed_t_def->hide();
			ui.ed_t_imp_len->hide();
			ui.label_len->hide();
			ui.label_t_imp->hide();
			ui.label_t_cycle->hide();
			ui.label_t_def->hide();

		}
///	DigitCounter* counter = new DigitCounter(this,6); // 4 цифры
///	counter->setRange(0, 9999);
///	counter->setValue(1234);

}

void generator::closeEvent( QCloseEvent * event )
{
	QDialog::closeEvent(event);
}


generator::~generator()
{
saveSettings();

}
void generator::RecalculateImpulse()
{
///	return;
	double k = 4.0 * log(2.0 * par_gen.Aimp) / (static_cast<double>(par_gen.Timp_len) * par_gen.Timp_len);

	// y = A * e^(-k*(x - len/2)^2)

	int len_div2 = par_gen.Timp_len / 2;

	for (quint16 i = 0; i < par_gen.Timp_len; i++)
	{
		if (par_gen.gaus_enable)
			imp_ampl[i] = par_gen.Aimp * exp(-k * (i - len_div2) * (i - len_div2)) * sin(2 * M_PI * i * par_gen.frequency / COEF_PERIOD_TRANSF);
		else
			imp_ampl[i] = par_gen.Aimp * sin(2 * M_PI * i * par_gen.frequency / COEF_PERIOD_TRANSF);
	}

}
///#define AMP_OFFS 0x2000

void generator::ImpulseToPlot()
{
///	return;
	qint16* plot_arr = plot_Array.data();

	if (plot_arr_length > (par_gen.Timp_offset + par_gen.Timp_len))
	{
		memset(plot_arr, 0, sizeof(qint16) * par_gen.Timp_offset);
		memcpy(plot_arr + par_gen.Timp_offset, imp_ampl.data(), sizeof(qint16) * par_gen.Timp_len);
		memset(plot_arr + par_gen.Timp_offset + par_gen.Timp_len, 0, sizeof(qint16) * (plot_arr_length - par_gen.Timp_offset - par_gen.Timp_len));
	}
	else if (plot_arr_length > par_gen.Timp_offset)
	{
		memset(plot_arr, 0, sizeof(qint16) * par_gen.Timp_offset);
		memcpy(plot_arr + par_gen.Timp_offset, imp_ampl.data(), sizeof(qint16) * (plot_arr_length - par_gen.Timp_offset));
	}
	else
		memset(plot_arr, 0, sizeof(qint16) * plot_arr_length);
}
void generator::ApplyImpAmlToPar()
{
///	return;
	imp_ampl.resize(par_gen.Timp_len);
}

void generator::Timp_len_changed()
{
///	return;

ApplyImpAmlToPar();
		RecalculateImpulse();
		ImpulseToPlot();

		plotRespond(plot_Array.data(), plot_arr_length);

gen_changed_param |= CHNG_IMP_POINTS;
	
}

void generator::Timp_offset_changed()
{
///	return;

ImpulseToPlot();
plotRespond(plot_Array.data(), plot_arr_length);

gen_changed_param |= CHNG_TIMP_OFFSET;
}

void generator::NumPeriods_changed()
{
///return;

float curr_period_len = COEF_PERIOD_TRANSF / par_gen.frequency;
		curr_period_len *= par_gen.num_periods;
		par_gen.Timp_len = curr_period_len;		// число тиков после изменения
		ApplyImpAmlToPar();
		ui.ed_t_imp_len->show_par();
		RecalculateImpulse();
		ImpulseToPlot();
		plotRespond(plot_Array.data(), plot_arr_length);
gen_changed_param |= CHNG_IMP_POINTS;

}


void generator::Aimp_changed()
{
///	return;

RecalculateImpulse();
ImpulseToPlot();
plotRespond(plot_Array.data(), plot_arr_length);
gen_changed_param |= CHNG_IMP_POINTS;

}
void generator::gen_gain_changed()
{
gen_changed_param |= CHNG_GAIN;
}
void generator::offs_changed()
{
gen_changed_param |= CHNG_OFFS;
}

void generator::dds_clicked()
{
///	return;

	par_gen.dds_enable = ui.checkBox_dds->isChecked();

RecalculateImpulse();
ImpulseToPlot();
plotRespond(plot_Array.data(), plot_arr_length);

gen_changed_param |= CHNG_ON_DDS;
}
void generator::InvClicked() {
	par_gen.dds_enable = ui.checkBox_dds->isChecked();

	RecalculateImpulse();
	ImpulseToPlot();
	plotRespond(plot_Array.data(), plot_arr_length);

	gen_changed_param |= CHNG_ON_DDS;

}

void generator::Freq_changed()
{
///	return;
	par_gen.frequency = static_cast<float>(dev_freq) / COEF_DEV_FREQ;
		float curr_period_len = COEF_PERIOD_TRANSF / par_gen.frequency ;
		curr_period_len *= par_gen.num_periods;
		par_gen.Timp_len = curr_period_len;		// число тиков после изменения
		ApplyImpAmlToPar();

		ui.ed_t_imp_len->show_par();
		RecalculateImpulse();
		ImpulseToPlot();
		plotRespond(plot_Array.data(), plot_arr_length);
gen_changed_param |= CHNG_IMP_POINTS;

}


void generator::GaussClicked()
{
///	return;

par_gen.gaus_enable = ui.checkBox_gauss->isChecked();

RecalculateImpulse();
ImpulseToPlot();
plotRespond(plot_Array.data(), plot_arr_length);

gen_changed_param |= CHNG_IMP_POINTS;
}
void generator::UpdatePlotter()
{
	_pCustomPlot->graph(0)->setData(_x, _y);
	_pCustomPlot->graph(0)->rescaleAxes();
	_pCustomPlot->replot();
}
void generator::setupPlotter()
{
	_pCustomPlot->addGraph();
	_pCustomPlot->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
///	_pCustomPlot->graph(0)->setName("Air");
	_pCustomPlot->xAxis->setVisible(true);
	_pCustomPlot->yAxis->setRange(-MAX_GEN_AMPL, MAX_GEN_AMPL);

	_pCustomPlot->xAxis->setTickLabels(true);
	_pCustomPlot->xAxis->setTickLabelPadding(false);
///	_pCustomPlot->xAxis2->setVisible(false);
///	_pCustomPlot->xAxis2->setTickLabels(false);
///	_pCustomPlot->yAxis2->setVisible(true);
///	_pCustomPlot->yAxis2->setTickLabels(false);
	// make left and bottom axes always transfer their ranges to right and top axes:
////	connect(_pCustomPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), _pCustomPlot->xAxis2, SLOT(setRange(QCPRange)));
////	connect(_pCustomPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), _pCustomPlot->yAxis2, SLOT(setRange(QCPRange)));
	// pass data points to graphs:

	// let the ranges scale themselves so graph 0 fits perfectly in the visible area:

	// Note: we could have also just called _pCustomPlot->rescaleAxes(); instead
	// Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
/////	_pCustomPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
///	_pCustomPlot->legend->setVisible(true);
}

void generator::fillTestData()
{
	static quint16 ttt = 1;

/*
	static quint16 ttt = 0;
	quint16 yyy = ttt & 0x3ff;
	_x.push_back(ttt);
	_y.push_back(yyy * 2);
	ttt++;
	while (_x.size() > MAX_X)
	{
		_x.pop_front();
		_y.pop_front();
	}
*/
	for (int i = 0; i < 501; ++i)
	{
		_x[i] = i;/// (ttt * i) / 50.0 - 1.0; // [-1, 1]
		_y[i] = _x[i] * _x[i]+ttt*100;    // y = x²
	}
	ttt += 3;
}
void generator::plotRespond(qint16* point_arr, quint16 num_points)
{
	qint16 tmp;
	for (int i = 0; i < num_points; ++i)
	{
		_x[i] = i;
///		_y[i] = point_arr[i] ;    //
		tmp = point_arr[i];    //
		_y[i] = tmp;    //
	}
///	_pCustomPlot->graph(0)->setData(_x, _y);
///	_pCustomPlot->graph(0)->rescaleAxes();
////_pCustomPlot->replot();
UpdatePlotter();

}


void generator::test_clicked()
{
	fillTestData();
	UpdatePlotter();
///	return;
}
void generator::saveSettings()
{
QSettings settings(QCoreApplication::applicationDirPath() + "//generator.ini",
	QSettings::IniFormat);
///=========================================================
settings.setValue("gen_ip_addr", par_gen.ip_addr);

settings.setValue("gen_Timp_len", par_gen.Timp_len);
settings.setValue("gen_Timp_offset", par_gen.Timp_offset);
settings.setValue("gen_gain", par_gen.gain);
settings.setValue("gen_offs", par_gen.offs);
///	settings.setValue("attenuator", sent_par.attenuator);

settings.setValue("gen_num_periods", par_gen.num_periods);

settings.setValue("gen_Aimp", par_gen.Aimp);
settings.setValue("gen_dev_frequency", par_gen.frequency);
settings.setValue("gen_gaus_enable", par_gen.gaus_enable);
settings.setValue("gen_dds_enable", par_gen.dds_enable);
settings.setValue("gen_auto_enable", par_gen.auto_enable);

}
void generator::loadSettings()
{
QSettings settings(QCoreApplication::applicationDirPath() + "//generator.ini",
	QSettings::IniFormat);
///===============================================================================
par_gen.ip_addr = settings.value("gen_ip_addr", "192.168.1.99").toString();

par_gen.Timp_len = settings.value("gen_Timp_len", 100).toInt();
par_gen.Timp_offset = settings.value("gen_Timp_offset", 0).toInt();
par_gen.gain = settings.value("gen_gain", 10).toInt();
par_gen.offs = settings.value("gen_offs", 0).toInt();
///	sent_par.step_osc = settings.value("step_osc", sent_par.step_osc).toInt();
///	sent_par.attenuator = settings.value("attenuator", sent_par.attenuator).toInt();

par_gen.num_periods = settings.value("gen_num_periods", 4).toInt();

par_gen.Aimp = settings.value("gen_Aimp", 1000).toInt();
par_gen.frequency = settings.value("gen_frequency", 2.000).toFloat();
par_gen.gaus_enable = settings.value("gen_gaus_enable", false).toBool();
par_gen.dds_enable = settings.value("gen_dds_enable", false).toBool();
par_gen.auto_enable = settings.value("gen_auto_enable", false).toBool();

}