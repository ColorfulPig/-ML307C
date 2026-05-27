/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

/*********************************************************************
*							INTEL Software
* --------------------------------------------------------------------
* Filename: 	RspDigRf3G.h
*
* Created By:  	Terry Yao, 8, Feb, 2006
*
* Programmers: 	
*
* Module:  		Generic RF Module
*

* Notes:
*
***********************************************************************/
#ifndef _RSP_DIG_RF3G_H_
#define _RSP_DIG_RF3G_H_

/***************************************************************
 * Global Functions
 ***************************************************************/

//#include <L1dspfnc.h>

#include <GenericRfDriver.h>
#include "nvm_header.h"
#include "DigRf3GPor_common.h"


#define RSP_POWER_UP_ADVANCE									0

#define RSP_POWER_UP_ASYNC_ADVANCE_SKYLARK						0
#define RSP_POWER_DOWN_ADVANCE_SKYLARK							0
#define RSP_WAKEUP_ADVANCE_SKYLARK 								0
#define RSP_TX_CONTIGDOWN_ADVANCE_SKYLARK 						0
#define RSP_TX_CONTIGDOWNSHORT_ADVANCE_SKYLARK 					0
#define RSP_TX_CONTIGUP_ADVANCE_SKYLARK 						0
#define RSP_TX_CONTIGUPSHORT_ADVANCE_SKYLARK 					0
#define RSP_TX_TO_IDLE_ADVANCE_SKYLARK 							0
#define RSP_TX_TO_IDLE_DELAY_SKYLARK 							20		// added by lihui, 2011-03-07, for change RDI/TDI speed
#define RSP_UPLOAD_TXRAMP_ADVANDE_SKYLARK						0

#define RSP_SET_AFC_ADVANDE_SKYLARK								0
#define RSP_SET_AFC_ASYNC_ADVANDE_SKYLARK						0


#define RSP_SET_AGC_ADVANDE_SKYLARK								0
#define RSP_DO_SNIFF_ADVANDE_SKYLARK							129


#define RSP_RX_MARGIN_FOR_DSDS                          		30        // add 30q Margin 

//#define RSP_IDLE_TO_MONRX_ADVANDE_SKYLARK						(295)// change from 220
#define RSP_IDLE_TO_MONRX_ADVANDE_SKYLARK						(265)// jpzhang debug for PTM Monitor schedule

#define RSP_IDLE_TO_MONRX_DELTA_TIME1_SKYLARK					(2)
#define RSP_IDLE_TO_MONRX_DELTA_TIME2_SKYLARK					(40)
#define RSP_IDLE_TO_MONRX_DELTA_TIME3_SKYLARK					(90)    // change from 80
// RSP_IDLE_TO_MONRX_ADVANE -(Sum of delta) = Mon burst real advanc 98 qS
#define RSP_IDLE_TO_NORMALRX_DELTA_TIME0_SKYLARK				(62)
#define RSP_IDLE_TO_NORMALRX_DELTA_TIME1_SKYLARK				(2)
#define RSP_IDLE_TO_NORMALRX_DELTA_TIME2_SKYLARK				(40) 
#define RSP_IDLE_TO_NORMALRX_ADVANDE_SKYLARK					(295)   // change from 285
#define RSP_IDLE_TO_NORMALRX_DELTA_TIME3_SKYLARK				(90)    // change from 80


//CRANE TX
#define RSP_IDLE_TO_TX_1_ADVANDE_SKYLARK						(427 - 55)

#define RSP_IDLE_TO_TX_1_TX_SPECIFIC_TIME_SKYLARK				60 
#define RSP_TX_1_TO_TX_2_TX_SPECIFIC_TIME_SKYLARK				100 
#define RSP_TRIG_TX_RF_START_DELTA_TIME                         265 //115+152-2

#define RSP_TRIG_TX_STOP_DELTA_TIME                              20
#define RSP_TX_TO_IDLE_TO_ADVANDE_SKYLARK                        80

#define RSP_IDLE_TO_TX_ADVANDE_ADJUST_GRANEGT                   -4

//CRANE RX
#define RSP_SNIFF_ADVANDE_SKYLARK								(60)	
#define RSP_RX_TO_IDLE_ADVANDE_SKYLARK							(80)      

#define RSP_TRIG_RX_PLL_START_DELTA_TIME_NORM                        110   //152
#define RSP_TRIG_RX_PLL_START_DELTA_TIME_MON                         80    //152
#define RSP_TRIG_RX_RF_ON_DELTA_TIME                                 50    //102
#define RSP_TRIG_R2G_DIG_START_DELTA_TIME                            2     //100
#define RSP_TRIG_RX_DLY_START_DELTA_TIME                             5     //95    
#define RSP_TRIG_RX_SEQ_2_START_DELTA_TIME                           75    //20
#define RSP_TRIG_RX_GPIO_1_RX_START_DELTA_TIME                       5     //15
#define RSP_TRIG_R2G_DATA_START_DELTA_TIME                           32    //13
#define RSP_TRIG_RX_GPIO_2_START_DELTA_TIME                          3     //10
#define RSP_TRIG_RX_AGC_START_DELTA_TIME                             3     //7

#define RSP_TRIG_RX_GPIO_2_STOP_DELTA_TIME                    10 
#define RSP_TRIG_R2G_DIG_STOP_DELTA_TIME                      3   //15
#define RSP_TRIG_RX_GPIO_1_RX_STOP_DELTA_TIME                 2   //15
#define RSP_TRIG_R2G_DATA_STOP_DELTA_TIME                     3   //20
#define RSP_TRIG_RX_SEQ_2_STOP_DELTA_TIME                     2   //20
#define RSP_TRIG_RX_RF_STOP_DELTA_TIME                        5   //25
#define RSP_TRIG_RX_PLL_STOP_DELTA_TIME                       5   //30

