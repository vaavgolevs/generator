#ifndef _PARAMS_H_
#define _PARAMS_H_

#include <QGlobal.h>
#include <QString>

#ifndef OFF
#define	OFF	0
#endif

#ifndef ON
#define	ON	1
#endif


#define DEVICE_METKA		"velographI set 1"

#ifndef PI
#define PI 3.141592653f
#endif

#define MAX_PARAMS_LEN	2048				// участок под par_session_t



#define NUM_DIG_NCHAN	1
#define NUM_DIG_NTACT	1
#define NUM_DIG_BEGR	6
#define NUM_DIG_ENDR	6
#define NUM_DIG_NUS		1
#define NUM_DIG_KUS		2
#define NUM_DIG_TT		1
#define NUM_DIG_TV		2
#define NUM_DIG_NSTROB	1
#define NUM_DIG_POR		4
#define NUM_DIG_BSTR	6
#define NUM_DIG_LSTR	6
#define NUM_DIG_NGEN	1
#define NUM_DIG_LEN_IMP	3
#define NUM_DIG_NPOW	1
#define NUM_DIG_VPOW	3
#define NUM_DIG_NUMPV	1
#define NUM_DIG_TVRCH	6
#define NUM_DIG_KVRCH	2


#define NUM_DIG_ALPHA	4
#define NUM_DIG_T_PR	5
#define NUM_DIG_CC		5
#define	NUM_DIG_X_OFFSET	4

#define	NUM_DIG_THICK	6



///#define NUM_DIG_MOTOR_SPEED	4
#define NUM_DIG_QUINT16		5


///++++++++++++++++++++++++++++++++++++++++
#define	LEN_OSC			256
#define NUM_CHANS 		2
////#define NUM_GENS 		2

#define NUM_USS 		2
#define NUM_TACTS		1
#define NUM_STRBS 		4
#define MAX_NUM_PVRCH	8

#define NUM_AMP			128	


#define NUM_PARK		256
#define NUM_LEVARK		3


#define MAX_TIME_DEF	65000


#define	MIN_AMPL		0
#define	MAX_AMPL		255
///++++++++++++++++++++++++++++++
#define	DEF_POW_VAL		140
#define	DEF_LEN_IMP		1

#define DEF_NUM_CHAN	0
#define DEF_NUM_TACT	0
#define	DEF_BEGR		0
#define DEF_ENDR		20
#define DEF_STEPR		7
#define DEF_NUM_US		0
#define DEF_NUM_STRB	4
#define DEF_STRB		0
#define DEF_NUM_GEN		0
#define DEF_NUM_POW		0
#define DEF_NUM_PARAM	1				///
#define DEF_PVRCH		0


//#define	DEF_KUS1		150
#define	DEF_REJ_US		1

#define DEF_TT			1
#define DEF_TV			7


#define	DEF_BEG_STRB	50
#define	DEF_LEN_STRB	100
#define	DEF_POR_STRB	50

#define	DEF_NUM_POINTV	2
#define	DEF_KUS_V		0
#define	DEF_TIME_V		10


#define	DEF_ALPHA		0
#define	DEF_T_PR		0
#define	DEF_CC			5900

#define	DEF_X_OFFSET	0

#define DEF_THICK		200


///++++++++++++++++++++++++++++++
#define	MIN_NUM_CHAN		0
#define	MAX_NUM_CHAN		(NUM_CHANS - 1)
#define	MIN_NUM_TACT		0
#define	MAX_NUM_TACT		(NUM_TACTS - 1)
#define	MIN_BEGR			0
#define	MAX_BEGR			60000
#define	MIN_ENDR			51
#define	MAX_ENDR			60000
#define	MIN_STEPR			0
#define	MAX_STEPR			50
#define	MIN_NUM_US		0
#define	MAX_NUM_US		(NUM_USS - 1)

#define	MIN_KUS			0
#define	MAX_KUS			80
#define	MIN_NUM_GEN		0
#define	MAX_NUM_GEN		(NUM_GENS - 1)
#define	MIN_LEN_IMP		1
#define	MAX_LEN_IMP		10


#define	MIN_NUM_PVRCH	0	


#define	MIN_NSTROB		0
#define	MAX_NSTROB		(NUM_STRBS - 1)
#define	MIN_POR			0
#define	MIN_POR_B_SCAN	1
#define	MAX_POR			255
#define	MIN_BSTR		0
#define	MAX_BSTR		60000
#define	MIN_LSTR		1
#define	MAX_LSTR		40000


