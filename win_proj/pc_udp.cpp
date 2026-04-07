#include "pc_udp.h"
////#include "device_cmd.h"

static quint8 CurRej=0;

void set_cur_rej(quint8 rej)
{
	CurRej=rej;
}
quint8 get_cur_rej(void)
{
return CurRej;
}

pc_udp::pc_udp(int *pport,QString &_ip_addr, xil_dat_req_t* o_data, par_contr_t* par_contr
	          , bool* end_cmd, quint32* g_changed_param, QVector<qint16>* imp_ampl
	          ,quint8* last_cmd_good, udp_stat_t* udp_stat)
	: 
	pUdpSendRes(nullptr), attached(false)
	,ip_addr(_ip_addr)
	,p_odata(o_data)
	, p_param(par_contr)
	, p_end_cmd(end_cmd)
	, pg_changed_param(g_changed_param)
	,p_imp_ampl(imp_ampl)

	, p_last_Cmd_good(last_cmd_good)
	,p_udp_stat(udp_stat)
	,req_cnt(0)
	,m_changed_param(0)
///	,m_cmd(0)
////	,m_cmd_val(0)
	,initing(false)
	, stop_def_rej(false)
///	,prev_unsuccesfull_conn(-1)
////	,unsuccesfull_conn(0)
	,timeout_resive(DEF_TIMEOUT_RSV)///,	//	// в мс
	,par_trk_buff( new quint8[ OFFS_PARAM_DAT+sizeof(par_contr_t)+OFFS_FLG_CHNG ] )
	,frames_sent(0)

////	p_port(pport),ip_addr(_ip_addr)
{
pUdpSendRes= new udp_send_res(p_udp_stat);
p_udp_send_res_thr = new QThread();

pUdpSendRes->moveToThread(p_udp_send_res_thr);

connect(this, SIGNAL(s_send_udp_dat(char*, int)), pUdpSendRes, SLOT(slot_send_udp_dat(char*, int)));
connect(pUdpSendRes, SIGNAL(s_res_dat()), this, SLOT(slot_res_udp_dat()));
connect(this, SIGNAL(s_set_host(QString, int)), pUdpSendRes, SLOT(slot_set_host(QString, int)));
connect(this, SIGNAL(s_Start(bool)), pUdpSendRes, SLOT(sl_Start(bool)));

p_udp_send_res_thr->start();

}

pc_udp::~pc_udp()
{
}
void pc_udp::udp_pc_init(void)
{
	/*
p_udp_stat->in_buf = (quint8*) malloc(MAX_FRAME_LEN);
p_udp_stat->snd_buff	= (quint8*)malloc(MAX_FRAME_LEN);
p_udp_stat->t_snd_buff1	= (quint8*)malloc(MAX_FRAME_LEN);
p_udp_stat->t_snd_buff2	= (quint8*)malloc(MAX_FRAME_LEN);
*/
}


bool pc_udp::dev_get_cur_rej(quint8 *btmp)
{
	return true;
///	return send_and_wait_get(CMD_REQ_DAT, NUM_REQ_REJ, btmp);
}
bool pc_udp::dev_set_cur_rej(quint8 rej)
{
	qDebug() << "Rej: " << rej;
	return true;

///return send_and_wait_put(CMD_PUT_DAT, NUM_SET_REJ, &rej,1);

}

void pc_udp::sl_dev_set_cur_rej(quint8 rej)
{
	dev_set_cur_rej(rej);
}

bool pc_udp::dev_put_param(quint8 *iparam, quint16 num_bytes)
{
	return true;
///	return _send_and_wait(CMD_PUT_DAT, REQ_PUT_PAR, reinterpret_cast<quint8 *>(iparam), num_bytes);
}

bool pc_udp::dev_get_version_str(quint8 *btmp)
{
	return true;
///	return send_and_wait_get(CMD_REQ_DAT, NUM_REQ_GET_VERS, btmp);
}

