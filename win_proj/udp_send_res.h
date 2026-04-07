#ifndef _UDP_SEND_RES_H_
#define _UDP_SEND_RES_H_

#include <QObject>
#include <QtNetwork/QUdpSocket>
#include <QElapsedTimer>
///#include "cmdreq.h"

////#include "dev_data.h"
#include "dev_interf.h"

#ifndef MAX_FRAME_LEN
#define MAX_FRAME_LEN			16000    ///60000
#endif

#define DEF_TIMEOUT_RSV 500 // в мс

struct hdlc_head_t
{
quint8 type_send;
quint8 type_dat;
quint16 len_dat;
///quint16 frames_received;
};

struct udp_stat_t
{
////bool stop_reseive;
bool reseive_wait;
quint8 	in_buf[MAX_FRAME_LEN];
quint16	len_dat_bytes;					//
quint16	__len_rsv_dat;
quint8 	snd_buff[MAX_FRAME_LEN];				//
quint8 	t_snd_buff1[MAX_FRAME_LEN];			//
quint8 	t_snd_buff2[MAX_FRAME_LEN];			//
///quint16	frame_out_len;
quint16	offs_snd_buf;
quint16	snd_buff2_len;
quint8 	req_cmd;					//
quint8 	ans_Cmd;
quint16 type_send;					//
int	err_len;
int	frames_received;
};

class udp_send_res : public QObject
{
Q_OBJECT

public:
	udp_send_res(udp_stat_t *udp_stat=nullptr);
	virtual ~udp_send_res();
	udp_stat_t *p_udp_stat;
	bool connected;
	QElapsedTimer elaps_timer;
///	QElapsedTimer elaps_timer_2;
///	c_dev_data *p_dev_data;
protected:
	const int timeout_resive;
	QUdpSocket *udpSocket;
	int port;
	QString host_addr;
	QHostAddress q_host_addr;
	QByteArray datagram;

	quint16 obr_udp_res_cmd(quint8 *i_buff,quint16 i_len);
	int udp_check_framein(quint8 *i_buff,quint16 i_len);
	void set_conn(bool connected);
	bool socket_run;

public slots:
	void udp_ready_dat();
	void slot_send_udp_dat(char*,int);
	void slot_set_host(QString addr, int iport);
	void sl_Start(bool);
	void socketError(QAbstractSocket::SocketError err);

////	void slot_set_ip(QString);

signals:
	void s_res_dat();
	void s_set_run(bool);
};

#endif // _UDP_SEND_RES_H_