#define	MIN_VPOW		0
#define	MAX_VPOW		255
#define	MIN_NUMPV		0
#define	MAX_NUMPV		(MAX_NUM_PVRCH - 1)
#define	MIN_TVRCH		0
#define	MAX_TVRCH		50000
#define	MIN_KVRCH		0
#define	MAX_KVRCH		MAX_KUS

#define	MIN_T_PR		0
#define	MAX_T_PR		8000
#define	MIN_CC			500
#define	MAX_CC			12000

#define	MIN_ALPHA		0
#define	MAX_ALPHA		900

#define MIN_X_OFFSET	0
#define MAX_X_OFFSET	500


#define MIN_THICK	 	20
#define MAX_THICK  		10000


/*
#define MIN_ZOND_FREQ	100
#define MAX_ZOND_FREQ	3000
*/



#define	MIN_QUINT16		0
#define	MAX_QUINT16		65534


#define INC_BEGR		1
#define INC_STEPR		1

#define INC_POR			1
#define INC_BSTR		1
#define INC_LSTR		1

#define INC_TVRCH		1


#define INC_T_PR		1
#define INC_CC			1
#define INC_ALPHA		1
#define COEF_POR		10
#define COEF_POR_INDEX	0.255f
#define COEF_LEN_IMP	25.0f
#define COEF_BEGR		10
#define COEF_ENDR		10
#define COEF_TVRCH		10
#define COEF_T_PR		100
#define COEF_ALPHA		10
#define COEF_X_OFFSET	10
#define COEF_THICK		10
#define TIME_T_STROB_KOEF		0.04f   ///0.125f
#define TIME_T_PR_KOEF			0.01f
#define TIME_T_STROB_EXTRA		0
#define STEP_KOEF 0.02f		
///#define TEN_F  10.0f
#define K_TIME_STROB  0.1f	    ///for strob
#define TIME_UNIT		0.02	///0.025f
// ?????
#define TACT_LEN_UNIT	160		///160nS
#define ZOND_FREQ_KOEF	(1000000000/TACT_LEN_UNIT)//2564.0f*2000.0f
#define DEF_LEN_HIPOW	1000
#define T_DEL_IZL		(3780/TACT_LEN_UNIT)
#define MIN_LEN_TACT	2000
#define DEF_LEN_TACT	4000
#define FLG_ON  (0x1<<0)
#define FLG_DN  (0x1<<1)

#define	MIN_KOEFF		0
#define	MAX_KOEFF		255
#define	MAX_KUS_ATT		75

#pragma pack (push, 1)
struct par_strb_t
{
	quint8 flg_strb;
	quint8 por;
///	quint8 down_up;
///	quint8 vibro;
	quint16 Beg;
	quint16 len;
};

struct point_vrch_t
{
	quint16	time;
	quint8	kus;
};

struct par_vrch_t
{
	quint8  on_vrch;
	quint8	num_p;
	quint16	time[MAX_NUM_PVRCH];
	quint8	kus[MAX_NUM_PVRCH];
};

struct par_strb_link_t
{
	quint8 link_por;
	quint8 link_kus;
};

struct par_us_t
{
	quint8 on_us;
	quint8 kus;
	quint8 detpar;				// ???
	quint8 	taut;
	quint8 	tauv;

	quint16	alpha;
	quint16 t_Pr;
	quint16 cc;
	quint16 x_offs;

	quint16 beg_r;
	quint8 stp_r;
	quint8 ots;										//otsechka
	quint8 num_strb;
	par_strb_t strb[NUM_STRBS];	 
	par_vrch_t vrch;

	quint8 on_strb_link;
	par_strb_link_t strb_link[NUM_STRBS];	

};


struct par_gen_t
{
	quint8 on_gen;
	quint8 len_imp;
};



struct par_takt_t
{
	quint16 len_hipow;
	quint16 len_tact;
////	par_gen_t gens[NUM_GENS];
	par_gen_t gen;
	par_us_t uss[NUM_USS];
};



struct par_ark_t
{
	quint8 on_ark;

	quint8 num_p;
	quint8 curr_p;

	quint16 time[NUM_PARK];
	quint8 value[NUM_PARK];