bool pc_udp::_send_and_wait(const quint8 type, const quint8 cmd, quint8 *in_dat
	, quint16 in_len, quint8 *out_dat, quint16 *out_len)
{
quint16 olen =0;
quint8 btmp;
///if (!attached)
///   return false;
olen =udp_send_to(type,cmd,in_dat,in_len);
el_timer.start();

p_udp_stat->reseive_wait = true;
emit s_send_udp_dat((char*)p_udp_stat->snd_buff,olen);

while(p_udp_stat->reseive_wait)
	{
	int	time_count = el_timer.elapsed();

	//qDebug() << "Receive wait time: " << time_count << "milliseconds";
	if(time_count>=timeout_resive)
		{
		qDebug() << "Receive timeout!!!!: " << time_count << "ms";
		emit s_set_conn_stat(false);
		*p_end_cmd = true;
		*p_last_Cmd_good = 0;
		return false;
		}
	}
if(out_dat)
	{
	memcpy(out_dat, p_udp_stat->in_buf+sizeof(hdlc_head_t), p_udp_stat->len_dat_bytes);
	}

if(out_len != nullptr)
	*out_len = p_udp_stat->len_dat_bytes;
emit s_set_conn_stat(true);
*p_end_cmd = true;
*p_last_Cmd_good = p_udp_stat->ans_Cmd;
qDebug() << "_send_and_wait: " << QString::number(*p_last_Cmd_good, 16);

return true;
}
/*
void pc_udp::sl_set_out_a(void* buf)
{
	p_odat = buf;
}
*/
bool pc_udp::send_and_wait_get(const quint8 type, const quint8 cmd, quint8 *out_dat,quint16 *res_len)
{
return _send_and_wait(type, cmd, nullptr, 0, out_dat, res_len);	// 
}
bool pc_udp::send_and_wait_put(const quint8 type, const quint8 cmd, quint8* in_dat, quint16 len)
{
	return _send_and_wait(type, cmd, in_dat, len, nullptr, 0);	// 
}

///==================================
quint16 pc_udp::udp_set_frame(quint8 *data, int len)
{
addfcs16((char*)data,len);				///
memcpy(p_udp_stat->snd_buff,data,len +2);
 
return len +2;
}
int g_flg_req_dat=0;
quint8 dev_state=0;

quint16	pc_udp::pc_get_req_dat(quint8 num_req,quint8 *buf)
{
quint16 rez=0;
#if 0
switch(num_req)
	{
	case NUM_REQ_STAT:
		*buf=dev_state;
		rez=1;
		break;
	case NUM_REQ_REJ:
		*buf=get_cur_rej();
		rez=1;
		break;
	case NUM_REQ_XIL_DAT:
		break;

	break;
	case NUM_REQ_GET_VERS:
		{
		char t_buf[256];
	////	rez=get_vers(t_buf);
	////	memcpy(buf, &rez,sizeof(quint16));
	////	memcpy(buf+sizeof(quint16), t_buf,rez);
		rez+=sizeof(quint16);
		}
		break;
	default:
		rez=0;
		break;
	}
#endif
return rez;
}
quint16	pc_udp::udp_send_req_to(quint8 ipar)
{
////static quint16 tst_offs=0;
quint8 p_tmp[8];
p_udp_stat->req_cmd=ipar;
///ResDat.req_cmd=ipar;
/////////////////////////p_tmp[0]  = CMD_REQ_DAT;
p_tmp[1] = ipar;
p_tmp[2] = 0;///offs
p_tmp[3] = 0;///offs
return udp_set_frame(p_tmp, 4);
}


quint16	pc_udp::udp_send_ack_to(quint8 ipar)
{
quint8 p_tmp[8];
////////////////////p_tmp[0]  = CMD_ACK;
p_tmp[1] = ipar;
return udp_set_frame(p_tmp, 2);
}

