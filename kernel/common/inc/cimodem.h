#ifndef __CI_MODEM_H__
#define __CI_MODEM_H__
#include <stdint.h>
#include <global_types.h>
#include "teldef.h"

#define MODEM_STATUS_AT			0
#define MODEM_STATUS_PS_DATA	1
#define MODEM_STATUS_CS_DATA	2

#define MODEM_STUB_MESSAGE_Q_SIZE 	16
#define MODEM_STUB_MESSAGE_Q_MAX	256
#define MODEM_STUB_STACK_SIZE		8192
#define MODEM_STUB_TASK_PRIORITY	70

#define SHM_HEADER_SIZE		sizeof(ShmApiMsg)
#define MODEMDATASTUB 		31


#define ModemDataStubRequestStartProcId			0x07
#define ModemDataStubReqDataProcId                 0x08
#define ModemDataStubIndDataProcId					0x08

typedef enum _CiChannelStatus{
	CI_CHANNEL_CS,
	CI_CHANNEL_PSD,
	CI_CHANNEL_CSD,
}CiChannelStatus;

typedef enum _ModemChannelState{
	Modem_AT_State,
	Modem_DATA_State,
}ModemChannelState;

void CiModemInit(void);
void CiModemTx(void* data, UINT32 len,UINT32 index);
void ci_modem_atm_usb_tx(UINT8 * data, UINT32 length, UINT8 atp);
void ci_modem_PS_data_tx(void* data, UINT32 len,UINT32 Aptindex);
void ci_modem_CS_data_tx(void* data, UINT32 len);

void ci_modem_init(void);

void ci_modem_set_cid(uint32_t cid,uint32_t Apt_index);

void ci_modem_set_status(uint8_t status,uint32_t Apt_index);
void  ci_modem_CSD_set_cid(uint32_t cid);
void ci_modem_CSD_set_status(uint8_t status);

uint8_t ci_modem_get_status(unsigned Apt_index);
unsigned long long ci_modem_get_ind_apt_index(void);
uint8_t ci_modem_get_default_channel_index(void);
int ci_insert_ps_buffer_to_Mux_Managment(unsigned int aptindex,char *buffer,unsigned int length);

/***********************************************************************
 *
 * Name:		IsAtCmdSrvReady
 *
 * Description: check whether at command server is ready to handle at commands
 *
 * Parameters: None
 *
 * Returns:
 *		TRUE: Ready
 *		FALSE: Not ready
 *
 * Notes:
 *
 ***********************************************************************/
BOOL IsAtCmdSrvReady(void);


/***********************************************************************
 *
 * Name:		ci_modem_set_ind_apt_index
 *
 * Description: enable a channle to receive AT indication, not supported in MMI
 *
 * Parameters: 
 *	TelAtParserID		AtpIndex		 [IN]	channle index
 *
 * Returns: None
 *
 * Notes:
 *
 ***********************************************************************/
void ci_modem_set_ind_apt_index(TelAtParserID index);



/***********************************************************************
 *
 * Name:		ci_modem_clear_ind_apt_index
 *
 * Description: enable a channle to receive indication, not supported in MMI
 *
 * Parameters: 
 *	TelAtParserID		AtpIndex		 [IN]	channle index
 *
 * Returns: None
 *
 * Notes:
 *
 ***********************************************************************/
void ci_modem_clear_ind_apt_index(TelAtParserID index);


/***********************************************************************
 *
 * Name:		ci_modem_get_ind_apt_index
 *
 * Description: get the channel 
 *
 * Parameters: 
 *	
 *
 * Returns: None
 *	channel index mapping info
 *
 * Notes:
 *
 ***********************************************************************/
unsigned long long ci_modem_get_ind_apt_index(void);

//fix build warning
UINT8 get_dlc_num(UINT8 index);
#endif