#define RSP_TRIG_RX_AFC_DELTA_TIME                            5
#define RSP_TRIG_RX_SEQ_3_START_DELTA_TIME                    0x7fff
#define RSP_TRIG_RX_SEQ_3_STOP_DELTA_TIME                     0x7fff

#define TRIG_RX_RF        0
#define TRIG_RX_RF_STOP   1
#define TRIG_RX_PLL       2
#define TRIG_RX_AGC       3
#define TRIG_SEQ_2        4
#define TRIG_SEQ_3        5  
#define TRIG_GPIOWRITE_2  6
#define TRIG_GPIOWRITE_1_RX  7
#define TRIG_RX_AFC       8
#define TRIG_RX_RF_DLY    9
#define TRIG_R2G_DIG      10
#define TRIG_R2G_DATA     11

#define CRANE_TRIG_DURATION                                 0x0001


#define	CF_TX_PREAMBLE_FIRSTBURST_BITS      				0    

#define	RX_ADC_FULL_SCALE_FACTOR							0x46

#define APPROX_QBIT_WB_SYM							73

#define WB_MEAS_IDLE_TO_RX_FW_LATENCY_WB_SYM		(4)
#define WB_MEAS_IDLE_TO_RX_SEQ_DURATION_WB_SYM		(3)
#define	WB_MEAS_IDLE_TO_RX_WB_SYM					(WB_MEAS_IDLE_TO_RX_SEQ_DURATION_WB_SYM + WB_MEAS_IDLE_TO_RX_FW_LATENCY_WB_SYM)
  
#define	WB_MEAS_AGC_SETTLE_WB_SYM					(8)
  
#define WB_MEAS_RX_TO_IDLE_FW_LATENCY_WB_SYM		(0)
#define WB_MEAS_RX_TO_IDLE_SEQ_DURATION_WB_SYM		(3)
#define	WB_MEAS_RX_TO_IDLE_WB_SYM                   (WB_MEAS_RX_TO_IDLE_FW_LATENCY_WB_SYM + WB_MEAS_RX_TO_IDLE_SEQ_DURATION_WB_SYM)
  
#define	WB_MEAS_IDLE_TO_RX			(WB_MEAS_IDLE_TO_RX_WB_SYM * APPROX_QBIT_WB_SYM)
#define	WB_MEAS_AGC_SETTLE      	(WB_MEAS_AGC_SETTLE_WB_SYM * APPROX_QBIT_WB_SYM)
#define	WB_MEAS_RX_TO_IDLE      	(WB_MEAS_RX_TO_IDLE_WB_SYM * APPROX_QBIT_WB_SYM)


#define RF_IF_GSM_TX_RAMPING_PARAMS     					0
#define RF_IF_DCXO_COARS_PARAMS     						1
#define RF_IF_EXT_TEMP_SENSOR_PARAMS     					2

#define RF_RX_START_ANALOG_ADVANCE_CFG_VAL					230

typedef enum
{
        TRIG_TX_RF_GPO0         =0,
        TRIG_T2G_DIG_GPO1       =1,
        TRIG_TX_PLL_GPO2        =2,
        TRIG_TX_AGC_GP03        =3,
        TRIG_SEQ_0_GP04         =4,
        TRIG_SEQ_1_GPO5         =5,
        TRIG_GPIOWRITE_0_GP06   =6,
        TRIG_GPIOWRITE_1_TX_GP07=7,
        TRIG_TX_AFC_GP08        =8,
        TRIG_APTDAC_GP09        =9,
        TRIG_PAEN_GP010         =10,
        TRIG_VMODE_GP011        =11,
        TRIG_DCDCEN_GP012       =12,
        TRIG_TX_RF_DLY_GP013    =13,
        NUM_OF_TRIGGER          =14  
}GSMTX_TCU_PORT;

typedef struct
{
    GSMTX_TCU_PORT    GSMTx_trigger_port;
    INT16             Gsm_trig_time_qB; 
	INT16             Gsm_delta_trig_time_qB;
}GSMTX_CTRL_SIG;

typedef struct
{
     INT  NumofSignal;
     INT8 StartTsgNum;	 
	 INT8 StopTsgNum;	 
	 GSMTX_CTRL_SIG  GsmTx_Sigs[NUM_OF_TRIGGER];    
}GSMTX_TCU_CTRL;


