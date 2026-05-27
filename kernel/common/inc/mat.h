/*----------------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------------------*/
/*                                                                      */
/*                                                                      */
/*  This file and the software in it is furnished under                 */
/*  license and may only be used or copied in accordance with the terms */
/*  of the license. The information in this file is furnished for       */
/*  informational use only, is subject to change without notice, and    */
/*  should not be construed as a commitment by Marvell Corporation.     */
/*  Marvell Corporation assumes no responsibility or liability for any  */
/*  errors or inaccuracies that may appear in this document or any      */
/*  software that may be provided in association with this document.    */
/*  Except as permitted by such license, no part of this document may   */
/*  be reproduced, stored in a retrieval system, or transmitted in any  */
/*  form or by any means without the express written consent of Marvell */
/*  Corporation.                                                        */
/*                                                                      */
/* Title: MAT module                                                    */
/*                                                                      */
/* Filename: mat.h                                                      */
/*                                                                      */
/* Author:   bchang                                                     */
/*                                                                      */
/* Project, Target, subsystem: Tavor, Arbel & Boerne RTOS, HAL          */
/*                                                                      */
/* Remarks: -                                                           */
/*                                                                      */
/* Created: 10/12/2013                                                  */
/*                                                                      */
/* Modified:                                                            */
/************************************************************************/

/************************************************************************/
#if !(defined __MAT_H__)
#define __MAT_H__

#include "global_types.h"

