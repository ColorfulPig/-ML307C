/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
INTEL CONFIDENTIAL
Copyright 2006 Intel Corporation All Rights Reserved.
The source code contained or described herein and all documents related to the source code ("Material") are owned
by Intel Corporation or its suppliers or licensors. Title to the Material remains with Intel Corporation or
its suppliers and licensors. The Material contains trade secrets and proprietary and confidential information of
Intel or its suppliers and licensors. The Material is protected by worldwide copyright and trade secret laws and
treaty provisions. No part of the Material may be used, copied, reproduced, modified, published, uploaded, posted,
transmitted, distributed, or disclosed in any way without Intel's prior express written permission.

No license under any patent, copyright, trade secret or other intellectual property right is granted to or
conferred upon you by disclosure or delivery of the Materials, either expressly, by implication, inducement,
estoppel or otherwise. Any license under such intellectual property rights must be express and approved by
Intel in writing.
-------------------------------------------------------------------------------------------------------------------*/



/***************************************************************************
 * Constants
 ***************************************************************************/

/* CQ00014180 begin */
#define MSA_POWER_MANAGER_CTRL_LENGTH             3
#define MSA_POWER_MANAGER_CTRL_LENGTH_TD_DRX_OOS  2

#define MSA_POWER_MANAGER_CTRL_ENABLE             0 /* word #0 */
#define MSA_POWER_MANAGER_USB_CNNT                1 /* Word #1 */
#define MSA_POWER_MANAGER_RAT                     1 /* Word #1 */
#define MSA_POWER_MANAGER_RESERVE                 2 /* Word #2 */

/* CQ00014180 end */



// following defines should be moved to Platform
#ifdef PHS_SW_DEMO_TTC
#define COEL_ABP_GSM_WB_CHOOSE ((volatile UINT32 *)0xD4070010)
//RTU clock enable :Set 1 will enable the RTU clock. default=0 needed to be checked in code.
#define COEL_APB_RTU_CLKEN ((volatile UINT32 *)0xD4070014) 
#define COEL_APB_SPARE1        ((volatile UINT32 *)0xD4070018) 
#define COEL_APB_SPARE2        ((volatile UINT32 *)0xD407001C)

/* TCU clock trigger control register  0:trigger from slow clock 1:trigger from WB */
#if defined (EDEN_1928) || defined (NEZHA3_1826)
//#define TCTS_CTL_REG_ADD ((volatile UINT32 *)0xD023B000)
#define TCTS_CTL_REG_ADD ((volatile UINT32 *)0xD404B000) //qianying
#else
#define TCTS_CTL_REG_ADD ((volatile UINT32 *)0xD403B000)
#endif
#else
#define MVT_CTL_REG_ADD ((volatile UINT32 *)0xF0203030)
#define GPB_MVT_REG_ADD ((volatile UINT32 *)0xF024000C)

#endif

#ifdef VALIDATION_STAND_ALONE

//ICAT EXPORTED STRUCT
typedef struct {
    UINT8      appBusyState;
    UINT8      appStartRunTimeValid;
    UINT32     appStartRunTime;
}  aamManageParamsForSA_ts;

//ICAT EXPORTED STRUCT
typedef struct {
    UINT8      waitsForBBD2OK;
    UINT8      pad;
}  aamControlParamsForSA_ts;

#endif