typedef union{
	struct {
		    INT16         gsm_txpll_trig_2_start_qB;                                 
            INT16         gsm_txrf_trig_2_start_qB;                                  
            INT16         gsm_txrfdly_trig_2_start_qB;                               
            INT16         gsm_txrf_trig_2_stop_qB;                                   
            INT16         gsm_txrfdly_trig_2_stop_qB;                                
            INT16         gsm_txpll_trig_2_stop_qB;                                  
            INT16         gsm_t2g_dig_trig_2_gmsk_nru_qB;                            
            INT16         gsm_t2g_dig_trig_2_edge_nru_qB;
		    INT16         gsm_t2g_dig_trig_2_gmsk2gmsk_mru_qB;                                                           
            INT16         gsm_t2g_dig_trig_2_edge2edge_mru_qB;     
            INT16         gsm_seq0_trig_2_gmsk_nru_qB[2];                               
            INT16         gsm_seq1_trig_2_gmsk_nru_qB[2];                               
            INT16         gsm_gpio0_trig_2_gmsk_nru_qB[2];                              
            INT16         gsm_gpio1tx_trig_2_gmsk_nru_qB[2];                            
            INT16         gsm_paen_trig_2_gmsk_nru_qB[2];                               
            INT16         gsm_vmode_trig_2_gmsk_nru_qB[2];                              
            INT16         gsm_seq0_trig_2_edge_nru_qB[2];                               
            INT16         gsm_seq1_trig_2_edge_nru_qB[2];                               
            INT16         gsm_gpio0_trig_2_edge_nru_qB[2];                              
            INT16         gsm_gpio1tx_trig_2_edge_nru_qB[2];                            
            INT16         gsm_paen_trig_2_edge_nru_qB[2];                               
            INT16         gsm_vmode_trig_2_edge_nru_qB[2];                              
            INT16         gsm_seq0_trig_2_gmsk_nrd_qB[2];                               
            INT16         gsm_seq1_trig_2_gmsk_nrd_qB[2];                               
            INT16         gsm_gpio0_trig_2_gmsk_nrd_qB[2];                              
            INT16         gsm_gpio1tx_trig_2_gmsk_nrd_qB[2];                            
            INT16         gsm_paen_trig_2_gmsk_nrd_qB[2];                               
            INT16         gsm_vmode_trig_2_gmsk_nrd_qB[2];                              
            INT16         gsm_seq0_trig_2_edge_nrd_qB[2];                               
            INT16         gsm_seq1_trig_2_edge_nrd_qB[2];                               
            INT16         gsm_gpio0_trig_2_edge_nrd_qB[2];                              
            INT16         gsm_gpio1tx_trig_2_edge_nrd_qB[2];                            
            INT16         gsm_paen_trig_2_edge_nrd_qB[2];                               
            INT16         gsm_vmode_trig_2_edge_nrd_qB[2];                              
            INT16         gsm_txagc_trig_2_gmsk_mru_qB[2];                              
            INT16         gsm_txagc_trig_2_edge_mru_qB[2];                              
            INT16         gsm_seq0_trig_2_gmsk_mru_qB[2];                               
            INT16         gsm_seq1_trig_2_gmsk_mru_qB[2];                               
            INT16         gsm_gpio0_trig_2_gmsk_mru_qB[2];                              
            INT16         gsm_gpio1tx_trig_2_gmsk_mru_qB[2];                            
            INT16         gsm_paen_trig_2_gmsk_mru_qB[2];                               
            INT16         gsm_vmode_trig_2_gmsk_mru_qB[2];                              
            INT16         gsm_seq0_trig_2_edge_mru_qB[2];                               
            INT16         gsm_seq1_trig_2_edge_mru_qB[2];                               
            INT16         gsm_gpio0_trig_2_edge_mru_qB[2];                              
            INT16         gsm_gpio1tx_trig_2_edge_mru_qB[2];                            
            INT16         gsm_paen_trig_2_edge_mru_qB[2];                               
            INT16         gsm_vmode_trig_2_edge_mru_qB[2];                              
			INT16         gsm_rxpll_trig_2_start_qB;                                 
            INT16         gsm_rxrf_trig_2_start_qB;                                  
            INT16         gsm_rxrfdly_trig_2_start_qB;                               
            INT16         gsm_r2g_dig_trig_2_start_qB;                               
            INT16         gsm_r2g_data_trig_2_start_qB;                              
            INT16         gsm_r2g_dig_trig_2_stop_qB;                                
            INT16         gsm_r2g_data_trig_2_stop_qB;                               
            INT16         gsm_gpio2_trig_2_start_qB;                                 
            INT16         gsm_gpio1rx_trig_2_start_qB;                               
            INT16         gsm_seq2_trig_2_start_qB;                                  
            INT16         gsm_seq3_trig_2_start_qB;                                  
            INT16         gsm_rxpll_trig_2_stop_qB;                                  
            INT16         gsm_rxrf_trig_2_stop_qB;                                   
            INT16         gsm_rxrfdly_trig_2_stop_qB;                                
            INT16         gsm_seq2_trig_2_stop_qB;                                   
            INT16         gsm_seq3_trig_2_stop_qB;                                   
            INT16         gsm_gpio1rx_trig_2_stop_qB;                                
            INT16         gsm_gpio2_trig_2_stop_qB;                                  
            INT16         gsm_rxagc_trig_2_gain_chng_qB;                             
            INT16         gsm_txagc_trig_2_gain_chng_qB;                             
            INT16         gsm_rxafc_trig_delay_qB;                                   
            INT16         gsm_txafc_trig_delay_qB;                                   
            INT16         gsm_aptdac_trig_2_start_qB;                                
            INT16         gsm_ddcen_trig_2_start_qB;                                 
            INT16         gsm_aptdac_trig_2_stop_qB;                                 
            INT16         gsm_ddcen_trig_2_stop_qB;    //112
			INT16         gsm_t2g_dig_trig_2_gmsk2edge_mru_qB;                                                           
            INT16         gsm_t2g_dig_trig_2_edge2gmsk_mru_qB;   
	}gsm_trig;

	INT16 tsg[114];            /* max size*/
}GSM_TRIGGER_TIME;
#if !defined (ENABLE_CAT1_LG)   //crane_LG

//ICAT EXPORTED STRUCT
typedef struct CalFileVersionFlagTag
{
	UINT16 calFileVersion;
}CalFileVersionFlag_ts;

//ICAT EXPORTED STRUCT
typedef struct RF_Config_returnSkylarkTag
{
	UINT16 band_combination_version;
}RF_Config_returnSkylark_ts;

//ICAT EXPORTED STRUCT
typedef struct 
{
	UINT16 	plRfDcxoIsUsingFlg;
}plRfDcxoIsUsingFlg_ts;

#endif

/*********************************************************************
 SKYLARK 2G RX START
*********************************************************************/
//word0
#define SKYLARK_2G_RX_START_HEADER					   		0xFFC6

//word1
#define SKYLARK_2G_RX_START_DL_BIT_SHIFT					15
#define SKYLARK_2G_RX_START_DL_BIT_MASK						0x1
#define SKYLARK_2G_RX_START_PRI_RX_BIT_SHIFT				14
#define SKYLARK_2G_RX_START_PRI_RX_BIT_MASK					0x1
#define SKYLARK_2G_RX_START_DIV_RX_BIT_SHIFT				13
#define SKYLARK_2G_RX_START_DIV_RX_BIT_MASK					0x1
#define SKYLARK_2G_RX_START_RX_LEVEL_BIT_SHIFT				4
#define SKYLARK_2G_RX_START_RX_LEVEL_BIT_MASK				0xFF       //8bit
#define SKYLARK_2G_RX_START_MX_BIT_SHIFT				    3
#define SKYLARK_2G_RX_START_MX_BIT_MASK				        0x1       //1bit
#define SKYLARK_2G_RX_START_GSM_BAND_BIT_SHIFT				0
#define SKYLARK_2G_RX_START_GSM_BAND_BIT_MASK				0x7       //3bit