/*---- parameter command type ----
  the type definition is generally following AT CMD string define
  AT commands are listed here.
*/
//ICAT EXPORTED ENUM
typedef enum
{
	MAT_MSCLASS,
	MAT_AOPS,
	MAT_AOPSCFG,
	MAT_CLAC,
	MAT_E,
	MAT_I,
	MAT_L,
	MAT_M,
	MAT_O,
	MAT_P,
	MAT_Q,
	MAT_T,
	MAT_V,
	MAT_X,
	MAT_Z,
	MAT_ampC,
	MAT_ampD,
	MAT_ampF,
	MAT_ampS,
	MAT_ampZ,
	MAT_ampM,
	MAT_ampV,
	MAT_ampW,
	MAT_CGMI,
	MAT_CGMM,
	MAT_CGMR,
	MAT_CGSN,
	MAT_CSCS,
	MAT_CIMI,
	MAT_ASTO,
	MAT_GMI,
	MAT_GMM,
	MAT_GMR,
	MAT_GSN,
	MAT_GOI,
	MAT_GCAP,
	MAT_GCI,
	MAT_IPR,
	MAT_ICF,
	MAT_IFC,
	MAT_IDSR,
	MAT_EXAMPLE,
	MAT_RawAT,

	MAT_A,

	MAT_D,
	MAT_H,
		
	MAT_CSTA,	
	MAT_CMOD,
	MAT_CVMOD,
	MAT_CHUP,	
	MAT_CBST,
	MAT_CVHU,
	MAT_CRLP,
	MAT_CEER,	
	MAT_CMUT,
	MAT_ECHUPVT,

	MAT_CREG,
	MAT_CIND,
	MAT_COPS,
	MAT_CPOL,
	MAT_CLCK,
	MAT_CPWD,
	MAT_CLIP,
	MAT_CLIR,
	MAT_COLP,
	MAT_COLR,
	MAT_CNAP,
	MAT_CCFC,
	MAT_CCWA,
	MAT_FDNCHECK,
	

	MAT_CHLD,
	MAT_CAOC,
	MAT_VTS,
	MAT_VTD,
	MAT_CSSN,
	MAT_CLCC,
	MAT_FCLASS,
	MAT_CDU,
	MAT_dollarVTS,
	MAT_starDIALE,
	MAT_CSCB,
	MAT_starCISCC,	

#ifdef VOLTE_ENABLE
	MAT_starCIIND,
	MAT_starIMSSRV,
    MAT_MORESMS,
	MAT_POCCMD,	    
#endif

	MAT_startECCLIST,
	MAT_starCCIREG,
	MAT_CUSD,
	MAT_PEER,
	MAT_CSQ,
	MAT_starREJCUSE,
	MAT_CMUX,
#ifdef AT_FOR_GW20180208SN
	MAT_NETDMSG,
	MAT_CSQEX,
#endif
	MAT_CPAS,
	MAT_CFUN,
	MAT_starCFUN,
	MAT_CPIN,
	MAT_CPIN2,
	MAT_EPIN,
	MAT_CPINR,	
	MAT_starSIMDETEC,
	MAT_CTZR,
	MAT_CTZU,
	MAT_starCTZR,
	MAT_CPBS,
	MAT_CPBR,
	MAT_CPBW,
	MAT_CPBF,
	MAT_CSIM,
	MAT_CRSM,
	MAT_CGLA,
	MAT_CRLA,
	MAT_CCHO,
	MAT_CCHC,
	MAT_MSTK,
	MAT_starEUICC,
	MAT_CACM,
	MAT_CAMM,
	MAT_CCWE,
	MAT_ADMINDATA,	
	MAT_CGREG,
	MAT_starREGOPT,
	MAT_CGATT,
	MAT_CGACT,
	MAT_CGDATA,
	MAT_CGDCONT,
	MAT_CGDSCONT,
	MAT_CGQMIN,
	MAT_CGQREQ,
	MAT_CGEQREQ,
	MAT_CGEQMIN,
	MAT_GETIP,
	MAT_starTGSINK,
	MAT_CGSEND,
	MAT_starICSSINK,
	MAT_starAUTHReq,
	#ifdef CRANE_MCU_DONGLE
	MAT_starCHAPAUTH,
	#endif
	MAT_CMGF,
	MAT_starCMGF,
	MAT_LKSMSSTA,
	MAT_CMSS,
	MAT_CMGS,
	MAT_CMGR,
	MAT_CMGW,
	MAT_CSCA,
	MAT_CNMI,
	MAT_CGSMS,
	MAT_CMMS,
	MAT_CMGD,
	MAT_CMGC,	
	MAT_CMGL,
	MAT_CSMS,
	MAT_CPMS,
	MAT_CNMA,
	MAT_CSMP,
	MAT_CSDH,
	MAT_CSAS,
	MAT_CRES,
	MAT_CPNER,
	MAT_CGCI,
	MAT_CGOI,
	MAT_VDUMP,
	MAT_VPDUS,
	MAT_VHDL,
	MAT_VECHO,
	MAT_ATDB,
	MAT_CPUC,
    MAT_CRC,
	MAT_CMEE,
	MAT_CDIP,
	MAT_CPLS,
	MAT_CGCMOD,
	MAT_CNUM,
	MAT_DS,
	MAT_CGTFT,
	MAT_starBAND,
	MAT_starBANDIND,
	MAT_starBANDRD,
	MAT_starCLCK,
	MAT_starMEPCG,
	MAT_starENVSIM,
	MAT_starCNMA,
	MAT_starRSTMEMFULL,
	MAT_starPOWERIND,
	MAT_starFASTDORM,
	MAT_starCellLock,
	MAT_EEMOPT,
	MAT_EEMGINFO,
	MAT_ERGA,
	MAT_ERTCA,
	MAT_starCam_I2C,
	MAT_starISP_REG,
	MAT_starCam_rawdump,
	MAT_starFILETEST,	
	MAT_starMRD_CDF,
	MAT_starMRD_IMEI,
	MAT_starMRD_MEP,
	MAT_starMRD_CalInfo,
	MAT_starMRD_MEPPLMN,
	MAT_starMRD_SN,
	MAT_starMRD_ITEM,
	MAT_starMRD_ADC,
	MAT_starMRD_RTPADC,
	//#ifdef AT_PRODUCTION_CMNDS
	MAT_starGSMTR,
	//#endif
	MAT_starCGSN,
	MAT_starHTCCTO,
	MAT_CMEMFULL,
	MAT_starEHSDPA,
	#ifdef CI_NEW_FEATURE1
	MAT_TPCN,
	MAT_FWDB,
	#endif
	MAT_starFDY,
	MAT_xorSYSINFO,
	MAT_starCPBC,
	MAT_starFDNBYPASS,
	MAT_starCSCB,
	MAT_starCBMCS,
	MAT_starNASCHK,
	MAT_CGEQOS,
	MAT_CEREG,
	MAT_CGCONTRDP,
	MAT_CGSCONTRDP,
	MAT_CGTFTRDP,
	MAT_CGEQOSRDP,
	MAT_CGEREP,
	MAT_CEMODE,
	MAT_CGPADDR,
	MAT_xorCACAP,
	MAT_CGCLASS,
	MAT_CESQ,
	MAT_BGLTEPLMN,
	MAT_STARCGDFAUTH,
	MAT_dollarMYMINISYS,
	MAT_dollarMYFOTA,
	MAT_VZWRSRP,
	MAT_VZWRSRQ,
	MAT_starCGDFLT,
#ifdef LWIP_IPNETBUF_SUPPORT
	MAT_STARNETACT,
	MAT_STARNETREF,
	MAT_STARNETDNS,
	MAT_STARNETIF,
	MAT_STARNETIFCM,
	MAT_STARMPSAPN,
#endif
	MAT_starMODEMRESET,
	MAT_starVZWTESTAPP,
	MAT_VZWAPNE,
	MAT_COPN,
	MAT_starGATR,
	MAT_starGRIP,
	MAT_playMP3,
	MAT_starCGMR,
	MAT_starCOMCFG,
	MAT_starRFTEMP,
	MAT_startRFTEMPEX,
#ifdef CRANE_SOC_TEMPERATURE_SENSOR	
	MAT_starSOCTEMP,
#endif	
	MAT_TEMPTEST,
	MAT_BANSELCT,
	MAT_SYSSLEEP,
	MAT_starCGATT,
	MAT_CGPIAF,
	MAT_CIREG,
	MAT_starUSBT,
	MAT_starLTECOEX,
	MAT_LTEPOWER,
	MAT_LTETR,
	MAT_COMMTR,	
	MAT_starCSQ,
	MAT_MAXPOWER,
	MAT_SIMDETEC,
	MAT_CPLMNS,
	MAT_WS46,
	MAT_starCELL,
	MAT_CISRVCC,
	MAT_CEVDP,
	MAT_CEUS,
	/* add for BT SAP */
	MAT_BTSTATR,
	MAT_starWBAMR,
	MAT_CNMPSD,
	MAT_starREGMODE,
	MAT_starIMLCONFIG,
	MAT_CCLK,
	MAT_starURSLCT,
	MAT_starCBRAT,
	MAT_starSECCAP,
	MAT_starLTEBAND,
	MAT_starPSTHRESHOLD,
#ifdef AGPS_ENABLE	
	MAT_LPNWUL,
	MAT_LPLOCVR,	
	MAT_LPECID,
	MAT_LPOTDOAABORT,
	MAT_LPOTDOAREQ,
    MAT_AGPSSET,
#endif
    MAT_L1DEBUG,
    MAT_CSCO,
	MAT_CIREP,
	MAT_OPERCFG,
	MAT_starPSDC,
	MAT_CEN,
	MAT_CNEM,
	MAT_CAVIMS,
	MAT_CASIMS,
	MAT_CMMIVT,
	MAT_CPSMS,
	MAT_CEDRXS,
	MAT_CEDRXRDP,
	MAT_CCIOTOPT,
	MAT_CRCES,
	MAT_CSCON,
	MAT_CIPCA,
	MAT_CABTSR,
	MAT_CABTRDP,
	MAT_CGAPNRC,
	MAT_MPBK,
	MAT_CSODCP,
	MAT_CRTDCP,
#ifdef LWIP_IPNETBUF_SUPPORT
	MAT_LOG,
	MAT_starDIALMODE,
	MAT_starAPNMODE,
	MAT_starAGDCONT,
	MAT_starAGACT,
	MAT_starLWIPCTRL,
	MAT_starNTP,
	MAT_starCGDCONT,
	MAT_starMTU,
	MAT_plusSWITCHSIM,
	MAT_plusDUALSIM,
	MAT_plusBINDSIM,
#endif
	MAT_plusCWRITESIM,
//#ifdef BT_TEST_SUPPORT  20201214@xiaokeweng force enable as BT/WIFI could be enable/disable in SDK build
    MAT_BTTEST,
//#endif
    MAT_GPSINIT,
    MAT_GPSSLEEP,
    MAT_GPSPF,
    MAT_AGNSSGET,
    MAT_AGNSSSET,
    MAT_GPSST,
    MAT_GPSSET,
	MAT_TRUSTNUM,
	MAT_CHKTRUSTNUM,
	MAT_starISIMAID,
	MAT_CSSAC,
	MAT_MEDCR,
	MAT_AGNSSCFG,
#ifdef UNIDMP_SUPPORT
    MAT_UNIKEYINFO,
    MAT_UNIKEYINFOM,
    MAT_UNICERTINFO,
    MAT_UNISHCERTINFO,
    MAT_UNIDELCERTINFO,
    MAT_UNIMQTTCONN,
    MAT_UNIMQTTDISCON,
    MAT_UNIMQTTSTATE,
    MAT_UNIMQTTSUB,
    MAT_UNIMQTTPUB,
    MAT_UNIPSMSET,
    MAT_UNIDMPAPNSET,
    MAT_UNIDMPNETLOG,
#endif
#ifdef CRANE_MCU_DONGLE
	MAT_STARREADVER,
	MAT_STARREADCPUUID,
	MAT_RESET,
	MAT_RSTSET,
#ifndef NO_EXTEND_MY_Q_AT
	MAT_DNS,
	MAT_dollarMYPOWEROFF,
	MAT_dollarMYSOCKETLED,
	MAT_dollarMYGMR,
	MAT_dollarMYCCID,
	MAT_dollarMYNETURC,
	MAT_dollarMYTYPE,
	MAT_dollarMYNETCON,
	MAT_dollarMYNETACT,
	MAT_dollarMYIPFILTER,
	MAT_dollarMYNETSRV,
	MAT_dollarMYNETOPEN,
	MAT_dollarMYNETREAD,
	MAT_dollarMYNETWRITE,
	MAT_dollarMYNETCLOSE,
	MAT_dollarMYNETACK,
	MAT_dollarMYNETACCEPT,
	MAT_dollarMYNETCREATE,
	MAT_dollarMYFTPOPEN,
	MAT_dollarMYFTPCLOSE,
	MAT_dollarMYFTPSIZE,
	MAT_dollarMYFTPGET,
	MAT_dollarMYFTPPUT,
	MAT_dollarMYBCCH,
	MAT_dollarMYBAND,
	MAT_dollarMYTIMEUPDATE,
	MAT_dollarMYLACID,
	MAT_dollarMYGPSPOS,
	
	MAT_dollarMYGETKEY,
	
	MAT_dollarMYSETINFO,
	MAT_dollarMYSYSINFO,
	MAT_dollarMYSYSINFOURC,
	MAT_dollarMYDOWNLOAD,
	MAT_QICSGP,
	MAT_QIACT,
	MAT_QIDEACT,
	MAT_QIOPEN,
	MAT_QICLOSE,
	MAT_QISTATE,
	MAT_QISEND,
	MAT_QIRD,
	MAT_QISENDEX,
	MAT_QISWTMD,
	MAT_QIGETERROR,
	MAT_QISDE,
	MAT_QICFG,
	MAT_QFTPCFG,
	MAT_QFTPOPEN,
	MAT_QFTPCLOSE,
	MAT_QFTPCWD,
	MAT_QFTPPWD,
	MAT_QFTPPUT,
	MAT_QFTPGET,
	MAT_QFTPSIZE,
	MAT_QFTPDEL,
	MAT_QFTPMKDIR,
	MAT_QFTPRMDIR,
	MAT_QFTPMDTM,
	MAT_QFTPRENAME,
	MAT_QFTPLIST,
	MAT_QFTPNLST,
	MAT_QFTPMLSD,
	MAT_QFTPLEN,
	MAT_QFTPSTAT,
	MAT_TCPKEEPALIVE,
#ifdef ATNET_SSL
	MAT_QSSLCFG,
	MAT_QSSLOPEN,
	MAT_QSSLSEND,
	MAT_QSSLRECV,
	MAT_QSSLCLOSE,
	MAT_QSSLSTATE,
#endif
#else
	MAT_dollarMYDOWNLOAD,//by cmiot
#endif
#endif

#ifdef NEUL_COMMAND
	MAT_NSOCR,
	MAT_NSOST,
	MAT_NSORF,
	MAT_NSOCL,
	MAT_NCDP,
	MAT_NMGS,
	MAT_NMGR,
	MAT_NNMI,
	MAT_NSMI,
	MAT_NQMGR,
	MAT_NQMGS,
	MAT_NRB,
	MAT_NUESTATS,
	MAT_NEARFCN,
	MAT_NPING,
	MAT_NBAND,
	MAT_NLOGLEVEL,
	MAT_NCONFIG,
	MAT_NTSETID,
#endif
	MAT_xorHVER,
	MAT_starPROD,
	MAT_NVMFLUSH,
	MAT_starSLT,
	MAT_starPMICREG,
	MAT_starREGRW,
	MAT_starSSGLPC,
	MAT_ZDON,
	MAT_starASRCOPS,
	MAT_starICCID,
	MAT_CEN1,
	MAT_CEN2,
	MAT_starSULOGCFG,	
#ifdef ENABLE_WIFI_SCAN
	MAT_starWIFICTRL,
#endif
	MAT_starSIMPOLL,
	MAT_cellinfo,
	MAT_starEPIN,
	MAT_starVER,
//#ifdef WIFI_FUNCTION_SUPPOR 20201214@xiaokeweng force enable as BT/WIFI could be enable/disable in SDK build	
	MAT_WIFI_CMD,	
//#endif	
	MAT_Audio_CMD,
    MAT_starMRDBACKUP,
    MAT_starSELECTVSIM,
    MAT_starAVSIM,
    MAT_starMRDWIFIMAC,
    MAT_starMRDBTID,
    MAT_starAUDNVM,
	MAT_starSPN,
    MAT_CMRSS,
    MAT_CMGSS,
    MAT_CMSMS,
    MAT_CMGMS,
    MAT_PACSP,  
    MAT_ENVCFG,
    MAT_CUAD,
//#ifndef LTEONLY_THIN
#ifndef NO_AUDIO
#ifdef ECALL_SUPPORT    
    MAT_CECALLINSIDE,
    MAT_CECALL,
    MAT_ECALLDATA,
    MAT_ECALLVOICE,
    MAT_ECALLCFG,
    MAT_ECALLONLY,
    MAT_ECALLREG,
    MAT_ECALLONLYSIM,
    MAT_ECALLMSDGEN,
    MAT_ECALLMSD,
    MAT_ECALLPUSH,
    MAT_ECALLMSDCFG,
    MAT_ECALLMEDIAMSD,
    MAT_IMSECALLSUPT,
    MAT_ECALLONLYREG,
    MAT_ECALLOVERIMS,
    MAT_ECALLSMSNUM,
    MAT_ECALLSMS,
    MAT_ECALLMODE,
    MAT_ECALLTIMER,
    MAT_ECALLMUTESPK,
#endif
	MAT_AUDGAIN,
	MAT_AUDREC,
	MAT_AUDRECSTOP,
	MAT_AUDPLAY,
	MAT_AUDPLAYSTOP,
#endif
//#endif
    MAT_STARGETIP,
    MAT_ROAMINGDATA,
    MAT_starDNSCFG,
    MAT_COMFEATURE, 
    MAT_RPM,
    MAT_CFGRPMSWITCH,
    MAT_CFGRPMPARA,
    MAT_CFGRPMCOUNTER,
    MAT_CFGRPMCLR, 
    MAT_CMER,
	  
#if (!defined(LTEONLY_THIN) && !defined(LTEGSM_THIN))
    MAT_RESENDPARA,
#endif
#ifdef LPA_SUPPORT
    MAT_CTRSPSTRT,
    MAT_CTRSPGETINFO,
    MAT_CTRSPPROFACT,
    MAT_CTRSPNTFYLEAB,
    MAT_CTRSPSVN,
#endif    
    MAT_CR,
#ifdef SINGLE_SIM
	MAT_plusSINGLESIM,
	MAT_SIMSLOT,
#endif
	MAT_starBLACKCELL,    
	MAT_UNKNOWN,
	MAT_CMD_UNKNOWN = MAT_UNKNOWN,

	/* response type for MAT */
	MAT_RSP_UNKNOWN = 1000,
	MAT_RSP_OK,
	MAT_RSP_ERROR,
	MAT_RSP_CME_ERROR,
	MAT_RSP_CMS_ERROR,
	MAT_RSP_BUSY,

	/* indication type for MAT */
	MAT_IND_CONNECT,
	MAT_IND_NO_CARRIER,
	MAT_IND_RING,
	MAT_IND_NO_ANSWER,
	MAT_IND_NO_DIALTONE,

	/* CC Indication */
	MAT_IND_CRING,
	MAT_IND_CCCM,
	MAT_IND_CSSU,
	MAT_IND_CSSI,
	MAT_IND_CR,
	MAT_IND_CEI,

	/* DEV Indication */
	MAT_IND_SYSCONFIG,
	MAT_IND_EEMGINFOBASIC,
	MAT_IND_EEMGINFOSVC,
	MAT_IND_EEMGINFOPS,
	MAT_IND_EEMGINFONC,
	MAT_IND_EEMGINBFTM,
	MAT_IND_EEMUMTSSVC,
	MAT_IND_EEMUMTSINTRA,
	MAT_IND_EEMUMTSINTER,
	MAT_IND_EEMUMTSINTERRAT,
	MAT_IND_EEMLTESVC,
	MAT_IND_EEMLTEINTRA,
	MAT_IND_EEMLTEINTER,
	MAT_IND_EEMLTEINTERRAT,
	MAT_IND_SNETIND,
	MAT_IND_LPNWDL,
	MAT_IND_LPSTATE,
	MAT_IND_LPMEAST,
	MAT_IND_LPRESET,
	MAT_IND_DIP,
	MAT_IND_LPOTDOAMEAS,
	/* MM Indication */
	MAT_IND_CACAP,
	MAT_IND_MODE,
	MAT_IND_COPN,
	MAT_IND_NITZ,
	MAT_IND_MSRI,
	MAT_IND_HOME_ZONE,

	/* MSG Indication */
	MAT_IND_MMSG,
	MAT_IND_CMTI,
	MAT_IND_CBM,
	MAT_IND_CDS,
	MAT_IND_CMT,

	/* PB Indication */
	MAT_IND_SCPBR,
	MAT_IND_MPBK,

	/* PS Indication */
	MAT_IND_CGEQNEG,
	MAT_IND_CGEV,

	/* SIM Indication */
	MAT_IND_COTA,
	MAT_IND_REFRESH,
	MAT_IND_SIM_RESET,
	MAT_IND_CARDMODE,
	MAT_IND_SPN,

	/* SS Indication */
	MAT_IND_LPLOC,
	MAT_IND_SSRC,

	/* DAT Indication */
	MAT_IND_PSSDC,

	/* the change of sim/usim availability status report */
	MAT_IND_BTSSTAT,

	MAT_IND_DSAC,
	MAT_IND_ADMINDATA,
	MAT_IND_CIMI,
	MAT_IND_PSLOAD,
	MAT_IND_RBLOOP,
	MAT_IND_CELL,
	MAT_IND_CIREPI,
	MAT_IND_CIREPH,
	MAT_IND_DATASTATUS,
	MAT_IND_CEDRXP,
	MAT_IND_CCIOTOPTI,
	MAT_IND_CABTSRI,
	MAT_IND_CIREGU,
	MAT_IND_AMRCODEC,
	MAT_IND_CNEC_ESM,
	MAT_IND_ATREADY,
	MAT_IND_PLMNLIST,
	MAT_IND_WIFICELLINFO,

//#if defined (ML302A_SUPPORT)
#include "cm_atcmd_mat.h"
//#endif
	NUM_OF_MAT_CMD
}  MATCmdType, MATRspType;


