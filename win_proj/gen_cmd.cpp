#include "gen_cmd.h"

extern void addfcs16( char *cp, int len );


c_gen_cmd::c_gen_cmd(QObject *parent, quint8* odata):
		QObject(parent),
	    tim_dt(DEF_TIMER_DT),
	    attached(false),
	    dev_connected(false),
	    prev_connected(false),
	    end_cmd(false),
	    last_Cmd_good(0),
	    p_thr_udp(nullptr),
	    p_pc_udp(nullptr),
	    g_changed_param(0),
	    p_odata((xil_dat_req_t* )odata),
	    ip_addr(),
	    port(2000)
{
	p_thr_udp = new QThread();
	p_pc_udp = new pc_udp(&port, ip_addr,p_odata, &curr_par_contr, &end_cmd, &g_changed_param
		, &imp_ampl, &last_Cmd_good, &udp_stat);
	p_pc_udp->udp_pc_init();
	p_pc_udp->moveToThread(p_thr_udp);
	p_thr_udp->start();
	connect(this, SIGNAL(s_put_req_xil(xil_dat_req_t)), p_pc_udp, SLOT(sl_put_req_xil(xil_dat_req_t)));
	connect(this, SIGNAL(s_get_xil()), p_pc_udp, SLOT(sl_get_xil()));
	connect(this, SIGNAL(s_put_xil(xil_dat_req_t)), p_pc_udp, SLOT(sl_put_xil(xil_dat_req_t)));
	connect(this, SIGNAL(s_put_req_dac(xil_dat_req_t)), p_pc_udp, SLOT(sl_put_req_dac(xil_dat_req_t)));
	connect(this, SIGNAL(s_put_dac(xil_dat_req_t)), p_pc_udp, SLOT(sl_put_dac(xil_dat_req_t)));
	connect(this, SIGNAL(s_get_dac()), p_pc_udp, SLOT(sl_get_dac()));
	connect(this, SIGNAL(s_get_stat()), p_pc_udp, SLOT(sl_get_stat()));
	connect(this, SIGNAL(s_start_timer(quint32)), p_pc_udp, SLOT(sl_start_timer(quint32)));
	connect(this, SIGNAL(s_start(bool)), p_pc_udp, SLOT(sl_start(bool)));
	connect(this, SIGNAL(s_set_ip(QString, int)), p_pc_udp, SLOT(SlotSetIp(QString, int)));

}

c_gen_cmd::~c_gen_cmd()
{
	p_thr_udp->quit();
	p_thr_udp->wait();

	delete p_thr_udp;

}

bool c_gen_cmd::IsAttached() const
{
	return attached;
}
void c_gen_cmd::StartStop(bool start)
{
	if (start)
	{
		emit s_set_ip(ip_addr, port);
		emit s_start(true);
		emit s_start_timer(tim_dt);
		new_conn = true;
		////	req_timer.start(tim_dt);
		///	put_dbg_str("Start");
		qDebug() << "Start ";
		g_changed_param = 0;
	///	emit s_SetIniting(true);

	}
	else
	{
		emit s_start_timer(0);////	req_timer.stop();
	///	put_dbg_str("Stop");
		emit s_start(false);
		dev_connected = false;
		qDebug() << "Stop timer";
	}
	attached = start;
}
void c_gen_cmd::SetupDevice()
{
	g_changed_param = 0;

	quint16 tmp;
	if (dbg_get_stat(&tmp)) {
		qDebug() << "SetupDevice OK "<<tmp;
		attached = true;
	}
	else {
		qDebug() << "SetupDevice Bad " ;
		attached = false;

	}

///	p_tune_thr->is_initing = true;

///	p_pc_udp->set_host(ip_addr, port);
///	p_tune_thr->dev_cmd.AttachToCMD(p_udp_cmd);
///	UpdateDevice(true);

}
#if 0
bool c_device_cmd::dev_put_req_dac(dac_spi_req_t* ireq)
{
///	return send_and_wait(CMD_PUT_DAT, NUM_SEND_SPI_DAC_REQ, reinterpret_cast<quint8*>(ireq), sizeof(dac_spi_req_t));
	return false;
}