//word2
#define SKYLARK_2G_RX_START_FILT_SET_BIT_SHIFT				14
#define SKYLARK_2G_RX_START_FILT_SET_BIT_MASK				0x3      //2bit
#define SKYLARK_2G_RX_START_LO_INJ_BIT_SHIFT				13
#define SKYLARK_2G_RX_START_LO_INJ_BIT_MASK					0x1      //1bit
#define SKYLARK_2G_RX_START_ARFCN_BIT_SHIFT					0
#define SKYLARK_2G_RX_START_ARFCN_BIT_MASK					0x3FF      //10bit


// word3
#define SKYLARK_2G_RX_START_AFC_BIT_SHIFT					0
#define SKYLARK_2G_RX_START_AFC_BIT_MASK					0x7FFF      //15bit





/*********************************************************************
 SP8 2G RX STOP
*********************************************************************/
//word0
/*********************************************************************
 SKYLARK 2G RX STOP
*********************************************************************/
//word0
#define SKYLARK_2G_RX_STOP_HEADER					   		0xFFC7
//word1
#define SKYLARK_2G_RX_STOP_DL_BIT_SHIFT						15
#define SKYLARK_2G_RX_STOP_DL_BIT_MASK						0x1
#define SKYLARK_2G_RX_STOP_PRI_RX_BIT_SHIFT					14
#define SKYLARK_2G_RX_STOP_PRI_RX_BIT_MASK					0x1
#define SKYLARK_2G_RX_STOP_DIV_RX_BIT_SHIFT					13
#define SKYLARK_2G_RX_STOP_DIV_RX_BIT_MASK					0x1




/*********************************************************************
// SKYLARK 2G TX START
*********************************************************************/
//word0
#define SKYLARK_2G_TX_START_HEADER					   		0xFFC2              // for power use in dBm

//word1
#define SKYLARK_2G_TX_START_DL_BIT_SHIFT					15
#define SKYLARK_2G_TX_START_DL_BIT_MASK						0x1
#define SKYLARK_2G_TX_TEMP_RANGE_BIT_SHIFT					12
#define SKYLARK_2G_TX_TEMP_RANGE_BIT_MASK					0x3
#define SKYLARK_2G_TX_TIMING_ADVANCE_BIT_SHIFT				0
#define SKYLARK_2G_TX_TIMING_ADVANCE_BIT_MASK				0xFF		//8bit
#define SKYLARK_2G_TX_START_GSM_BAND_BIT_SHIFT				10
#define SKYLARK_2G_TX_START_GSM_BAND_BIT_MASK				0x7       //3bit

//word2
#define SKYLARK_2G_TX_START_NUM_OF_BURST_BIT_SHIFT			8
#define SKYLARK_2G_TX_START_NUM_OF_BURST_BIT_MASK			0x7		//3bit
#define SKYLARK_2G_TX_START_ARFCN_BIT_SHIFT					0
#define SKYLARK_2G_TX_START_ARFCN_BIT_MASK					0x3FF      //10bit

//word3
#define SKYLARK_2G_TX_START_SLOT1_BURST_TYPE_BIT_SHIFT		15
#define SKYLARK_2G_TX_START_SLOT1_BURST_TYPE_BIT_MASK		0x1
#define SKYLARK_2G_TX_START_SLOT1_BURST_MOD_BIT_SHIFT		14
#define SKYLARK_2G_TX_START_SLOT1_BURST_MOD_BIT_MASK		0x1
#define SKYLARK_2G_TX_START_SLOT1_TARGET_POWER_BIT_SHIFT	0
#define SKYLARK_2G_TX_START_SLOT1_TARGET_POWER_BIT_MASK		0x3FFF

//word4
#define SKYLARK_2G_TX_START_SLOT2_BURST_TYPE_BIT_SHIFT		15
#define SKYLARK_2G_TX_START_SLOT2_BURST_TYPE_BIT_MASK		0x1
#define SKYLARK_2G_TX_START_SLOT2_BURST_MOD_BIT_SHIFT		14
#define SKYLARK_2G_TX_START_SLOT2_BURST_MOD_BIT_MASK		0x1
#define SKYLARK_2G_TX_START_SLOT2_TARGET_POWER_BIT_SHIFT	0
#define SKYLARK_2G_TX_START_SLOT2_TARGET_POWER_BIT_MASK		0x3FFF

//word5
#define SKYLARK_2G_TX_START_SLOT3_BURST_TYPE_BIT_SHIFT		15
#define SKYLARK_2G_TX_START_SLOT3_BURST_TYPE_BIT_MASK		0x1
#define SKYLARK_2G_TX_START_SLOT3_BURST_MOD_BIT_SHIFT		14
#define SKYLARK_2G_TX_START_SLOT3_BURST_MOD_BIT_MASK		0x1
#define SKYLARK_2G_TX_START_SLOT3_TARGET_POWER_BIT_SHIFT	0
#define SKYLARK_2G_TX_START_SLOT3_TARGET_POWER_BIT_MASK		0x3FFF

//word6
#define SKYLARK_2G_TX_START_SLOT4_BURST_TYPE_BIT_SHIFT		15
#define SKYLARK_2G_TX_START_SLOT4_BURST_TYPE_BIT_MASK		0x1
#define SKYLARK_2G_TX_START_SLOT4_BURST_MOD_BIT_SHIFT		14
#define SKYLARK_2G_TX_START_SLOT4_BURST_MOD_BIT_MASK		0x1
#define SKYLARK_2G_TX_START_SLOT4_TARGET_POWER_BIT_SHIFT	0
#define SKYLARK_2G_TX_START_SLOT4_TARGET_POWER_BIT_MASK		0x3FFF