/*---- parameter service ID ----
*/
//ICAT EXPORTED ENUM
typedef enum
{
   MAT_SVC_0,
   MAT_SVC_1,
   MAT_SVC_2,
   MAT_SVC_3,
   MAT_SVC_4,
   MAT_SVC_5,
   MAT_SVC_6,
   MAT_SVC_7,
   MAT_SVC_8,
   MAT_SVC_9,
   MAT_SVC_10,
   MAT_SVC_11,
   MAT_SVC_12,
   MAT_SVC_13,
   MAT_SVC_14,
   MAT_SVC_15,
   MAT_SVC_16,
   MAT_SVC_17,
   MAT_SVC_18,
   MAT_SVC_19,
   MAT_SVC_20,
   MAT_SVC_21,
   MAT_SVC_22,
   MAT_SVC_23,
   MAT_SVC_24,
   MAT_SVC_25,
   MAT_SVC_26,
   MAT_SVC_27,
   MAT_SVC_28,
   MAT_SVC_29,
   MAT_SVC_30,
   MAT_SVC_31,
   MAT_SVC_32,
   MAT_SVC_33,
   MAT_SVC_34,
   MAT_SVC_35,
   MAT_SVC_36,
   MAT_SVC_37,
   MAT_SVC_38,
   MAT_SVC_39,
   MAT_SVC_40,
   MAT_SVC_41,
   MAT_SVC_42,
   MAT_SVC_43,
   MAT_SVC_44,
   MAT_SVC_45,
   MAT_SVC_46,
   MAT_SVC_47,
   MAT_SVC_48,
   MAT_SVC_49,
   MAT_SVC_50,
   MAT_SVC_51,
   MAT_SVC_52,
   MAT_SVC_53,
   MAT_SVC_54,
   MAT_SVC_55,
   MAT_SVC_56,
   MAT_SVC_57,
   MAT_SVC_58,
   MAT_SVC_59,
   MAT_SVC_60,
   MAT_SVC_61,
   MAT_SVC_62,
   MAT_SVC_63,
   NUM_OF_MAT_SVC
}MATSvcId;

