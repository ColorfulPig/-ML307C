/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

/**********************************************************************
*
* Filename: dtc_sim.c
*
* Programmers: Giladk
*
* Functions: dtc simulator header
*
* Description: Header file for dtc simulator. Contains function call declarations, constants and types for use
* 			by f8 and f9 algorithms. Declare tools used by dtc simulator as well.
*
* --------------------------------------------------------------------
* Revision History
*
* Date		   Who		  Version	   Description
* --------------------------------------------------------------------
*
* 06 Dec 2007 	giladk	initial version	create file, insert DTC driver API, implement DTC simulator
*
**********************************************************************/

#ifndef _DTC_SIM_H_
#define _DTC_SIM_H_

#include "dtc_config.h"

/***************************************************************************
* Nested Include Files
***************************************************************************/
#include "dtc.h"
// CQ00092184 begin
#if defined(DTC_PC_TEST_MODE)
#include "dtc_pc_sim_stub.h"
#endif
// CQ00092184 end
/***************************************************************************
 * Manifest Constants
 ***************************************************************************/

/***************************************************************************
*   Macro Functions
***************************************************************************/

#define BITS_PER_INT8 8
#define BITS_PER_INT16 16
#define BITS_PER_INT32 32
#define BITS_TO_INT8S(b) ((b+7)/8)
#define PNULL NULL
#define MIN_VALUE(a,b) (a>b?b:a)

#define UTIL_BITMAP_GET(bitmap,offset)                                      \
    (bitmap [offset/BITS_PER_INT8] & (0x80>>(offset%BITS_PER_INT8))) ? 1 : 0

/***************************************************************************
 * Types
 ***************************************************************************/
	typedef unsigned char Int8;
	typedef unsigned short Int16;
	typedef unsigned long Int32;
	typedef unsigned char UINT8;
	typedef unsigned short UINT16;
	typedef unsigned long UINT32;
	typedef Int8 Boolean;

/***************************************************************************
 * Typed Constants
 ***************************************************************************/

#ifdef UPGRADE_DTC_DRIVER
#ifdef UPGRADE_DTC_SIMULATOR

/***************************************************************************
 * Externals
 ***************************************************************************/
extern volatile UINT32 DtcInIdx[2];
extern volatile UINT32 DtcOutIdx[2];
extern volatile	UINT8 DtcChActive[2];
extern volatile	UINT8 DtcChBlockFlag[2];



/***************************************************************************
 * Function Prototypes
 ***************************************************************************/

extern void             f8Gen
    (Int8                 * key_p,
     Int32                  countC,
     Int32                  bearer,
     DtcDirection             dir,
     Int8                 * data_p,
     Int16                  bitLength,
     Int16                  bitOffset
    );
extern Int8           * f9Gen
    (Int8                 * key_p,
     Int32                  countI,
     Int32                  fresh,
     DtcDirection             dir,
     Int8                 * data_p,
     Int16                  bitLength
    );



void dtcSwapCK(
	UINT8* in,
	UINT8* out);

void dtcSimulateReq(dtcTransferReq_ts *dtcRequest, UINT32 chNum);


#endif //UPGRADE_DTC_SIMULATOR

#endif //UPGRADE_DTC_DRIVER

void dtcUtilBitCopy (
      Int8   *source_p,
      Int32  sourceBitOffset,
      Int32  sourceBitLength,
      Int8   *destination_p,
      Int32  destinationBitOffset);

// CQ00092184 begin
#if defined(DTC_PC_TEST_MODE)
extern Boolean isMaciR9_EHI_g;
extern void Eea3Simulator(Int8* CK,Int32 COUNT,Int32 BEARER,Int32 DIRECTION,Int32 LENGTH,Int8* M,Int8 RemainKeyLenIn,Int8 *RemainKeyIn,Int8 *RemainKeyOut);
extern void Eia3Simulator(Int8* IK,Int32 COUNT,Int32 DIRECTION,Int32 BEARER,Int32 LENGTH,Int8* M,Int8* MAC);
extern UINT8 Eia2Simulator(Int8                 * key_p,
     Int32                  countC,
     Int32                  bearer,
     UINT8                  dir,
     Int8                 * message_p,
     Int16                  bitLength,
     Int16                  bitOffset,
     Int8                 * MACT
    );

extern UINT8 Eea2Simulator
    (Int8                 * key_p,
     Int32                  countC,
     Int32                  bearer,
     UINT8                  dir,
     Int8                 * data_p,
     Int16                  bitLength,
     Int16                  bitOffset,
     Int8                 * RemainKeyIn,
     UINT8                  RemainKeyLenIn,
     Int8                 * RemainKeyOut
    );
// CQ00092184 end	
#endif



#endif