//word7
#define SKYLARK_2G_TX_START_SLOT5_BURST_TYPE_BIT_SHIFT		15
#define SKYLARK_2G_TX_START_SLOT5_BURST_TYPE_BIT_MASK		0x1
#define SKYLARK_2G_TX_START_SLOT5_BURST_MOD_BIT_SHIFT		14
#define SKYLARK_2G_TX_START_SLOT5_BURST_MOD_BIT_MASK		0x1
#define SKYLARK_2G_TX_START_SLOT5_TARGET_POWER_BIT_SHIFT	0
#define SKYLARK_2G_TX_START_SLOT5_TARGET_POWER_BIT_MASK		0x3FFF


//word8
#define SKYLARK_2G_TX_START_SLOT6_BURST_TYPE_BIT_SHIFT		15
#define SKYLARK_2G_TX_START_SLOT6_BURST_TYPE_BIT_MASK		0x1
#define SKYLARK_2G_TX_START_SLOT6_BURST_MOD_BIT_SHIFT		14
#define SKYLARK_2G_TX_START_SLOT6_BURST_MOD_BIT_MASK		0x1
#define SKYLARK_2G_TX_START_SLOT6_TARGET_POWER_BIT_SHIFT	0
#define SKYLARK_2G_TX_START_SLOT6_TARGET_POWER_BIT_MASK		0x3FFF
#define SKYLARK_2G_TX_START_SLOT1_LPI_BIT_SHIFT				15
#define SKYLARK_2G_TX_START_SLOT1_LPI_BIT_MASK				0x1
#define SKYLARK_2G_TX_START_SLOT2_LPI_BIT_SHIFT				14
#define SKYLARK_2G_TX_START_SLOT2_LPI_BIT_MASK				0x1
#define SKYLARK_2G_TX_START_SLOT3_LPI_BIT_SHIFT				13
#define SKYLARK_2G_TX_START_SLOT3_LPI_BIT_MASK				0x1
#define SKYLARK_2G_TX_START_SLOT4_LPI_BIT_SHIFT				12
#define SKYLARK_2G_TX_START_SLOT4_LPI_BIT_MASK				0x1


//word9
#define SKYLARK_2G_TX_START_AFC_BIT_SHIFT					0
#define SKYLARK_2G_TX_START_AFC_BIT_MASK					0x7FFF      //15bit

#define SKYLARK_2G_TX_SLOT1_EEDGE_MODE_TYPE_BIT_MASK		0x7		
#define SKYLARK_2G_TX_SLOT2_EEDGE_MODE_TYPE_BIT_MASK		0x7	
#define SKYLARK_2G_TX_SLOT3_EEDGE_MODE_TYPE_BIT_MASK		0x7	
#define SKYLARK_2G_TX_SLOT4_EEDGE_MODE_TYPE_BIT_MASK		0x7	
#define SKYLARK_2G_TX_SLOT5_EEDGE_MODE_TYPE_BIT_MASK		0x7	
#define SKYLARK_2G_TX_SLOT6_EEDGE_MODE_TYPE_BIT_MASK		0x7	

#define SKYLARK_2G_TX_SLOT1_EEDGE_MODE_TYPE_BIT_SHIFT		0
#define SKYLARK_2G_TX_SLOT2_EEDGE_MODE_TYPE_BIT_SHIFT		4
#define SKYLARK_2G_TX_SLOT3_EEDGE_MODE_TYPE_BIT_SHIFT		8
#define SKYLARK_2G_TX_SLOT4_EEDGE_MODE_TYPE_BIT_SHIFT		12
#define SKYLARK_2G_TX_SLOT5_EEDGE_MODE_TYPE_BIT_SHIFT		0
#define SKYLARK_2G_TX_SLOT6_EEDGE_MODE_TYPE_BIT_SHIFT		4



/*********************************************************************
Skylark 2G AGC
*********************************************************************/
	//word0
#define SKYLARK_2G_AGC_HEADER					   			0xFFDB
	
	//word1
#define SKYLARK_2G_AGC_DL_BIT_SHIFT							15
#define SKYLARK_2G_AGC_DL_BIT_MASK							0x1
#define SKYLARK_2G_AGC_PRI_RX_BIT_SHIFT						14
#define SKYLARK_2G_AGC_PRI_RX_BIT_MASK						0x1
#define SKYLARK_2G_AGC_DIV_RX_BIT_SHIFT						13
#define SKYLARK_2G_AGC_DIV_RX_BIT_MASK						0x1
#define SKYLARK_2G_AGC_RX_LEVEL_BIT_SHIFT					4
#define SKYLARK_2G_AGC_RX_LEVEL_BIT_MASK					0xFF       //8bit rx level



/*********************************************************************
SKYLARK TAS duration
*********************************************************************/
#define SKYLARK_RX_START_TAS_DURATION				   		0x0001
#define SKYLARK_RX_STOP_TAS_DURATION				   		0x0001
#define SKYLARK_TX_START_TAS_DURATION				   		0x0001


/*********************************************************************
SKYLARK TAS Number
*********************************************************************/
#define SKYLARK_RX_START_TAS_INDEX				   			0x0009
#define SKYLARK_RX_STOP_TAS_INDEX				   			0x0009		// modified by gxsun 100111, to use only 1 TAS source: TAS0 to trig GSM RX/TX action.
#define SKYLARK_TX_START_TAS_INDEX				   			0x0009		// modified by gxsun 100111, to use only 1 TAS source: TAS0 to trig GSM RX/TX action.
//#define SP8_RX_STOP_TAS_INDEX				   				0x0000		// modified by gxsun 100111, to use only 1 TAS source: TAS0 to trig GSM RX/TX action.
//#define SP8_TX_START_TAS_INDEX				   				0x0000		// modified by gxsun 100111, to use only 1 TAS source: TAS0 to trig GSM RX/TX action.
//#endif



