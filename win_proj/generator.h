#ifndef _GENERATOR_H_
#define _GENERATOR_H_

#include <QCloseEvent>
#include <QtWidgets/QtWidgets>
///#include "OscillogramPlotterSetter.h"

#include "ui_generator.h"
#include "dev_interf.h"
#include "qcustomplot.h"
#include "DigitCounter.h"

namespace Ui {
    class generator;
}
#define	DEF_LENGTH			1000

#define MAX_X            8000

#define MAX_GEN_AMPL         8192

#define XIL_REJ		0
#define DAC_REJ		1
///#define ADC_REJ		2
///#define CONTR_REJ	3

//#define TEST_REJ	4

#define NUM_DIG_AIMP		4
#define NUM_DIG_DEV_FREQ	10

#define NUM_DIG_GAIN		5
#define NUM_DIG_OFFS	    5
#define MIN_NUM_PER			1
#define MAX_NUM_PER			MAX_QUINT16

#define MIN_AIMP			0
#define MAX_AIMP			8000

#define MIN_GAIN			0
#define MAX_GAIN			4096

#define MIN_TDELAY			0
#define MAX_TDELAY			4096

#define MIN_OFFS		0
#define MAX_OFFS		4096

#define MIN_ATT		    0
#define MAX_ATT		    91

#define MIN_DEV_FREQ		1
#define MAX_DEV_FREQ		10000000

#define COEF_DEV_FREQ		10000

#define COEF_PERIOD_TRANSF	125

#define CHNG_TIMP_LEN			(0x1<<1)
#define CHNG_TIMP_OFFSET		(0x1<<2)

#define CHNG_TCYCLE				(0x1<<3)
#define CHNG_TDEF				(0x1<<4)

#define CHNG_IMP_POINTS			(0x1<<5)

#define CHNG_GAIN				(0x1<<6)
#define CHNG_OFFS			    (0x1<<7)
#define CHNG_ON_DDS             (0x1<<8)
#define CHNG_ON_INV             (0x1<<9)

///#define CHNG_BEG_OSC			(0x1<<7)
///#define CHNG_STEP_OSC			(0x1<<8)

#define CHNG_ATTENUATOR			(0x1<<10)


struct get_type_t
{
	QString ip_addr;		///
	int port;				///

	quint16 Timp_len;		// полная длина импульа
	quint16 Timp_offset;	// сдвиг импульса отностиельно сигнала синхронизации

	quint16 gain;
	quint16 offs;

	quint16 num_periods;
	quint16 Aimp;			// амплитуда импульса
	float frequency;	// частота колебаний в импульсе
	bool gaus_enable;
	bool dds_enable;
	bool inv_enable;

///	quint8 attenuator;
};



class generator : public QDialog
{
Q_OBJECT

protected:
	Ui::generator ui;
	virtual void closeEvent( QCloseEvent * event );
	quint16 plot_arr_length;
	QVector<qint16> plot_Array;
///	OscillogramProperties *p_osc_prop;

	///OscillogramPlotterSetter plotter;

	void RecalculateImpulse();
	void ImpulseToPlot();
	void ApplyImpAmlToPar();
	quint32 dev_freq;

public:
	generator(QWidget *parent= nullptr);
	virtual ~generator();
	quint32 gen_changed_param;

	get_type_t par_gen;
	QVector<qint16> imp_ampl;
	void show_all_par(void);
public slots:
	void Timp_len_changed();
	void Timp_offset_changed();
	void NumPeriods_changed();
	void Aimp_changed();
	void Freq_changed();
	void GaussClicked();
	void gen_gain_changed();
	void offs_changed();
	void dds_clicked();
	void InvClicked();
	void test_clicked();

protected:
	QCustomPlot* _pCustomPlot;
	QVector<double> _x;
	QVector<double> _y;
	void setupPlotter();
	void UpdatePlotter();
	void fillTestData();
	void plotRespond(qint16* point_arr, quint16 num_points);

};

#endif // _GENERATOR_H_