//ICAT EXPORTED ENUM
typedef enum
{
	MAT_SIM_0,
	MAT_SIM_1,
	NUM_OF_MAT_SIM
}MATSimId;

/*---- data type of MAT parameters ----
*/
//ICAT EXPORTED ENUM
typedef enum {
    MAT_DATA_TYPE_DECIMAL,
    MAT_DATA_TYPE_STRING,
	MAT_DATA_TYPE_SET,
	MAT_DATA_TYPE_RANGE,
} MATDataType;

//ICAT EXPORTED STRUCT
struct MATParameterRange
{
	int min;
	int max;
};

/*---- data type of MAT parameters ----
*/
//ICAT EXPORTED STRUCT
struct MATParameterItem_T;
//ICAT EXPORTED STRUCT
typedef union MATDataValue_U {
    unsigned int	decimal;
    char			*string_p;
	struct MATParameterItem_T *set_p;
	struct MATParameterRange range;
} MATDataValue;

/*  parameters definition
     type       -- data type for value
     is_default -- if the AT parameter is omitted, we should pass a defaut value
     value      -- value for each parameter
*/
//ICAT EXPORTED STRUCT
typedef struct MATParameter_T
{
    MATDataType  type;
    BOOL         is_default;
    MATDataValue value;
}MATParameter;