// on FPGA ,we only can use TAS0~3 for GSM, and the correspond RF_CONT pin is RF_CONT[2]~RF_CONT[5], 
/* so in GSM ,the TCU GPO index shoulde be TCU_GPOA[0]~TCU_GPOA[3]
RF_IF1	RF_CONT8	SW Control1
RF_IF2	RF_CONT3	SW Control2
RF_IF3	RF_CONT9	SW Control4 (OPT)
RF_IF4	RF_CONT7	SW Control3
RF_IF5	DAC_ST23	HSDPA_STROBE (OPT)
RF_IF6	RF_CONT0	TXEN
RF_IF7	RF_CONT1	WB_PA_EN/PD_EN
RF_IF8	RF_SPI0_CLK	PGC_CLK
RF_IF9	RF_SPI0_STRB0	PGC_EN
RF_IF10	RF_SPI1_STRB0	WB_CEN
RF_IF11	RF_SPI1_D	WB_CDATA
RF_IF12	RF_SPI1_CLK	WB_CCLK
RF_IF13	RF_SPI0_D	PGC_DATA
RF_IF14	RF_CONT2	HSDPA_PA_SW/PA_Mode
RF_IF15	RF_CONT5	HSDPA_VGC_SW
RF_IF16	RF_CONT6	GSM_Strobe
RF_IF17	DSSP1_SCLK	GSM_CCLK
RF_IF18	DSSP1_FRM	GSM_CEN
RF_IF19	DSSP1_RX_TX	GSM_CDATA
RF_IF20	DSSP0_SCLK	RX_TX_CLK
RF_IF21	DSSP0_FRM	RX_TX_EN
RF_IF22	DSSP0_RX_TX	RX_TX_DATA
*/

// num of synths in 32bits ,rounded for DigRF RFICC format in L1C
#define RSP_IDLE_TO_RX_NUM_OF_SYNTHS_SKYLARK					3 //   3
#define RSP_DV4_ATTR_SWITCH_NUM_OF_SYNTHS_SKYLARK     			4
#define RSP_RX_TO_IDLE_NUM_OF_SYNTHS_SKYLARK					1
#define RSP_TX_TO_IDLE_NUM_OF_SYNTHS_SKYLARK					1



#define RSP_IDLE_TO_TX_NUM_OF_SYNTHS					7
#define RSP_IDLE_TO_TX_NUM_OF_SYNTHS_SKYLARK			8

#define RSP_SNIFF_NUM_OF_SYNTHS_SKYLARK							1
#define RSP_SETAFC_NUM_OF_SYNTHS_SKYLARK						2
#define RSP_SETPDRB_NUM_OF_SYNTHS_SKYLARK						3
#define RSP_AGC_NUM_OF_SYNTHS_SKYLARK							1
#define RSP_IFC_NUM_OF_SYNTHS_SKYLARK							1
#define RSP_RX_SPECIFIC_NUM_OF_SYNTHS_SKYLARK					RSP_IDLE_TO_RX_NUM_OF_SYNTHS_SKYLARK
#define RSP_TX_SPECIFIC_NUM_OF_SYNTHS_SKYLARK					RSP_IDLE_TO_TX_NUM_OF_SYNTHS_SKYLARK


#define GSM_RampTiming_LENGTH  			7

#define SKYLARK_REG_LO_LEAKAGE_GSM_TYPE					0
#define SKYLARK_REG_LO_LEAKAGE_WB_TYPE					1

#define SKYLARK_LO_LEAKAGE_REG_LENGTH_GSM  				(13*2)     // one for AGC point and one for DAC code

#define CRANEG_LO_LEAKAGE_REG_LENGTH_WB  				(20)		// one for AGC point and one for DAC cod

extern CalFileVersionFlag_ts gsm_calFile_version;
#define CAL_FILE_VERSION_PTR_GET() ((void*)(&gsm_calFile_version))

extern RF_Config_returnSkylark_ts RFCfg_Skylark;
#define RF_SCHEME_SETTING_PTR_GET_SKYLARK() ((void*)(&RFCfg_Skylark))

//
#define RF_INIT_NUMOFSYNTHS_TELEGRAM_LENGTH_BIT_MASK	0x3FF       //10bit telegram length
#define RF_INIT_NUMOFSYNTHS_TELEGRAM_LENGTH_BIT_SHIFT	0      

#define RF_INIT_NUMOFSYNTHS_RSP_TYPE_BIT_MASK			0xF       //4bit telegram type
#define RF_INIT_NUMOFSYNTHS_RSP_TYPE_BIT_SHIFT			10     	

#define RF_INIT_NUMOFSYNTHS_WRORRD_BIT_MASK				0x1       //1bit write or readback
#define RF_INIT_NUMOFSYNTHS_WRORRD_BIT_SHIFT			15   


#define NUM_RX_FILTER_WORKING_MODE    7              // is same to num of RfRxWorkingMode
//#ifdef L1_DCXO_ENABLED
#if 0
#define DCXO_CALIBRATION_NVM_FILE_NAME		"Dcxo_Calibration.nvm"
#define DCXO_CALIBRATION_NVM_VER			 	"00.00"  
#define DCXO_TEMPCOMPENSATION_NVM_FILE_NAME		"Dcxo_TempCompensation.nvm"
#define DCXO_TEMPCOMPENSATION_NVM_CUSTOMER_FILE_NAME		"Dcxo_TempCompensation_Customer.nvm"
#define DCXO_TEMPCOMPENSATION_NVM_VER			 	"00.00"  
#endif
#define DCXO_CONFIG_NVM_FILE_NAME		"Dcxo_Config_calibration.nvm"
#define DCXO_CFG_NVM_VER			 	"00.00"  

