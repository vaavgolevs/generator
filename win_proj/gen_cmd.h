#ifndef __GEN_CMD_H__
#define __GEN_CMD_H__

#include <QObject>
#include <QThread>
#include <QTimer>

#include "pc_udp.h"
///#include <QElapsedTimer>


#define TIMEOUT_RSV		300
#define	DEF_TIMER_DT 15	///10


class c_gen_cmd : public QObject
{
Q_OBJECT
public:
	c_gen_cmd(QObject *parent, quint8* odata);
	virtual ~c_gen_cmd();
	pc_udp *p_pc_udp;
	int tim_dt;	// в мс
	quint32 g_changed_param;
	par_contr_t curr_par_contr;
	QVector<qint16> imp_ampl;

	xil_dat_req_t* p_odata;
	bool is_connected()
	{
		return dev_connected;
	}

protected:
	QThread *p_thr_udp;
	bool attached;
	bool dev_connected;
	bool prev_connected;
	bool new_conn;
	xil_dat_req_t rdy_dat;
	bool data_is_rdy;
public:
	QString ip_addr;		///
	int port;				///
	bool  end_cmd;
	quint8  last_Cmd_good;
	udp_stat_t udp_stat;
	bool dbg_get_xil(xil_dat_req_t&, xil_dat_req_t* odat);
	void dbg_put_xil(xil_dat_req_t& ireq);
	void dbg_put_dac(xil_dat_req_t& ireq);
	bool dbg_get_dac(xil_dat_req_t& req, xil_dat_req_t* o_dat);
	bool dbg_get_stat(quint16* o_dat);

public:
	void StartStop(bool);

///	void AttachToCMD(c_base_cmd *base_cmd);
///	void DetachFromCMD();

	///void ResetConnCount();
	////int GetUnSuccesfullCount() const;

	bool IsAttached() const;

///	int time_count;


///	int GetRequestPeriod() const;
public:

///	bool dev_put_param(quint8 *iparam, quint16 num_bytes);

	///bool dev_put_req_xil(xil_dat_req_t *ireq);
	///bool dev_get_xil(xil_dat_req_t *odat);
	///bool dev_put_xil(xil_dat_req_t *ireq);
///	bool dev_put_req_dac(dac_spi_req_t *ireq);
///	bool dev_get_dac(dac_spi_req_t *odat);
///	bool dev_put_dac(dac_spi_req_t *ireq);
	void ApplyImpAmlToPar();
///	void StopDevice();
	void SetupDevice();


signals:
	void s_put_req_xil(xil_dat_req_t);
	void s_get_xil();
	void s_put_xil(xil_dat_req_t);
	void s_put_req_dac(xil_dat_req_t);
	void s_put_dac(xil_dat_req_t);
	void s_get_dac();
	void s_get_stat();
	void s_start_timer(quint32);
	void s_set_ip(QString, int);
	void s_start(bool);

///	void signal_write();
///	void NoConnection();

///	void signal_start();
///	void signal_stop();
};

#endif // __GEN_CMD_H__