/*
 * macro helper for setting parameter
 */
#define MAT_INT_VALUE(param, v) \
    do \
    { \
      (param).type = MAT_DATA_TYPE_DECIMAL; \
      (param).is_default = FALSE; \
      (param).value.decimal = (v); \
    } while (0)

#define MAT_STR_VALUE(param, v) \
    do \
    { \
      (param).type = MAT_DATA_TYPE_STRING; \
      (param).is_default = FALSE; \
      (param).value.string_p = (v); \
    } while (0)

#define MAT_RANGE_VALUE(param, min, max) \
    do	\
    {	\
      (param).type = MAT_DATA_TYPE_RANGE; \
      (param).is_default = FALSE;		\
      (param).value.range.min = (min); \
	  (param).value.range.max = (max); \
    } while (0)

#define MAT_DEFAULT_VALUE(param) \
    do \
    { \
      (param).type = MAT_DATA_TYPE_DECIMAL; \
      (param).is_default = TRUE; \
      (param).value.decimal = 0; \
    } while (0)

#define MAT_SET_VALUE(param, v)	\
    do \
    { \
      (param).type = MAT_DATA_TYPE_SET; \
      (param).is_default = FALSE; \
      (param).value.set_p = (v); \
    } while (0)

//ICAT EXPORTED ENUM
typedef enum
{
    MAT_RC_NULL,
    MAT_RC_OK,        // OK will be sent
    MAT_RC_ERROR,     // ERROR will be sent
    MAT_RC_CME_ERROR, // CME ERROR will be sent
    MAT_RC_CMS_ERROR, // CMS ERROR will be sent
    MAT_RC_RESERVED
}MATResultCode;