#define DCXO_TEST_RECORD_NVM_FILE_NAME	"Dcxo_Test_Record.nvm"
#define DCXO_TEST_RECORD_CFG_NVM_VER	"00.00"  


#define LYRA_CONFIG_NVM_FILE_NAME		"LyraConfig.nvm"
#define LYRA_CFG_NVM_VER			 	"00.00"  

#define GSM_PA_COMPENSATION_CONFIG_NVM_FILE_NAME	"GSMPACompensationConfig.nvm"
#define GSM_PA_COMPENSATION_CFG_NVM_VER			 "00.00"  

#define WB_PMAX_REDUCTION_CONFIG_NVM_FILE_NAME	"WBPmaxReductionConfig.nvm"
#define WB_PMAX_REDUCTION_CFG_NVM_VER			 "00.00"  

#define GSM_PMAX_REDUCTION_CONFIG_NVM_FILE_NAME	"GSMPmaxReductionConfig.nvm"
#define GSM_PMAX_REDUCTION_CFG_NVM_VER			 "01.00" 

#define RF_LTE_PMAX_REDUCTION_CONFIG_NVM_FILE_NAME	"RspDigRfLtepmaxReductionConfig.nvm" //hwei


#define SKYLARK_LO_LEAKAGE_CONFIG_NVM_FILE_NAME	"SkylarkLOLeakageRegConfig.nvm"
#define SKYLARK_LO_LEAKAGE_CFG_NVM_VER			 "00.00" 

#define CF_NVM_RFPERFORMANCE_FILE_NAME				"RfPerformanceData.nvm"
#define CF_NVM_RFPERFORMANCE_FILE_SKYLARK_VERSION				"0.0"

//ICAT EXPORTED STRUCT
#define NUM_OF_RSP_INPUT_VALUE_LENGTH_SKYLARK                  16

//UINT32 RspInputValuesDigRF[NUM_OF_RSP_INPUT_VALUE_LENGTH];

/*pedef struct RspInputValuesDigRFTag {
	SignedInt16	input[NUM_OF_RSP_INPUT_VALUE_LENGTH];
} RspInputValuesDigRF_ts;
*/


#define 	TX_TIME_ADVANCE_DELTA_SKYLARK                       14 //17  //10(guoxin)	//7  //modified by lihui,2011-02-22, for change RDI/TDI speed
#define		RX_TIME_ADVANCE_DELTA_SKYLARK                       5	//0  //modified by lihui,2011-2-22, for change RDI/TDI speed


//ICAT EXPORTED ENUM
typedef enum {
	LYRA_B12458 = 0,
	NUM_OF_LYRA_BOARD_VERSION
}LYRA_HW_VERSION;



//ICAT EXPORTED ENUM
typedef enum BandCombinationTypeTag{
	LYRA_B12458_M = 0, //
	NUM_OF_BandCombinationType
}BandCombinationType;


//ICAT EXPORTED ENUM
typedef enum RspTypeTag{
	RX_START,
	RX_STOP,
	TX_START,
	AFC,
	PD_RB,
	TX_LOW_2_FAST_RATE_Phase1,	// added by lihui, 2011-2-22, for change RDI/TDI speed
	TX_LOW_2_FAST_RATE_Phase2,
	TX_FAST_2_LOW_RATE,
	RX_SPECIFIC,
	TX_SPECIFIC,	
	TX_STOP,	
	NUM_OF_RSP_TYPE
}RspType;

//ICAT EXPORTED STRUCT
typedef struct Lyra_ConfigNvmTag
{
	BandCombinationType band_combination_version;
} Lyra_ConfigNvm_ts;


//ICAT EXPORTED ENUM
typedef enum SKYLARKGsmBandModeTag{
	EGSM900_SKYLARK,
	DCS1800_SKYLARK,
	GSM850_SKYLARK,
	PCS1900_SKYLARK,
	NUM_OF_GSM_BAND
}SKYLARKGsmBandMode;

//ICAT EXPORTED ENUM
typedef enum SP8GsmHLBandModeTag{
	GSM_L_BAND_SP8,
	GSM_H_BAND_SP8,
	DIM_SIZE_GSM_HL_BAND
}SP8GsmHLBandMode;

//ICAT EXPORTED ENUM
typedef enum SP8PORPhaseTag{
	POR_PHASE_1 = 0,
	POR_PHASE_2,
	NUM_OF_POR_PHASE
}SP8PORPhase;

//ICAT EXPORTED ENUM
typedef enum CalFileVersionTag{
	V5_0 = 0,
	V6_0,
	NUM_OF_CALFILE_VERSION
}CalFileVersion;

//ICAT EXPORTED ENUM
typedef enum PATypeGmskTag{
	PA_TYPE_GMSK_LINEAR,
	PA_TYPE_GMSK_VRAMP,	
	NUM_OF_PA_TYPE_GMSK
}PATypeGmsk;


//ICAT EXPORTED STRUCT
typedef struct Rf_LO_Leakage_Gsm_ConfigNvm_OneBand_Tag
{
	UINT16 GSM_LO_Leakage_Config_Value[SKYLARK_LO_LEAKAGE_REG_LENGTH_GSM]; // GSM need Reg Addr
} Rf_LO_Leakage_Gsm_ConfigNvm_OneBand_ts;


//ICAT EXPORTED STRUCT
typedef struct Rf_LO_Leakage_Gsm_ConfigNvmTag
{
	Rf_LO_Leakage_Gsm_ConfigNvm_OneBand_ts GSM_LO_Leakage_Value[NUM_OF_GSM_BAND]; // 
} Rf_LO_Leakage_Gsm_ConfigNvm_ts;