	quint8 num_lev;
	quint8 curr_lev;

	qint8 levels[NUM_LEVARK];
};


struct par_thick_t
{
	quint8 on_thick;
	quint16 thick_val;
};

struct par_dop_t
{
	quint8	curr_chan;				/// ???
	quint8	curr_tact;				///
	quint16	curr_begr;				///
	quint16	curr_endr;				///
	quint8	curr_step_r;			///

	quint8  curr_us;
	quint8  rej_osc;
	quint8  curr_pvrch;
	quint8  curr_strob;
////	quint8  curr_gen;

	quint8  axis_x_mode;
	quint8  axis_y_mode;
	quint16	curr_nparam;			/// ???
	par_ark_t ark;
	par_thick_t thick;
};


struct par_aws_t
{
	quint8 aws_calibrated;
	quint16 B_level;
};


struct par_dgs_t
{
	quint8 dgs_on;
	quint8 dgs_calibrated;

	quint8 mode;

	quint8 probe_set_diam;
	qint32 probe_diam;
	qint32 probe_S;

	quint16 probe_freq;

	qint32 KM;

	quint16 reffer_Val;
	quint16 reffer_Pow;
	quint16 curr_SE;
	quint16 extra_db;
	quint8 levelsMode;
};

struct par_trk_t
{
	quint8 vers_par;				// 
	quint8 num_tacts;				// число тактов в схеме
	quint8 num_izm;
	quint8 num_us;
	quint8 rsv;//// num_gen;
	quint8 pow_vol;					// byte
	par_takt_t takts[NUM_TACTS];
	par_dop_t par_dop;
};


struct par_contr_t
{
	quint8 version;
	par_trk_t trk_par;
///	par_dop_t par_dop;
	par_aws_t aws;
	par_dgs_t dgs;
};


#pragma pack (pop)

struct saved_par_contr_t
{
	QString description;
	QString probe;
///	char description[256];
///	probe;
	par_contr_t par_contr;
};

///++++++++++++++++++++++++++++++++++

#define CHNG_CHAN		(0x1<<0)
#define CHNG_TACT		(0x1<<1)
#define CHNG_NUS		(0x1<<2)
#define CHNG_ONUS		(0x1<<3)
#define CHNG_KUS		(0x1<<4)
#define CHNG_BEGR		(0x1<<5)
#define CHNG_ENDR		(0x1<<6)
#define CHNG_TT			(0x1<<7)
#define CHNG_TV			(0x1<<8)

#define CHNG_NSTRB		(0x1<<9)
#define CHNG_ALLSTRB	(0x1<<10)
#define CHNG_NUMSTRB	(0x1<<11)
#define CHNG_LINKSTRB	(0x1<<12)

#define CHNG_NUMPV		(0x1<<13)
#define CHNG_VRON		(0x1<<14)
#define CHNG_VCURP		(0x1<<15)
#define CHNG_VRCh		(0x1<<16)

#define CHNG_NGEN		(0x1<<17)
#define CHNG_ONGEN		(0x1<<18)

#define CHNG_LIMP		(0x1<<19)
#define CHNG_PVAL		(0x1<<21)

#define CHNG_PROBE		(0x1<<22)

#define CHNG_REJ_OSC	(0x1<<23)
#define CHNG_AXIS_DIAL	(0x1<<24)

#define CHNG_ARK	  	(0x1<<25)
#define CHNG_THICK		(0x1<<26)

#define CHNG_AWS		(0x1<<27)
#define CHNG_DGS		(0x1<<28)

#define CHNG_ALL_PAR		(0x1<<31)


#define OFFS_NUM_BYTES	0
#define OFFS_FLG_CHNG	2
#define OFFS_PARAM_DAT	6
///++++++++++++++++++++++++++++++++++++++++++++++++
#define	MIN_TT			0
#define	MAX_TT			7
#define	MIN_TV			0
#define	MAX_TV			15

#define QUINT8_MAX		255
#define K_TIME		0.02f




class Params
{

public:
	Params();
	~Params();

	static quint8 calc_step(quint16 beg_r, quint16 end_r);
	static quint16 calc_endr(quint16 beg_r, quint8 step);

	static int calc_val_pow_Index(unsigned char val);
	static quint8 calc_val_pow(int Index);


	static void SetDefaultPar(saved_par_contr_t &par_contr);

};




#endif /// _PARAMS_H_
