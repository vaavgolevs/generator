#include "udp_send_res.h"

extern int checkfcs16(char *cp, const int len );
quint32 cnt_error=0;


udp_send_res::udp_send_res(udp_stat_t *udp_stat):timeout_resive(DEF_TIMEOUT_RSV)
				,p_udp_stat(udp_stat),connected(false), socket_run(false)
				,host_addr("192.168.1.99")	//	

{
	q_host_addr = QHostAddress("192.168.1.99");
///	QHostAddress adress = QHostAddress("192.168.20.230");
////q_host_addr.setAddress(ip_addr);
///udpSocket = new QUdpSocket(this);
///connect(udpSocket, SIGNAL(readyRead()), this,SLOT(udp_ready_dat()));
///udpSocket->bind(q_host_addr);
}

void udp_send_res::slot_set_host(QString addr, int iport)
{
///host_addr = addr;
///q_host_addr.setAddress(host_addr);
port = iport;
////udpSocket->bind(q_host_addr);
}
void udp_send_res::sl_Start(bool start_stop)
{
if (start_stop)
	{
	udpSocket = new QUdpSocket(this);
	connect(udpSocket, SIGNAL(readyRead()), this, SLOT(udp_ready_dat()));
	connect(udpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
	udpSocket->bind(port);
	socket_run = true;

}
else if (socket_run)
	{
///	udpSocket->close();
	disconnect(udpSocket, SIGNAL(readyRead()), this, SLOT(udp_ready_dat()));
	disconnect(udpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
	delete udpSocket;
	udpSocket = nullptr;
	socket_run = false;

	}
emit s_set_run(socket_run);
}
void udp_send_res::socketError(QAbstractSocket::SocketError err)
{
	qDebug() << "SocketError " << QString("Error: %1").arg(err);
}

udp_send_res::~udp_send_res()
{

}
void udp_send_res::set_conn(bool connected)
{

}

int udp_send_res::udp_check_framein(quint8 *i_buff,quint16 i_len)
{
int res = 1;
if(i_len < 3 )
	{
////	g_udp_stat.err_len++;
	res = -1;
	}
if( !checkfcs16((char*)i_buff, i_len - 2 ) )
	{
		res = -2;
	}
return res;
}
void udp_send_res::udp_ready_dat()
{
QByteArray datagram;
QHostAddress curr_host_addr;
quint16 t_port=0;
quint16 olen=0;
qint64  next_size;
hdlc_head_t *p_hdlc_head;

while(udpSocket->hasPendingDatagrams()) 
	{
	next_size=udpSocket->pendingDatagramSize();
	if(next_size==0)
		return;
	datagram.resize(next_size);
///	udp_stat.len_rsv_dat=size;
////		p_dev_data->time_stamp();

////elaps_timer_2.start();
///	udpSocket->readDatagram(datagram.data(), datagram.size(), &curr_host_addr,&t_port );
    next_size = udpSocket->readDatagram(datagram.data(), next_size, &curr_host_addr);

	if(curr_host_addr.toIPv4Address() != q_host_addr.toIPv4Address())
		return;
	set_conn(true);
	qDebug() << "next_size : " << next_size;

	olen=obr_udp_res_cmd((quint8 *)datagram.data(), next_size);
	qDebug() << "olen : " << olen;
#if 0
	if(olen)
		{
		p_hdlc_head=(hdlc_head_t *)datagram.data();
		if(p_hdlc_head->type_send==CMD_PUT_REQ_DAT)
			{
			if(p_hdlc_head->type_dat==p_udp_stat->req_cmd)
				{
					p_udp_stat->len_dat_bytes=p_hdlc_head->len_dat;
					p_udp_stat->reseive_wait=false;
					p_udp_stat->ans_Cmd = p_hdlc_head->type_dat;
					qDebug() << "CMD_PUT_REQ_DAT : "<< QString::number(p_hdlc_head->type_dat, 16);

				}
			}
		else if(p_hdlc_head->type_send == CMD_ACK)
			{
			qDebug() << "CMD_ACK : " << QString::number(p_hdlc_head->type_dat, 16);
			////	qDebug() << " type_dat = " << QString::number( p_hdlc_head->type_dat, 16) << " req_cmd = " << QString::number( p_udp_stat->req_cmd, 16) ;;
			p_udp_stat->len_dat_bytes=0;
			p_udp_stat->reseive_wait=false;
			p_udp_stat->ans_Cmd = p_hdlc_head->type_dat;
			///	qDebug() << "CMD_ACK ok ";

			}
	////???	emit s_res_dat();
		}
#endif
	}

}

quint16 udp_send_res::obr_udp_res_cmd(quint8 *i_buff,quint16 i_len)
{
int res;
quint16 rez=0;
if((res= udp_check_framein(i_buff,i_len))>0 )	//
	{

	//qDebug() << "i_buff data len"<<i_len;

	//if (i_len < 8) { qDebug() << "*******************ALERT****************" ;}
		/*
int lenin;
lenin = i_len;
if (i_len>16) {lenin=16;}

 for (int i = 0; i < lenin; ++i) {
        qDebug() << "Value at index" << i << ":" << static_cast<int>(i_buff[i]);
    }
 */
	memcpy(p_udp_stat->in_buf,i_buff,i_len);
	///p_udp_stat->_len_rsv_dat=i_len;
	rez=i_len;


//////	p_dev_data->time_stamp();
	}
else
	{
	rez=0;
	 qDebug() << " udp error framein = " << res;
    if(i_len > 0x400)
    	{
		qDebug() << " udp oversize = " << i_len;
    	}
 	}
return rez;	
}

void udp_send_res::slot_send_udp_dat(char* buff,int len)
{
////q_host_addr.setAddress(ip_addr);

//qDebug() << "Data sent size" << len ; 
if (socket_run)
	{
		udpSocket->writeDatagram((const char*)buff, len, q_host_addr, port);
		elaps_timer.start();
	///	qDebug() << "Data sent size" << len << (int)(*buff) << (int)(*(buff+1)) << (int)(*(buff+2)) << (int)(*(buff + 3));

	}
}