bool c_device_cmd::dev_get_dac(dac_spi_req_t* odat)
{
///	return send_and_wait(CMD_REQ_DAT, NUM_REQ_SPI_DAC_DAT, reinterpret_cast<quint8*>(odat));
	return false;

}

bool c_device_cmd::dev_put_dac(dac_spi_req_t* ireq)
{
///	return send_and_wait(CMD_PUT_DAT, NUM_SEND_SPI_DAC_DAT, reinterpret_cast<quint8*>(ireq), sizeof(dac_spi_req_t));
	return false;

}
#endif
void c_gen_cmd::ApplyImpAmlToPar()
{
////	imp_ampl.resize(curr_par_contr.sent_par.Timp_len);
}
#define MAX_WAIT_RDY 1000

void c_gen_cmd::dbg_put_xil(xil_dat_req_t& req) {
	emit s_put_xil(req);
}

bool c_gen_cmd::dbg_get_xil(xil_dat_req_t& req, xil_dat_req_t* o_dat) {
#if 0
	////	return false;
	end_cmd = false;
	QElapsedTimer timer_wait_rdy;

	emit s_put_req_xil(req);
	end_cmd = false;
	last_Cmd_good = 0;

	emit s_get_xil();
	timer_wait_rdy.start();
	////	qDebug() << "timer_wait_rdy.start ";

	while ((last_Cmd_good != NUM_REQ_XIL_DAT))
	{
		int	time_count = timer_wait_rdy.elapsed();
		if (time_count >= MAX_WAIT_RDY)
		{
			qDebug() << "dbg_get_xil error wait ";
			return false;
		}
	};
	memcpy(o_dat, p_odata, sizeof(xil_dat_req_t));
	qDebug() << "dbg_get_xil= " << QString::number(o_dat->addr<< o_dat->data[0], 16) <<"cmd =" << QString::number(last_Cmd_good, 16);
#endif
	return true;
}
void c_gen_cmd::dbg_put_dac(xil_dat_req_t& ireq) {
	emit s_put_dac(ireq);

}
bool c_gen_cmd::dbg_get_dac(xil_dat_req_t& req, xil_dat_req_t* o_dat) {
	////	return false;
	end_cmd = false;
#if 0
	last_Cmd_good = 0;
	////	xil_dat_req_t t_dat;
		////p_dev_data->odata = (void*)&t_dat;
	QElapsedTimer timer_wait_rdy;

	emit s_put_req_dac(req);
	end_cmd = false;

	emit s_get_dac();
	timer_wait_rdy.start();
	////	qDebug() << "timer_wait_rdy.start ";

	while ((last_Cmd_good != NUM_REQ_SPI_DAC_DAT))
	{
		int	time_count = timer_wait_rdy.elapsed();
		if (time_count >= MAX_WAIT_RDY)
		{
			qDebug() << "dbg_get_xil error wait ";
			return false;
		}
	};

	memcpy(o_dat, p_odata, sizeof(xil_dat_req_t));

	////qDebug() << "dbg_get_xil( "<< o_dat->addr<< o_dat->data[0];
#endif
	return true;
}
bool c_gen_cmd::dbg_get_stat(quint16* o_dat) {
	////	return false;
#if 0
	end_cmd = false;
	last_Cmd_good = 0;
	////	xil_dat_req_t t_dat;
		////p_dev_data->odata = (void*)&t_dat;
	QElapsedTimer timer_wait_rdy;

	emit s_get_stat();
	end_cmd = false;

///	emit s_get_dac();
	timer_wait_rdy.start();
	////	qDebug() << "timer_wait_rdy.start ";

	while ((last_Cmd_good != NUM_REQ_STAT))
	{
		int	time_count = timer_wait_rdy.elapsed();
		if (time_count >= MAX_WAIT_RDY)
		{
			qDebug() << "dbg_get_stat error wait ";
			return false;
		}
	};
	memcpy(o_dat, p_odata, sizeof(quint16));

	////qDebug() << "dbg_get_xil( "<< o_dat->addr<< o_dat->data[0];
#endif
	return true;
}