#if 1
int	pc_udp::udp_send_to(quint8 type_send,quint8 type_dat,quint8 *buff,quint16 len)
{
int res=0;
#if 0
quint16 len_dat=0;
int num_send=0;
quint8 *p_tmp1;
quint8 *p_tmp = p_udp_stat->t_snd_buff1;
if(type_send == CMD_ACK)
	{
	res=udp_send_ack_to(type_dat);
	}
else if(type_send == CMD_REQ_DAT)
	{

//	qDebug() << "CMD_REQ_DAT - Value of type_dat: " << type_dat;
	res=udp_send_req_to(type_dat);
	}
else if(type_send == CMD_PUT_REQ_DAT)
	{
	*p_tmp++  = CMD_PUT_REQ_DAT;
	num_send++;
	*p_tmp++  = type_dat;
	num_send++;
	p_tmp1 = p_tmp+2;  

	// Debugging the value of type_dat
//qDebug() << "Value of type_dat: " << type_dat;

// Debugging the value of p_tmp1
///qDebug() << "Value of p_tmp1: " <<(*p_tmp1);

	len_dat = pc_get_req_dat(type_dat,p_tmp1);

//qDebug() << "Value of len_dat: " << len_dat;
	
	*p_tmp++  = len_dat&0xff;
	num_send++;
	*p_tmp++  = (len_dat>>8)&0xff;
	num_send++;
	if(len_dat > HDLC1_MAX_FRAME_LEN)
		len_dat = HDLC1_MAX_FRAME_LEN;
	num_send += len_dat;
	res=udp_set_frame(p_udp_stat->t_snd_buff1, num_send);
	}
else if(type_send == CMD_PUT_DAT)
	{
	*p_tmp++  = CMD_PUT_DAT;
	num_send++;
	*p_tmp++  = type_dat;
	num_send++;
	if(len > HDLC1_MAX_FRAME_LEN)
		len = HDLC1_MAX_FRAME_LEN;
	p_tmp1 = p_tmp+2;  
	memcpy(p_tmp1,buff,len);
	*p_tmp++  = len&0xff;
	num_send++;
	*p_tmp++  = (len>>8)&0xff;
	num_send++;
	num_send += len;
	///qDebug() << "Value of p_tmp1: " <<(*p_tmp1);
	////if(buff)
	///qDebug() << "buff: " <<*buff;

	res=udp_set_frame(p_udp_stat->t_snd_buff1, num_send);
	}
#endif
return res;
}
#endif
/*
void pc_udp::SlotResetConnCount()
{
unsuccesfull_conn = 0;
prev_unsuccesfull_conn=-1;
}
*/
///========================================================================================
///========================================================================================
/*
void pc_udp::slot_put_motor_cmd_stop()
{
motor_struct_t motor_struct;
motor_struct.dir = 0;
motor_struct.speed = 0;
_send_and_wait(CMD_PUT_DAT, CMD_MOTOR_GO, reinterpret_cast<quint8 *>(&motor_struct), sizeof(motor_struct_t));
}
*/
void pc_udp::slot_res_udp_dat()
{
////	reseive_wait=false;
qDebug() << " slot_res_udp_dat " ;
////udp_parse_framein(ResDat.buff,ResDat.len);
}

void pc_udp::SlotSetIp(QString _ip, int iport)
{
emit s_set_host(_ip, iport);
}
/*
void pc_udp::sl_set_run(bool irun)
{
	socket_run = irun;
}
*/
/*
bool pc_udp::apply_params()
{
quint8 curr_mode = 0;
bool result = dev_get_cur_rej(&curr_mode);
#if 0
if(result )
	{
	if(curr_mode==REJ_MENU)
		{
		result = dev_set_cur_rej(REJ_TUNE);			// 
		}
		Params::g_changed_param = CHNG_ALL;
		result = send_par_to_dev();/// send_all_par_to_dev();
///		if(result )
////			Params::g_changed_param=0;
	}
#endif
return result;
}
*/

