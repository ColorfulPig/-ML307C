/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

/************************************************************************/
/* Filename: SysCfg.h                                                   */
/*                                                                      */
/* Author:   Anton                                                      */
/*                                                                      */
/* Description: all SW mini switches that are related to Flavor         */
/* should be activate only here.                                        */
/* Remarks:                                                             */
/*                                                                      */
/* Created: 15/08/2007                                                  */
/*                                                                      */
/* Modified:                                                            */
/************************************************************************/

#if !defined(SYS_CFG__H)
#define SYS_CFG__H

#include "hal_cfg.h"

#if defined(_HERMON_B0_SILICON_) && !defined(_TAVOR_BOERNE_)
#define PERIPHERAL_CLOCKS_VIA_PMU_API
#else
#define PERIPHERAL_CLOCKS_VIA_PRM_API
#endif

//#if defined(SILICON_TTC_CORE_SEAGULL) || defined(_TAVOR_HARBELL_) || defined(SILICON_PV2)
#if defined(SILICON_TTC_CORE_SEAGULL) || defined(SILICON_PV2)
#define NO_MMU_USED 
#endif

#endif // SYS_CFG__H