//ICAT EXPORTED STRUCT
typedef struct RspDigRf3GAuxTablesCalibrationBodySkylarkTag
{
    Int8    radioSwIfVersion;
    Int32   AUX_TABLE_RxSelfCalResults_CalBody[DIM_SIZE_RSP_BAND][16];
    Int32   AUX_TABLE_TxSelfCalResults_CalBody[DIM_SIZE_RSP_BAND][SKYLARK_LO_LEAKAGE_REG_LENGTH_GSM];  // should change to 
    Int16   AUX_TABLE_RxAgcControlCode_CalBody[DIM_SIZE_RSP_BAND][49];
    Int16   AUX_TABLE_RxAgcAccurateGain_CalBody[DIM_SIZE_RSP_BAND][49];
    Int16   AUX_TABLE_RxAgcRipple_CalBody[DIM_SIZE_RSP_BAND][DIM_SIZE_LNA_STATE][DIM_SIZE_BAND_ZONE][2];
    Int16   AUX_TABLE_RxAgcParameters_CalBody[DIM_SIZE_RSP_BAND][3];
    Int16   AUX_TABLE_RxBandZoneToFirstArfcn_CalBody[DIM_SIZE_RSP_BAND][DIM_SIZE_LNA_STATE][DIM_SIZE_BAND_ZONE][1];
    Int16   AUX_TABLE_TxApcRampScale_CalBody[DIM_SIZE_RSP_BAND][DIM_SIZE_TX_MODULATION][DIM_SIZE_BAND_ZONE][DIM_SIZE_PCL][1];
    Int16   AUX_TABLE_TxApcRampScaleMinValue_CalBody[DIM_SIZE_RSP_BAND][DIM_SIZE_TX_MODULATION][DIM_SIZE_TX_MODULATION_TRANSITION][2];
    Int16   AUX_TABLE_TxApcParameters_CalBody[DIM_SIZE_RSP_BAND][DIM_SIZE_TX_MODULATION][5];
    Int16   AUX_TABLE_AfcParameters_CalBody[5];
}RspDigRf3GAuxTablesCalibrationBodySkylark_ts;

typedef struct DigRf_CalNVMSkylarkStruct
{
	NVM_Header_ts                     			nvmHeader;
    RspDigRf3GAuxTablesCalibrationBodySkylark_ts    	gsmCalibData;
} DigRf_CalNVMSkylarkStruct;

//ICAT EXPORTED STRUCT
typedef struct RfPerformanceTuningBodyTag
{
    UINT8  radioSwIfVersion;
	INT16  T2gTxTimingCompensation[4][2];			//qbit
	UINT16 T2gGmskHighPwrCtrl_Ramp[10];	
	UINT16 T2gGmskLowPwrCtrl_Ramp[10];
	UINT16 TS2gNormalTxRampUp[2][2][7];
	UINT16 TS2gNormalTxRampDn[2][2][7];
	UINT16 TS2gMultiTxMidRampUp[2][2][7];
	INT16  R2gRxLevCompensation[4];					//Q4
	UINT16 RFPerformanceTuningTBD[16];	
	UINT16 Reserved;	
}RfPerformanceTuningBody_ts;

typedef struct RfPerformanceTuningNVMStruct
{
	NVM_Header_ts                     	 nvmHeader;
    RfPerformanceTuningBody_ts    	     RfPerformanceTuningData;
} RfPerformanceTuningNVMStruct;



//ICAT EXPORTED STRUCT
typedef struct
{
	UINT16			commandRef;
	UINT16			taskId;
	UINT16 			pmaxReductionFlag;
} plgSetPmaxReductionFlagReq_ts;

//ICAT EXPORTED STRUCT
typedef struct
{
	UINT16			commandRef;
	UINT16			taskId;
	UINT16 			pedestal;
	UINT16          LPI[4];
} plgSetGSMPedestalLPIReq_ts;

#define RSP_DIGRF3G_CALAUX_TABLES_PTR_GET_SKYLARK() (void *)(&rspDigRf3GAuxTablesCalibrationBodySkylark)

typedef enum 
{
	GSMTEST_TRXMODE_OFF=0,
    GSMTEST_TXMODE_ON,
    GSMTEST_RXMODE_ON
}GSM_TEST_MODE;


// function declaretion
TcuEvent *RspRfInitEngine(TcuEvent 			   *pel_p,
						   Int16				   burstQbitOffset,
						   Int16				   *currentQbitOffset,
						   RspType				   currentRspTye,
						   UINT32				  *inputValuesArray,
						   RxTds				   *rxTds_p);



TcuEvent *RspAgcEngine(TcuEvent 				*pel_p,
							Int16					burstQbitOffset,
							Int16					*currentQbitOffset,
							//UINT32 					*inputValuesArray,
							RxTds					*rxTds_p);

TcuEvent *RspSniffEngine(TcuEvent 				*pel_p,
							Int16					burstQbitOffset,
							Int16					*currentQbitOffset,
							UINT32 					*inputValuesArray,
							RxTds					*rxTds_p);



TcuEvent *RspRfTriggerTcuGpo(TcuEvent			*pel_p,
   										Int16	   			*currentQbitOffset,
										Int16				delta,
										Boolean	   			sigValue,
										Int16	 			sigNumber,
										SigChainTcuPort_ts	sigChainTcuPort);

TcuEvent *RspNopEngine(	TcuEvent				*pel_p,
									Int16					delta,
									Int16					*currentQbitOffset);

void SP8RF_RAMInit(void);
void SendAFCParametersToMSA(void);
void DigRf3_Init_GSM_OOS(void);
void DigRf3_Init_WB_OOS(void);
UINT16 Skylark_Get_LO_Leakage_Cal_Value(UINT16 mod_type,UINT16 band_index,UINT16 Tx_Vgc_Value);
void SP8RF_Phase2_RAMInit (void);
void DigRf3_Get_Default_CalData(void);
void TsgSorting(GSMTX_TCU_CTRL *Tcu_ctrl, UINT8 bAscend);
Boolean RspIsRxSpurChannel(Band band, Arfcn rxArfcn);


#endif

