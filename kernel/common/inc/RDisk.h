#ifndef	RDISKFSYS_H
#define RDISKFSYS_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                Rdiskfsys.h


GENERAL DESCRIPTION

    This file is for RAM disk file system.

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
11/26/08   zhoujin    Created module
===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include <stdlib.h>
#include "string.h"
#include "osa.h"
#include "global_types.h"

#define RAM_DISK_FILE_SYSTEM

#ifdef RAM_DISK_FILE_SYSTEM
/*===========================================================================

            LOCAL MACRO
===========================================================================*/
//#define RDISK_DEBUG

/*
 * Description:
 * Maximum number of open files
 *
 * Remarks:
 * Each handle takes 36 bytes
 *
 */
#define RDISK_DELETED				'\xE5'

#if (defined(LTEONLY_THIN) || defined(LTEGSM_THIN))
#define RDISK_NUM_HANDLES			8
#else
#define RDISK_NUM_HANDLES			32
#endif

#define RDISK_FILE_NAME_SIZE        128
#define RDISK_RW_FILE_SIZE          (8*1024)

/* --------------------------------------------------------------------
** Type declarations.
** -------------------------------------------------------------------
*/
typedef unsigned int time_t;

/* --------------------------------------------------------------------
** macro declarations.
** -------------------------------------------------------------------
*/
#define TRUE         			1
#define FALSE       			0

/* --------------------------------------------------------------------
** Type declarations for RAM Disk file handle.
** -------------------------------------------------------------------
*/

typedef struct
{
	UINT32	            StartAddress;
	UINT32	            Size;
	UINT32	            FilePos;
	char                filename[RDISK_FILE_NAME_SIZE];
}Rdisk_file_sHandle;

/* --------------------------------------------------------------------
** Type declarations for RAM Disk stat.
** -------------------------------------------------------------------
*/

typedef struct
{
	UINT32		        st_size;	/* File size in bytes */
	time_t				st_time;	/* Timestamp of last modification */
}
Rdisk_sStat;

/*===========================================================================

FUNCTION Rdisk_FindHandle

DESCRIPTION
  Find the destination file handle in rdisk file table.

DEPENDENCIES
  none

RETURN VALUE
  return staus

SIDE EFFECTS
  none

===========================================================================*/

int Rdisk_FindHandle(const char *name);

/****************************************************************************
* Functions: Rdisk_fopen
*
* DESCRIPTION:
*    The function Rdisk_fopen takes as arguments a file name, it is specified as a
*    character string. The file name is used in an implementation-specified matter
*    to open or create a file and associate it with a stream.
*    Returns the StreamInfoTable index used for the opened file.
*
* USAGE:
*    UINT32 Rdisk_fopen(const char *filename)
*
* PARAMETERS:
*
* INPUTS:
*  filename_ptr         const character string for file name
*
* OUTPUTS:
*
* RETURNS:
*  Returns the StreamInfoTable index used for the opened file.
*  If an error is detected, FDI_fopen returns 0.
*
* History:
* 20.05.05  Dmitry P. Initial version. Originaly described in FDI_FILE.c:260
*
****************************************************************************/

UINT32 Rdisk_fopen(const char *filename);

/*===========================================================================

FUNCTION RdiskSemaphoreInit

DESCRIPTION
  initialize the semaphore for ram disk file system.

DEPENDENCIES
  none

RETURN VALUE
  return staus

SIDE EFFECTS
  none

===========================================================================*/

void RdiskSemaphoreInit(void);

/*****************************************************************************
* Function Name: Rdisk_fclose()
*
* DESCRIPTION:
*    None.
*
* USAGE:
*    int Rdisk_fclose(UINT32 stream)
*
* PARAMETERS:
*
* INPUTS:
*  filename_ptr  const character null terminated string of the filename
*                to check
*
* OUTPUTS:
*
* RETURNS:
*  Returns TRUE if filename is valid; otherwise, it returns FALSE.
*
* HISTORY:
* 20.05.05 dmp Took from FDI5 from FDI_FILE.c:3875; and simplifyed.
*
***************************************************************************/
int Rdisk_fclose(UINT32 stream);

/*****************************************************************************
* Function Name: Rdisk_fread()
*
* DESCRIPTION:
*    None.
*
* USAGE:
*    size_t FDI_fread(void *buff, size_t element_size, size_t count, FILE_ID stream)
*
* PARAMETERS:
*
* INPUTS:
*  filename_ptr  const character null terminated string of the filename
*                to check
*
* OUTPUTS:
*
* RETURNS:
*  Returns TRUE if filename is valid; otherwise, it returns FALSE.
*
* HISTORY:
* 20.05.05 dmp Took from FDI5 from FDI_FILE.c:3875; and simplifyed.
*
***************************************************************************/
UINT32 Rdisk_fread(char *buff, UINT32 element_size, UINT32 count, UINT32 stream);

/*****************************************************************************
* Function Name: Rdisk_fwrite()
*
* DESCRIPTION:
*    None.
*
* USAGE:
*    UINT32 Rdisk_fwrite(const void *buff, UINT32 element_size, UINT32 count, UINT32 stream)
*
* PARAMETERS:
*
* INPUTS:
*  filename_ptr  const character null terminated string of the filename
*                to check
*
* OUTPUTS:
*
* RETURNS:
*  Returns TRUE if filename is valid; otherwise, it returns FALSE.
*
* HISTORY:
* 20.05.05 dmp Took from FDI5 from FDI_FILE.c:3875; and simplifyed.
*
***************************************************************************/

UINT32 Rdisk_fwrite(const char *buff, UINT32 element_size, UINT32 count, UINT32 stream);

/*****************************************************************************
* Function Name: Rdisk_stat()
*
* DESCRIPTION:
*    None.
*
* USAGE:
*    int Rdisk_stat(const char *file_name, Rdisk_sStat *stat)
*
* PARAMETERS:
*
* INPUTS:
*  filename_ptr  const character null terminated string of the filename
*                to check
*
* OUTPUTS:
*
* RETURNS:
*  Returns TRUE if filename is valid; otherwise, it returns FALSE.
*
* HISTORY:
* 20.05.05 dmp Took from FDI5 from FDI_FILE.c:3875; and simplifyed.
*
***************************************************************************/
int Rdisk_stat(const char *file_name, Rdisk_sStat *stat);

/*****************************************************************************
* Function Name: Rdisk_remove()
*
* DESCRIPTION:
*    None.
*
* USAGE:
*    int Rdisk_remove(const char *filename_ptr)
*
* PARAMETERS:
*
* INPUTS:
*  filename_ptr  const character null terminated string of the filename
*                to check
*
* OUTPUTS:
*
* RETURNS:
*  Returns TRUE if filename is valid; otherwise, it returns FALSE.
*
* HISTORY:
* 20.05.05 dmp Took from FDI5 from FDI_FILE.c:3875; and simplifyed.
*
***************************************************************************/
int Rdisk_remove(const char *filename_ptr);

#endif

#endif /* RDISKFSYS_H */
