#ifndef _DEV_INTERF_H_
#define _DEV_INTERF_H_
#include "params.h"
///=====================================================

#define UDP_MAX_FR_LEN 1000
#define MAX_ALT_DATA_LEN 64
struct sys_par_t
{
	quint8         ip_addr[16];
	quint16        num_dev;				///
	quint16	curr_nparam;				///
	quint8         type_conn;      ///0-bt ,1-wifi  
	quint8         aws_on;      ///0-off ,1-on  
	quint8         dgs_on;      ///0-off ,1-on  
};


struct adc_spi_req_t
{
	quint8 addr;
	quint8 resv;///nbytes;
	quint16 data;///[MAX_SPI_ADC_LEN];
};

struct xil_dat_req_t
{
	quint16 addr;
	quint16 nbytes;
	quint16 data[MAX_ALT_DATA_LEN];
};
struct req_cmd_t
{
	quint32 cmd;
	xil_dat_req_t req;
};


struct cpu_dat_req_t
{
	quint32 addr;
	quint32 nbytes;
	quint32 *data;///[MAX_SPI_ADC_LEN];
};

struct strob_struct_t
{
	quint16 ampl;
	quint16 time;
};

struct ehogram_t
{
	strob_struct_t strob_chan[NUM_STRBS];
	quint8 ehogram[LEN_OSC];
	quint8 batarey;
};


struct device_data_t
{
	adc_spi_req_t g_adc_spi_req;
	adc_spi_req_t g_adc_spi_dat;
	xil_dat_req_t g_alt_dat_req;
	xil_dat_req_t g_alt_dat;
	cpu_dat_req_t g_cpu_dat_req;
};

#define TYPE_NO_ARC		0
#define TYPE_ARC_MLZO	1
#define TYPE_ARC_LZMA	2
#define ERROR_NUM_ZAMER	0xffffffff


#define KEY_E_SIZE						5
#define KEY_P_SIZE						128
#define KEY_Q_SIZE						128

struct device_key_data
{
	char key_e[KEY_E_SIZE+1];
	char key_p[KEY_P_SIZE+1];
	quint8 alg_id;
	quint8 key_type;
	char key_q[KEY_Q_SIZE+1];
};


#define VERSION_AWS_FLAG				(0x1 << 1)
#define VERSION_DGS_FLAG				(0x1 << 2)


#define REQ_GET_OSC						0x2c
#define REQ_GET_THICK					0x2d
#define REQ_GET_OSC_ARD					0x2e

#define REQ_GET_DEVICE_ID				0x30

#define REQ_PUT_PAR						0x32
#define REQ_GET_PAR						0x33
#define REQ_GET_DEVICE_INFO				0x3A
#define REQ_PUT_DEVICE_ID				0x3B


/*
#define REQ_SET_OPEN_FILE				0x80
#define REQ_SET_CLOSE_FILE				0x81
#define REQ_GET_RD_FILE					0x82
#define REQ_SET_WR_FILE					0x83
#define REQ_GET_FILE_STAT				0x84
#define REQ_SET_FILE_SEEK				0x85
#define REQ_SET_FILE_NAME				0x86
#define REQ_GET_FILE_ERROR				0x87		// получение последней команды с ошибкой
*/

// флаги открытия фала в приборе
/*
#define	FILE_FA_READ				0x01
#define	FILE_FA_OPEN_EXISTING		0x00

#define	FILE_FA_WRITE				0x02
#define	FILE_FA_CREATE_NEW			0x04
#define	FILE_FA_CREATE_ALWAYS		0x08
#define	FILE_FA_OPEN_ALWAYS			0x10
*/

// для использования в comm_mode, не передаётся в прибор

/*
#define FILE_OPEN_FILE_READ				0x75	
#define FILE_OPEN_FILE_WRIIE			0x76


#define FILE_SEEK_FILE_READ				0x77	// возврат к последнему правильному написанному участку
#define FILE_SEEK_FILE_WRITE			0x78



#define FILE_ERROR_OK					0x0
#define FILE_ERROR_OPEN					0x1
#define FILE_ERROR_WR					0x2
#define FILE_ERROR_RD					0x3
#define FILE_ERROR_SEEK					0x4
#define FILE_ERROR_STAT					0x5

#define FILE_MAX_LEN_NAME				256
#define FILE_MIN_LEN_NAME				3
#define FILE_MAX_LEN_CHUNK				1024

#define FILE_USUAL_LEN_CHUNK			512	// обычно будем считывать/записывать по 512 байт
*/



///============================================
#define NUM_WR_DB_TB_SPIFFS				0x88
#define NUM_WR_DB_TB_EEPROM				0x89

#define NUM_SEND_PSPI_DAT		0x8F
///#define NUM_SEND_SPI_ADC_DAT	0x90
///#define NUM_REQ_SPI_ADC_DAT		0x91
///#define NUM_SEND_SPI_ADC_REQ	0x92
#define NUM_SEND_EX_PLL_DAT		0x90
#define NUM_REQ_EX_PLL_DAT		0x91
#define NUM_SEND_EX_PLL_REQ		0x92

#define NUM_SEND_FPGA_DAT		0x93
#define NUM_REQ_FPGA_DAT		0x94
#define NUM_SEND_FPGA_REQ		0x95

#define NUM_SET_EX_PLL_FRQ		0x96


///#define NUM_SEND_MOD_STR		0x96
///#define NUM_REQ_MOD_STR			0x97
/// 
#define NUM_SEND_EEPROM_DAT		0x97
#define NUM_REQ_EEPROM_DAT		0x98
#define NUM_SEND_EEPROM_REQ		0x99


#define NUM_SEND_UDB_DAT		0xA7
#define NUM_REQ_UDB_DAT			0xA8
#define NUM_SEND_UDB_REQ		0xA9

///+++++++++++++++++++++++++++++++++
#define REQ_XIL_DAT	(0x1<<0)
#define GET_XIL_DAT	(0x1<<1)
#define PUT_XIL_DAT	(0x1<<2)
///=====================================================


#endif