///===========================================================
bool pc_udp::send_param()
{
#if 0
	quint16 tnum_bytes = 0;
	quint16 t_offs = 0;

	if (*pg_changed_param == 0)
		return true;
	memcpy(&m_param, p_param, sizeof(par_contr_t));
	m_changed_param = *pg_changed_param;
	*pg_changed_param = 0;
	memcpy(par_trk_buff + OFFS_FLG_CHNG, &m_changed_param, sizeof(quint32));
	t_offs = OFFS_PARAM_DAT;
	tnum_bytes = OFFS_PARAM_DAT;

	par_sent_t& m_sent_par = m_param.sent_par;


	if (m_changed_param & CHNG_TIMP_LEN)
	{
		memcpy(par_trk_buff + t_offs, &m_sent_par.Timp_len, sizeof(quint16));
		t_offs += sizeof(quint16);
		tnum_bytes += sizeof(quint16);
		m_changed_param &= ~CHNG_TIMP_LEN;
	}
	if (m_changed_param & CHNG_TIMP_OFFSET)
	{
		memcpy(par_trk_buff + t_offs, &m_sent_par.Timp_offset, sizeof(quint16));
		t_offs += sizeof(quint16);
		tnum_bytes += sizeof(quint16);
		m_changed_param &= ~CHNG_TIMP_OFFSET;
	}
	if (m_changed_param & CHNG_TCYCLE)
	{
		memcpy(par_trk_buff + t_offs, &m_sent_par.Tcycle, sizeof(quint16));
		t_offs += sizeof(quint16);
		tnum_bytes += sizeof(quint16);
		m_changed_param &= ~CHNG_TCYCLE;
	}
	if (m_changed_param & CHNG_TDEF)
	{
		memcpy(par_trk_buff + t_offs, &m_sent_par.Tdef, sizeof(quint16));
		t_offs += sizeof(quint16);
		tnum_bytes += sizeof(quint16);
		m_changed_param &= ~CHNG_TDEF;
	}
	if (m_changed_param & CHNG_IMP_POINTS)
	{
		memcpy(par_trk_buff + t_offs, &m_sent_par.Timp_len, sizeof(qint16));
		t_offs += sizeof(quint16);
		memcpy(par_trk_buff + t_offs, p_imp_ampl->data(), sizeof(qint16) * m_sent_par.Timp_len);
		t_offs += sizeof(qint16) * m_sent_par.Timp_len;
		tnum_bytes += sizeof(qint16) * m_sent_par.Timp_len + sizeof(quint16);
		m_changed_param &= ~CHNG_IMP_POINTS;
	}
	if (m_changed_param & CHNG_KUS)
	{
		memcpy(par_trk_buff + t_offs, &m_sent_par.kus, sizeof(quint16));
		t_offs += sizeof(quint16);
		tnum_bytes += sizeof(quint16);
		m_changed_param &= ~CHNG_KUS;
	}
	if (m_changed_param & CHNG_OFFS)
	{
		memcpy(par_trk_buff + t_offs, &m_sent_par.offs, sizeof(quint16));
		t_offs += sizeof(quint16);
		tnum_bytes += sizeof(quint16);
		m_changed_param &= ~CHNG_OFFS;
	}

	quint16 tnum_bytes1;
	tnum_bytes1 = tnum_bytes + OFFS_FLG_CHNG;
	memcpy(par_trk_buff, &tnum_bytes, sizeof(quint16));

	return dev_put_param(par_trk_buff, tnum_bytes);
#endif
	return false;
}

void pc_udp::sl_set_run(bool irun)
{
	socket_run = irun;
}
void pc_udp::obr_cmd(req_cmd_t&req_cmd)
{
	bool res;
#if 0
	switch (req_cmd.cmd) {
	case NUM_SEND_XIL_DAT:
		dev_put_xil(req_cmd.req);
		break;
	case NUM_SEND_XIL_REQ:
		dev_put_req_xil(req_cmd.req);
		break;
	case NUM_REQ_XIL_DAT:
		dev_get_xil();
		break;
	case NUM_SEND_SPI_DAC_DAT:
		dev_put_dac(req_cmd.req);
		break;
	case NUM_SEND_SPI_DAC_REQ:
		dev_put_req_dac(req_cmd.req);
		break;
	case NUM_REQ_SPI_DAC_DAT:
		dev_get_dac();
		break;
	case NUM_REQ_STAT:
		dev_get_stat();
		break;
	}
#endif
}