//ICAT EXPORTED STRUCT
typedef struct MATParameterItem_T
{
	UINT16  max_para_number;
    UINT16 num_Retparameters;
    MATParameter* RetPara;
}MATParameterItem, MATParameterSet;

//ICAT EXPORTED ENUM
typedef enum
{
    MAT_RET_CONFIRM,
    MAT_RET_INDICATION
}MATRetType;

//ICAT EXPORTED ENUM
typedef enum
{
	MAT_PAR_RESULT_CODE_OK,
	MAT_PAR_RESULT_CODE_ERROR
}MATParseResultCode;

//ICAT EXPORTED ENUM
typedef enum
{
    MAT_RET_SUCCESS,
	MAT_RET_TOO_MANY_PARAMETERS,
    MAT_RET_CURRENT_CH_HAS_CMD_PROCESSING,
    MAT_RET_PARA_TYPE_UNSUPPORT,
    MAT_RET_NULL_CLINET_TAG_POINTER,
    MAT_RET_NUM_PARA_ERROR,
    MAT_RET_PARA_TYPE_ERROR,
    MAT_RET_HANDLER_FAILURE,
    MAT_RET_FIND_FREE_CHANNEL_ERROR
}MATRecvRetType;

/* MMI<--MAT return parameters definition
   cmd_type          -- the command or indication type.
   resultCode        -- general result code: OK, ERROR, CME/CMS ERROR

   numItems          -- the number of items in the returned parameters
   RetParaItems      -- the detail value of each returned parameters
*/
//ICAT EXPORTED STRUCT
typedef struct MATReturnPara_T
{
    MATResultCode resultCode;
    UINT16 numItems;
    MATParameterItem* RetParaItems;
	MATRspType cmd_type;
} MATReturnPara;


typedef struct _ShmApiMsg
{
	int svcId;
	int procId;
	int msglen;
}ShmApiMsg;


#define SHM_HEADER_SIZE sizeof(ShmApiMsg)



/*
 * This function gets a byte steam from a string.
 *
 * @str the hex string, for example 010203ffeeef
 * @length the length of output byte stream
 *
 * @return the byte stream.
 * @note the byte stream must be free when not used.
 */
UINT8* MATGetDataFromStr(const char* str, int *length);
UINT8* MATGetStrFromData(const char* Bytestreamdata, int data_length);
#endif  /* _MAT_H_ */

