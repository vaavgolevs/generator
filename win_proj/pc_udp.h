#ifndef _PC_UDP_H_
#define _PC_UDP_H_

#include <QObject>
#include <QThread>
#include <QQueue>
#include <QTimerEvent>

#include "udp_send_res.h"
#include "dev_interf.h"
///#include "params.h"
////#include "cmdreq.h"

////const char* VER_TXT="baloo v1.0(G.V.A.)";

#define HDLC1_MAX_FRAME_DATA		8192
#define HDLC1_MAX_FRAME_LEN	(HDLC1_MAX_FRAME_DATA+2)

///#define FLG_CMD_CLR_COORD   (0x1<<0)
////#define FLG_CMD_VDP_SET		(0x1<<1)
/*
struct req_cmd_t
{
quint32 cmd;
xil_dat_req_t req;
};

*/
extern void addfcs16( char *cp, const int len );

class pc_udp : public QObject ///public c_base_dev_cmd
{
 Q_OBJECT
public:
	pc_udp(int *pport,QString &_ip_addr, xil_dat_req_t* o_data = nullptr, par_contr_t* par_contr=nullptr
		, bool* end_cmd=nullptr	, quint32* g_changed_param=nullptr, QVector<qint16>* imp_ampl=nullptr
		,quint8* last_cmd_good = nullptr, udp_stat_t *udp_stat =nullptr);
	virtual ~pc_udp();
public:
	xil_dat_req_t *p_odata;
	quint32* pg_changed_param;
	void udp_pc_init(void);
	QString ip_addr;
	bool* p_end_cmd;
	quint8* p_last_Cmd_good;
///	void* p_odat;
	QQueue<req_cmd_t> req_que;
private:
	void dev_put_req_xil(xil_dat_req_t ireq);
	void dev_get_xil(void);
	void dev_put_xil(xil_dat_req_t ireq);
	void dev_put_req_dac(xil_dat_req_t ireq);
	void dev_get_dac(void);
	void dev_put_dac(xil_dat_req_t ireq);

	void dev_get_stat(void);

protected:
	void timerEvent(QTimerEvent* event) override;
	int timer_id;
	///QTimer *p_req_timer;
	bool attached;
	udp_send_res *pUdpSendRes;
	QThread *p_udp_send_res_thr;

	const int timeout_resive;
	QElapsedTimer el_timer;
///	QElapsedTimer _el_timer_2;
	udp_stat_t *p_udp_stat;
	quint16 udp_set_frame(quint8 *data, int len);
	quint16	udp_send_ack_to(quint8 ipar);
	quint16	udp_send_req_to(quint8 ipar);
	//CMD REQ DAT with ID
	quint16	udp_send_req_id_to(quint8 ipar);

	int	udp_set_ans(quint8 type_send,quint8 type_dat);
	int	udp_send_to(quint8 type_send,quint8 type_dat,quint8 *buff=nullptr,quint16 len=0);

	quint16 udp_parse_framein(quint8 *in_buf,quint16 len);
	quint16	pc_get_req_dat(quint8 num_req,quint8 *buf);
	int pc_set_rec_dat(quint8 cmd,void *in_buf);
	quint16 parse_frame(quint8 *in_buf,quint16 len);
protected:
	bool send_and_wait_get(const quint8 type, const quint8 cmd, quint8* out_dat, quint16* res_len=0);
	bool send_and_wait_put(const quint8 type, const quint8 cmd, quint8* in_dat, quint16 len);
	bool  _send_and_wait(const quint8 type, const quint8 cmd, quint8 *in_dat
	, quint16 in_len, quint8 *out_dat=nullptr, quint16 *out_len=nullptr);

	qint16 req_cnt;
	bool initing;
///	bool apply_params();
///	bool send_par_to_dev();
	quint32	m_changed_param;

	par_contr_t* p_param;
	par_contr_t m_param;
	QVector<qint16>* p_imp_ampl;

	bool send_param();
	quint8 *par_trk_buff;
	bool socket_run;
	void start_timer(quint32);

public:
	bool dev_get_cur_rej(quint8 *btmp);
	bool dev_set_cur_rej(quint8 rej);

	int frames_sent;
protected:
	bool stop_def_rej;
	void obr_cmd(req_cmd_t& t_req_cmd);

/// for debug slots =================================
public slots:
	void sl_put_req_xil(xil_dat_req_t ireq);
	void sl_get_xil(void);
	void sl_put_xil(xil_dat_req_t ireq);
	////void sl_set_out_a(void*);
	///bool send_all_par_to_dev();
	void sl_set_stop_def_rej(bool on);
	void sl_put_req_dac(xil_dat_req_t ireq);
	void sl_put_dac(xil_dat_req_t ireq);
	void sl_get_dac(void);
	void sl_get_stat(void);

public slots:
	void req_timer_timeout();

	void slot_res_udp_dat();

	void SlotSetIp(QString, int);
	void SlotSetIniting(bool);
	void sl_start(bool);
///	void start_timer(quint32);
	void sl_start_timer(quint32);
	void sl_set_run(bool);
	void sl_dev_set_cur_rej(quint8 rej);
	bool dev_put_param(quint8* iparam, quint16 num_bytes);
	bool dev_get_version_str(quint8* btmp);

signals:
	void s_send_udp_dat(char*,int);
///	void s_change_uns(int);
	void s_set_conn_stat(bool);
	void s_set_host(QString, int);
	void sign_ready_data(quint8 type);
	////void signal_unsuccesfull();
	void signal_connect_setted();
	void  s_Start(bool);
	void  s_ready_xil_dat(xil_dat_req_t);
	///void  s_set_run(bool);
};

#endif // _PC_UDP_H_
