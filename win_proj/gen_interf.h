#ifndef _GEN_INTERF_H_
#define _GEN_INTERF_H_

#include "params.h"

#define CMD_PUT_DAT  				0x3
#define CMD_REQ_DAT  				0x4
#define CMD_PUT_REQ_DAT  		    0x5
///#define CMD_PUT_REQ_DAT_ID  		0x1
///==========  from dev ====================
#define CMD_ACK  					0x85

#define CMD_SUBM_PUT				0x85
#define CMD_SUBM_REQ	  			0x5	

#define NUM_REQ_STAT			    0x8		///

#define NUM_REQ_REJ						0x9
#define NUM_SET_REJ						0xA
#define NUM_REQ_GET_VERS				0x2e

#define REQ_PUT_PAR						0x32

#define MAX_XIL_DATA_LEN		32


#define CHNG_ALL		(0x1<<31)
/*
struct dac_spi_req_t
{
	quint8 addr;
	quint8 resv;///nbytes;
	quint16 data;///[MAX_SPI_ADC_LEN];
};
*/
struct xil_dat_req_t
{
	quint16 addr;
	quint16 nbytes;
	quint16 data[MAX_XIL_DATA_LEN];
};


struct cpu_dat_req_t
{
	quint32 addr;
	quint32 nbytes;
	quint32 *data;///[MAX_SPI_ADC_LEN];
};

///#define NUM_SEND_PSPI_DAT		0x8F
#define NUM_SEND_SPI_ADC_DAT	0x90
#define NUM_REQ_SPI_ADC_DAT		0x91
#define NUM_SEND_SPI_ADC_REQ	0x92

#define NUM_SEND_XIL_DAT		0x93
#define NUM_REQ_XIL_DAT			0x94
#define NUM_SEND_XIL_REQ		0x95

////#define NUM_SEND_MOD_STR		0x96
///#define NUM_REQ_MOD_STR			0x97

///#define NUM_SEND_ACT_DAT		0xA0
///#define NUM_REQ_ACT_DAT			0xA1
////#define NUM_SEND_ACT_REQ		0xA2
#define NUM_SEND_SPI_DAC_DAT	0x98
#define NUM_REQ_SPI_DAC_DAT		0x99
#define NUM_SEND_SPI_DAC_REQ	0x9A

/*
#define REQ_XIL_DAT				(0x1<<0)
#define GET_XIL_DAT				(0x1<<1)
#define PUT_XIL_DAT				(0x1<<2)
*/

#endif