void pc_udp::req_timer_timeout()
{
	if (!socket_run)
		return;
	req_cmd_t t_req_cmd;
	send_param();
	int cnt = req_que.count();
	if (cnt)
	{
		///		qDebug() << "cnt= "<<cnt;
		t_req_cmd = req_que.dequeue();
		obr_cmd(t_req_cmd);
	}

}
void pc_udp::SlotSetIniting(bool ini)
{
initing=ini;
}
void pc_udp::timerEvent(QTimerEvent* event)
{
	req_timer_timeout();
////	qDebug() << "Timer ID:" << event->timerId();

}
void pc_udp::start_timer(quint32 dt)
{
	if (dt == 0) {
		///	req_timer->stop();
		killTimer(timer_id);
		qDebug() << "start_timer[0]" << timer_id;
	}
	else {
		timer_id = startTimer(dt);
		qDebug() << "start_timer[dt]" << timer_id;
	}
	////  req_timer->start(dt);


  ///	emit s_Start(start_stop);
}
void pc_udp::sl_start(bool start_stop)
{
	emit s_Start(start_stop);
}



void pc_udp::sl_start_timer(quint32 dt)
{
	start_timer(dt);
	qDebug() << " sl_start_timer " << dt;
#if 0
	if (dt == 0)
		///	req_timer->stop();
		killTimer(1);
	else
		startTimer(50);
	////  req_timer->start(dt);


  ///	emit s_Start(start_stop);
#endif
}
///=========================================================================================================
void pc_udp::dev_get_stat(void)
{
#if 0
	bool rez;
	if (p_odata)
	{
		xil_dat_req_t* odat = (xil_dat_req_t*)p_odata;
		///		rez = send_and_wait_get(CMD_REQ_DAT, NUM_REQ_XIL_DAT, reinterpret_cast<quint8*>(odat));
		rez = send_and_wait_get(CMD_REQ_DAT, NUM_REQ_STAT, reinterpret_cast<quint8*>(p_odata));
		if (rez)
		{
			;
			///	p_dev_data->odata
			////		emit s_ready_xil_dat(odat);
		///		qDebug() << " dev_get_xil " << odat->addr << odat->data[0];
		}
		///		qDebug() << " dev_get_xil " << odat->addr << odat.data[0];
	}
#endif
}

void pc_udp::dev_put_req_xil(xil_dat_req_t ireq)
{
	xil_dat_req_t t_req = ireq;
////	_send_and_wait(CMD_PUT_DAT, NUM_SEND_XIL_REQ, reinterpret_cast<quint8*>(&t_req), sizeof(xil_dat_req_t));
////	qDebug() << " dev_put_req_xil ";
}

void pc_udp::dev_get_xil(void)
{
#if 1
	bool rez=false;
	if (p_odata)
	{
		xil_dat_req_t *odat= (xil_dat_req_t*)p_odata;
///		rez = send_and_wait_get(CMD_REQ_DAT, NUM_REQ_XIL_DAT, reinterpret_cast<quint8*>(odat));
	////	rez = send_and_wait_get(CMD_REQ_DAT, NUM_REQ_XIL_DAT, reinterpret_cast<quint8*>(p_odata));
		if (rez)
		{
			;
		///	p_dev_data->odata
		////		emit s_ready_xil_dat(odat);
			qDebug() << " dev_get_xil " << odat->addr << odat->data[0];
		}
///		qDebug() << " dev_get_xil " << odat->addr << odat.data[0];
	}
#endif
}
void pc_udp::dev_put_xil(xil_dat_req_t ireq)
{
	xil_dat_req_t t_req = ireq;
////	_send_and_wait(CMD_PUT_DAT, NUM_SEND_XIL_DAT, reinterpret_cast<quint8*>(&t_req), sizeof(xil_dat_req_t));
}

void pc_udp::dev_put_dac(xil_dat_req_t ireq)
{
	xil_dat_req_t t_req = ireq;
///	_send_and_wait(CMD_PUT_DAT, NUM_SEND_SPI_DAC_DAT, reinterpret_cast<quint8*>(&t_req), sizeof(xil_dat_req_t));
}
void pc_udp::dev_put_req_dac(xil_dat_req_t ireq)
{
	xil_dat_req_t t_req = ireq;
////	_send_and_wait(CMD_PUT_DAT, NUM_SEND_SPI_DAC_REQ, reinterpret_cast<quint8*>(&t_req), sizeof(xil_dat_req_t));
	////	qDebug() << " dev_put_req_xil ";
}

