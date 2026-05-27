#ifndef	LFS_CACHE_H
#define	LFS_CACHE_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                lfs_cache.h


GENERAL DESCRIPTION

    This file is for nor flash handware attaction layer.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

   Copyright (c) 2011 by Marvell, Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when         who        what, where, why
--------   ------     ----------------------------------------------------------
12/23/07   zhoujin    Created module
===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "UART.h"
#include "lfs.h"
#include "lfs_trace.h"

/*===========================================================================

                        LOCAL MACRO
===========================================================================*/

/* CRANE QSPI base address.*/
#define CRANE_QSPI_BASE_ADDRESS             0x80000000

/* Invalid LFS block number.*/
#define LFS_INVALID_NUMBER                  0xFACEBABE

/* LFS queue ID */
#define LFS_QUEQUE_ID                       0xABBACDDC

/* LFS queue guard */
#define LFS_QUEQUE_GUARD                    0x68686868

/* LFS unlink ID */
#define LFS_QUNLINK_ID                      0xABBA0000

/* LFS unlink guard */
#define LFS_QUNLINK_GUARD                   0x68680000

/* LFS Write back to Flash flag */
#define LFS_WRITE_BACK_TO_FALSH             0x1

/* LFS cache task stack size */
#define LFS_TASK_STACK_SIZE                 2048

/* LFS Max cache count */
#define LFS_CACHE_CNT                       50

/* Array Size */
#define ARR_SIZE(a)                         (sizeof((a))/sizeof((a[0])))

#if (defined(CRANEL_4MRAM) || defined(LTEONLY_8M8M) || defined(LTEGSM_THIN))
//#define LFS_USE_CACHE

/* LFS cache task priority */
#define LFS_TASK_PRIORITY                   41

#else
/* LFS cache task priority */
#define LFS_TASK_PRIORITY                   250

#endif

/*===========================================================================

             EXTERN DEFINITIONS AND DECLARATIONS FOR MODULE

 ===========================================================================*/

/* LFS block information */
typedef struct lfsBlockInfo
{
    UINT32 FlashBlockSize;
    UINT32 LfsStartAddress;
    UINT32 LfsEndAddress;
    UINT32 LfsStartBlock;
    UINT32 LfsEndBlock;
} lfsBlockInfo;

/* LFS block bit map */
typedef struct lfsBlkMapType
{
	BOOL    Alloc;
	UINT32  LfsGrpCnt;
	UINT32  *LfsBlkTbl;
	UINT32  GrpOfst;
	UINT32  BitOfst;
} lfsBlkMapType;

/* LFS Message queue */
typedef struct lfsMsgQ
{
	UINT32  id;
	UINT32  block;
	UINT8  *cache;
	UINT32  dirty;
    struct lfsMsgQ *next;
    struct lfsMsgQ *prev;
    UINT32  cnt;
    UINT32  guard;
} lfsMsgQ;