void pc_udp::dev_get_dac(void)
{
#if 1
	bool rez=false;
	if (p_odata)
	{
		xil_dat_req_t* odat = (xil_dat_req_t*)p_odata;
		///		rez = send_and_wait_get(CMD_REQ_DAT, NUM_REQ_XIL_DAT, reinterpret_cast<quint8*>(odat));
///		rez = send_and_wait_get(CMD_REQ_DAT, NUM_REQ_SPI_DAC_DAT, reinterpret_cast<quint8*>(p_odata));
		if (rez)
		{
			;
			///	p_dev_data->odata
			////		emit s_ready_xil_dat(odat);
			qDebug() << " dev_get_dac " << odat->addr << odat->data[0];
		}
		///		qDebug() << " dev_get_xil " << odat->addr << odat.data[0];
	}
#endif
}


void pc_udp::sl_put_xil(xil_dat_req_t ireq)
{
	req_cmd_t  t_req;
////	t_req.cmd = NUM_SEND_XIL_DAT;
	t_req.req = ireq;
	req_que.enqueue(t_req);

	////xil_dat_req_t t_req = ireq;

	/// _send_and_wait(CMD_PUT_DAT, NUM_SEND_XIL_DAT, reinterpret_cast<quint8 *>(&t_req), sizeof(xil_dat_req_t));
}

void pc_udp::sl_put_req_xil(xil_dat_req_t ireq)
{
	req_cmd_t  t_req;
////	t_req.cmd = NUM_SEND_XIL_REQ;
	t_req.req = ireq;
	*p_odata = ireq;
	req_que.enqueue(t_req);
////	qDebug() << " sl_put_req_xil ";
////	int cnt = req_que.count();
////	qDebug() << "cnt0= " << cnt;

	///	xil_dat_req_t t_req = ireq;
	///	_send_and_wait(CMD_PUT_DAT, NUM_SEND_XIL_REQ, reinterpret_cast<quint8*>(&t_req), sizeof(xil_dat_req_t));
}

void pc_udp::sl_get_xil(void)
{
	req_cmd_t  t_req;
/////	t_req.cmd = NUM_REQ_XIL_DAT;
	///	t_req.req = ireq;
	req_que.enqueue(t_req);
///	qDebug() << " sl_get_xil ";
////	int cnt = req_que.count();
////	qDebug() << "cnt2= " << cnt;

	///	bool rez;
		///	xil_dat_req_t t_req = ireq;
////	xil_dat_req_t odat;

	///	rez = send_and_wait_get(CMD_REQ_DAT, NUM_REQ_XIL_DAT, reinterpret_cast<quint8*>(&odat));
	///	if (rez)
	///		emit s_ready_xil_dat(odat);
}
void pc_udp::sl_set_stop_def_rej(bool on) {
	stop_def_rej = on;
}

void pc_udp::sl_put_dac(xil_dat_req_t ireq)
{
	req_cmd_t  t_req;
////	t_req.cmd = NUM_SEND_SPI_DAC_DAT;
	t_req.req = ireq;

	req_que.enqueue(t_req);

	////xil_dat_req_t t_req = ireq;

	/// _send_and_wait(CMD_PUT_DAT, NUM_SEND_XIL_DAT, reinterpret_cast<quint8 *>(&t_req), sizeof(xil_dat_req_t));
}
void pc_udp::sl_put_req_dac(xil_dat_req_t ireq) {
	req_cmd_t  t_req;
////	t_req.cmd = NUM_SEND_SPI_DAC_REQ;
	t_req.req = ireq;
	*p_odata = ireq;

	req_que.enqueue(t_req);

}
void pc_udp::sl_get_dac(void)
{
	req_cmd_t  t_req;
////	t_req.cmd = NUM_REQ_SPI_DAC_DAT;
	req_que.enqueue(t_req);
}
void pc_udp::sl_get_stat(void)
{
	req_cmd_t  t_req;
////	t_req.cmd = NUM_REQ_STAT;
	req_que.enqueue(t_req);
}