/*===========================================================================

                          INTERNAL FUNCTION DECLARATIONS

===========================================================================*/

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      GetLfsBlkInfo                                                    */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function Get LFS Block info.                                 */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
lfsBlockInfo *GetLfsBlkInfo(int partition);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      GetLfsBlockMapInfo                                               */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function Get LFS Block Map infomation.                       */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
lfsBlkMapType *GetLfsBlockMapInfo(int partition);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      lfs_cache_init                                                   */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function initialize LFS cache.                               */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void lfs_cache_init(int partition);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      lfs_erase_all                                                    */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function erase all LFS flash region.                         */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void lfs_erase_all(void);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      lfs_write_back                                                   */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function set write back flag.                                */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void lfs_write_back(int partition);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      lfs_erase_block                                                  */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function erase LFS block.                                    */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void lfs_erase_block(UINT32 block);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      lfs_erase_partition                                              */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function erase all LFS partition.                            */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void lfs_erase_partition(void);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      lfs_alloc_block_cache                                            */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function alloc LFS cache memory.                            */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
lfsMsgQ * lfs_alloc_block_cache(UINT32 blocknum, int partition);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      lfs_get_block_cache                                              */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function Go through all the entries and check if the block   */
/*      is cached or not. If cached, Return the cached buffer.           */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
lfsMsgQ * lfs_get_block_cache(UINT32 blocknum, int partition);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      lfs_read_block_to_buf                                            */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function read the block data from flash to cached buffer.    */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
lfsMsgQ * lfs_read_block_to_buf(UINT32 blocknum, int partition);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      lfs_write_back                                                  */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function write cached buffer back to flash.                  */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
BOOL lfs_write_back_to_flash(BOOL lock, BOOL all);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      lfs_flash_cache_read                                             */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function do cache read operation.                            */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
BOOL lfs_flash_cache_read(UINT32 flashaddress, UINT8 *Buf, UINT32 size, int partition);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      lfs_flash_cache_write                                            */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function do cache write operation.                           */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
BOOL lfs_flash_cache_write(UINT32 flashaddress, UINT8 *Buf, UINT32 size, int partition);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      lfs_cache_task                                                   */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The task write lfs cached buffer back to flash.                  */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void lfs_cache_task(void *argv);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      lfs_cache_task_suspend                                           */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function do lfs cache task suspend operation.                */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void lfs_cache_task_suspend(void);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      lfs_cache_task_resume                                            */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function do lfs cache task resume operation.                 */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void lfs_cache_task_resume(void);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      LfsMsgQValid                                                     */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function check the validity of LFS queque and return         */
/*      the status.                                                      */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      mvUsbNetQ                           usb net response queue       */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      TRUE                                Valid resonse queue          */
/*      FALSE                               Invalid resonse queue        */
/*                                                                       */
/*************************************************************************/
BOOL LfsMsgQValid(lfsMsgQ *queue);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      LfsCacheLock                                                     */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function do lfs cache lock.                                  */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void LfsCacheLock(int partition);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      LfsCacheUnlock                                                   */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function do lfs cache unlock.                                */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void LfsCacheUnlock(int partition);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      LfsMsgQInit                                                      */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function initializes lfs queue. It should be called         */
/*      on behalf of a queue prior to using the queue.                   */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      mvUsbNetQ                           Response head pointer        */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void LfsMsgQInit(lfsMsgQ *hdr);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      LfsMsgQPut                                                       */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function places a data block at the tail of a lfs queue on  */
/*      behalf of a queue prior to using the queue.                      */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      hdr                                 The queue head pointer       */
/*      pRspQ                               The queue pointer            */
/*      len                                 the data length              */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void LfsMsgQPut(lfsMsgQ *hdr, lfsMsgQ *queue);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      LfsMsgQGet                                                       */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function removes the data block at head of a queue and      */
/*      returns a pointer to the data block. If the queue is empty, then */
/*      a NULL pointer is returned. queue on behalf of a queue prior to  */
/*      using the queue.                                                 */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      hdr                                 The queue head pointer       */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      pRspQ                               The returned queue pointer   */
/*                                                                       */
/*************************************************************************/
lfsMsgQ *LfsMsgQGet(lfsMsgQ *hdr);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      LfsMsgQRemove                                                    */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function removes the data block at head of a queue and      */
/*      returns a pointer to the data block. If the queue is empty, then */
/*      a NULL pointer is returned. queue on behalf of a queue prior to  */
/*      using the queue.                                                 */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      hdr                                 The queue head pointer       */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      pRspQ                               The returned queue pointer   */
/*                                                                       */
/*************************************************************************/
void LfsMsgQRemove(lfsMsgQ *hdr, lfsMsgQ *queue);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      LfsGetMsgQCnt                                                    */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function Get the LFS queque count.                          */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      mvUsbNetQ                           Response head pointer        */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
UINT32 LfsGetMsgQCnt(int partition);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      Lfs_set_block_flag                                               */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function set/clear block erase flag.                         */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void Lfs_set_block_flag(UINT32 block, BOOL alloc, int partition);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      Lfs_block_preerase                                               */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function pre-erase lfs block.                                */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void Lfs_block_preerase(void);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      LfsMutexInit                                                     */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function initialize lfs mutex.                               */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void LfsMutexInit(int partition);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      LfsMutexLock                                                     */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function do lfs mutex lock.                                  */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void LfsMutexLock(BOOL lock, int partition);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      LfsMutexUnlock                                                   */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function do lfs mutex unlock.                                */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void LfsMutexUnlock(BOOL lock, int partition);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      Lfs_record_corrupted_files                                       */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function record corrupted files and add it into file queue.  */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void Lfs_record_corrupted_files(lfs_t *lfs, lfs_mdir_t *dir, UINT16 id,
                int (*cb)(lfs_t *, lfs_mdir_t *, uint16_t, struct lfs_info *));

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      lfs_remove_corrupted_files                                       */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function precheck corrupted files and delete.                */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void lfs_remove_corrupted_files(lfs_t *lfs);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      lfs_write_back_to_partition                                      */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function write cached buffer back to partition.              */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
BOOL lfs_write_back_to_partition(BOOL lock, BOOL all, int partition);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      LfsMutexLockALL                                                  */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function do lfs mutex lock & FDI lock.                       */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void LfsMutexLockALL(void);
#endif /* LFS_CACHE_H */
