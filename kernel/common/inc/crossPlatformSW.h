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

#if !defined(_CROSS_PLAT_SW_H_)
#define      _CROSS_PLAT_SW_H_

#include "global_types.h"

#if (defined FEATURE_ACNVM)
    #if (defined FLAVOR_APP)
        #include "nvm_server.h"
    #endif
    #if (defined FLAVOR_COM)
    #if !defined(CRANE_MCU_DONGLE)
        #include "nvmClient.h"
    #endif
    #endif
#endif

#ifdef RTC_TUNNEL_ENABLE
    #include "rtct_shared.h"
#endif

#include "bsp.h"
#ifndef CRANE_MCU_DONGLE
#include "FDI2FS_API.h"
#else
#include "FDI_TYPE.h"
#include "FDI_FILE.h"
#endif

#include "FDI_ERR.h"
#include "fat32_utils.h"
#ifndef CRANE_SD_NOT_SUPPORT
#include "ff.h"
#endif
/*----------- Local function definition -------------------------------------*/
void crossPlatformSWPhase1Init(void);
void crossPlatformSWPhase2Init(void);
void crossPlatformSWPhase3Init(void);
void crossPlatformSWPhaseAfterAllNvmInit(void);
void enable_nvm_flush(void);
void disable_nvm_flush(void);
void ffst_open_status(unsigned int handle);
void ffst_close_status(unsigned int handle);
void ffst_write_start_status(unsigned int handle);
void ffst_write_end_status(unsigned int handle);
void ffst_read_start_status(unsigned int handle);
void ffst_read_end_status(unsigned int handle);
void ffst_remove_start_status(char *str);
void ffst_remove_end_status(char *str);
void ffst_close_all_file(void);


/***********************************************************************
 *
 * Name:        FDI_IsFormatted
 *
 * Description: check whether whole file system is formatted.
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  		TRUE: 		Formatted
 *		FALSE: 		not formatted
 *
 * Notes:
 *
 ***********************************************************************/
BOOL FDI_IsFormatted(void);

/***********************************************************************
 *
 * Name:        FDI_Format
 *
 * Description: format whole file system.
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  		ERR_CODE
 *
 * Notes:
 *
 ***********************************************************************/
ERR_CODE FDI_Format(void);


/***********************************************************************
 *
 * Name:		FDI_fopen
 *
 * Description: used in an implementation-specified matter to open or create a file and
*    associate it with a stream.
 *
 * Parameters:
 *  char                 *filename_ptr      [IN]    const character string for file name specifier.
*  char			*mode                [IN] 	const character string for type specification;
 *
 * Returns:
 *	the stream index used for the opened file.
 *    If an error is detected, FDI_fopen returns 0.
 *
 * Notes:
 *
 ***********************************************************************/
FILE_ID FDI_fopen(const char *filename_ptr, const char *mode);


/***********************************************************************
 *
 * Name:		FDI_fclose
 *
 * Description: used to close a file stream.
 *
 * Parameters:
 *  FILE_ID                 stream      [IN]    stream index for the file to be closed.
 *
 * Returns:
 *		0:   pass
 * 	 	EOF: fail
 * Notes:
 *
 ***********************************************************************/
int FDI_fclose(FILE_ID stream);


/***********************************************************************
 *
 * Name:		FDI_fread
 *
 * Description: read file.
 *
 * Parameters:
 *	 void		*buffer_ptr   	[OUT]	pointer to buffer to place data read
 *	size_t		element_size 	[IN] 		size of element referenced by buffer pointer
 *	size_t		count    		[IN]		number of elements to be read
 *	FILE_ID 		stream		[IN]		stream index for the file to be closed.
 *
 * Returns:
 *		number of elements succesfully read
 * Notes:
 *
 ***********************************************************************/
size_t FDI_fread(void *buffer_ptr, size_t element_size, size_t count, FILE_ID stream);



/***********************************************************************
 *
 * Name:		FDI_fwrite
 *
 * Description: write file.
 *
 * Parameters:
 *	 void		*buffer_ptr   	[IN]		pointer to buffer to be written
 *	size_t		element_size 	[IN] 		size of element referenced by buffer pointer
 *	size_t		count    		[IN]		number of elements to be read
 *	FILE_ID 		stream		[IN]		stream index for the file to be closed.
 *
 * Returns:
 *		number of elements succesfully write
 * Notes:
 *
 ***********************************************************************/
size_t FDI_fwrite(const void *buffer_ptr,size_t element_size,size_t count,FILE_ID stream);



/***********************************************************************
 *
 * Name:		FDI_fseek
 *
 * Description: sets the file position indicator of the file specified  by stream.
 *				The new position, measured in bytes from the beginning of the file
 *				is obtained by adding offset to the position specified by wherefrom.
 *
 * Parameters:
 *    FILE_ID                 stream      [IN]    stream index for the file
 *	long				offset 	[IN] 		the offset to obtain to the position indicator (in bytes)
 *	int				wherefrom    		[IN]		the position to add offset(SEEK_SET, SEEK_CUR, SEEK_END)

 *
 * Returns:
  *		0:  pass
 * 	 	EOF: fail
 * Notes:
 *
 ***********************************************************************/
int FDI_fseek(FILE_ID stream, long offset, int wherefrom);


/***********************************************************************
 *
 * Name:		FDI_remove
 *
 * Description:  remove the file which is compatible to the file name from the existing  files list
 *
 * Parameters:
 *  char                 *filename_ptr      [IN]    const character string for file name specifier.
 *
 * Returns:
 *		0:  pass
 * 	 	EOF: fail
 * Notes:
 *
 ***********************************************************************/
int FDI_remove(const char *filename_ptr);




/***********************************************************************
 *
 * Name:		FDI_GetFileSize
 *
 * Description:  get file size
 *
 * Parameters:
 *  int                 fd      [IN]    file handler
 *
 * Returns:
 * 	 	file length
 * Notes:
 *
 ***********************************************************************/
unsigned int FDI_GetFileSize(int fd);







/***********************************************************************
 *
 * Name:        FDI_findfirst
 *
 * Description: begins a search for files specified wildcards..
 *
 * Parameters:
 *  char                 *filename_ptr      [IN]    const character string for file name specifier.
 *  FILE_INFO		*fileinfo_ptr	     [OUT]  pointer to type FILE_INFO structure filled with located file information
 *
 * Returns:
 *  		0: found
 * 	 	EOF: not found
 *
 * Notes:
 *
 ***********************************************************************/
int FDI_findfirst(const char *filename_ptr, FILE_INFO *fileinfo_ptr);


/***********************************************************************
 *
 * Name:		FDI_findnext
 *
 * Description: begins a search for files specified wildcards..
 *
 * Parameters:
 *	FILE_INFO		*fileinfo_ptr		 [OUT]	pointer to type FILE_INFO structure filled with  file information
 *
 * Returns:
 *			0: found
 *		EOF: not found
 *
 * Notes:
 *
 ***********************************************************************/

int FDI_findnext( FILE_INFO *fileinfo_ptr);

/***********************************************************************
 *
 * Name:		FDI_eof
 *
 * Description: Check whether current pos is at EOF.
 *
 * Parameters:
 *  FILE_ID                 stream      [IN]    stream index for the file to be closed.
 *
 * Returns:
 *		1:  EOF
 *		0: in file
 *	others: error
 *
 * Notes:
 *
 ***********************************************************************/
int FDI_eof(FILE_ID stream);

int FDI_rename(const FDI_TCHAR *oldname_ptr, const FDI_TCHAR *newname_ptr);

void FDI_Factory_Reset(void);

//#ifndef CRANE_SD_NOT_SUPPORT
/*
Summary:    FAT_IsFormatted
Parameters: none
Return:     TRUE  the sdcard have been formatted
            FALSE the sdcard haven't been formatted

*/
BOOL FAT_IsFormatted(char *vol);

/*
*Summary:   FAT_Format
*Paramters: none
*Return:    ERR_NONE  format ok
            others    format err
*/
	ERR_CODE FAT_Format(char *vol);


/*
*Summary:    FAT_fopen
*Parameters: filename_ptr  the argument is a ascii string
             mode the argument can be 'r' 'w' 'c' 'b'
*Return:     0 open success
             others open err

*/
unsigned int FAT_fopen(const char *filename_ptr, const char *mode);

/*
*Summary:    FAT_fclose
*Parameters: stream is the file handle
*Return:     0 ok
             -1 err
*/
int FAT_fclose(unsigned int stream);

/*
*Summary:    FAT_fread
*Parameters: buffer_ptr   the buf pointer
             element_size the block buf size
             count        the count of block buf
             stream       the file handle
 *Return:    the count of block buf that have been read success
*/
size_t FAT_fread(void *buffer_ptr, size_t element_size, size_t count, unsigned int stream);

/*
*Summary:    FAT_fwrite
*Parameters: buffer_ptr   the buf pointer
             element_size the block buf size
             count        the count of block buf
             stream       the file handle
 *Return:    the count of block buf that have been written success
*/
size_t FAT_fwrite(const void *buffer_ptr,size_t element_size,size_t count,unsigned int stream);

/*
*Summary:    FAT_fseek
*Parameters: stream   the file handle
             offset   move size from the start address( the start address is configured by whereform arg)
             whereform the argument can be FS_SEEK_BEGIN,FS_SEEK_CURREN,FS_SEEK_END
 *Return:    0 success
             others fail
*/
int FAT_fseek(unsigned int stream, long offset, int wherefrom);

/*
*Summary:    FAT_remove
*Parameters: filename_ptr   the file name, a ascii string

 *Return:    0 success
             -1 fail
*/
int FAT_remove(const char *filename_ptr);

/*
*Summary:    FAT_findfirst
*Parameters: filename_ptr   the file name, a ascii string
             fileinfo_ptr   a pointer, must be alloced mem first
 *Return:    0 fail
             others success
*/
unsigned int FAT_findfirst(const char *filename_ptr, LPFAT32FILEINFO fileinfo_ptr);

/*
*Summary:    FAT_findnext
*Parameters: handle  the Return value of FAT_findfirst
             fileinfo_ptr   the pointer that used by FAT_findnext
 *Return:    0 fail
             others success
*/
int FAT_findnext(unsigned int handle, LPFAT32FILEINFO fileinfo_ptr);

/*
*Summary:    FAT_findfileclose
*Parameters: filename_ptr   the file name, a ascii string
             fileinfo_ptr   a pointer, must be alloced mem first
 *Return:    0 fail
             others success
*/
BOOL FAT_findfileclose(unsigned int handle);

/*
*Summary:    FAT_rename
*Parameters: oldname_ptr  the old name ascii string
             newname_ptr  the new name ascii string
 *Return:    0 success
             others fail
*/
int FAT_rename(const char *oldname_ptr, const char *newname_ptr);

/*
*Summary:    FAT_eof
*Parameters: stream the file handle
 *Return:    0  it is the end
             -1 it is not the end
*/
int FAT_eof(unsigned int stream);

/*
*Summary:    FAT_filesize
*Parameters: stream the file handle
*Return:     0  the file  handle is err, or the filesize is zero
             others the len of the file
*/
unsigned int FAT_GetFileSize(unsigned int stream);
typedef struct _FS_INFO
{
    long long iToTalSize;
	long long iUsedSize;
}FS_INFO;

/*
*Summary:    FAT_MakeDir
*Parameters: dir_name the direcotry name
             mode     now, this value can be NULL
 *Return:    0  create dir ok
             -1 create dir err
*/
int FAT_MakeDir(char *dir_name, unsigned int mode);
/*
*Summary:    FAT_MakeDir_M
*Parameters: dir_name the direcotry name
             mode     now, this value can be NULL
 *Return:    0  create dir ok
                 others create dir err
*/
int FAT_MakeDir_M(char *dir_name, unsigned int mode);

/*
*Summary:    FAT_RemoveDir
*Parameters: dir_name the direcotry name
 *Return:    0  delete ok
             -1 delete  err
*/
int FAT_RemoveDir(char *dir_name);

/*
*Summary:    FAT_SetCurrentDir
*Parameters: dir_name the direcotry name
 *Return:    0  set ok
             -1 set err
*/
int FAT_SetCurrentDir(char *dir_name);

/*
*Summary:    FAT_GetCurrentDir
*Parameters: dir_name  the buffer used to save the directory name(the full path)
             max_len   the buffer len
 *Return:    0  get name ok
             -1 get name err
*/
int FAT_GetCurrentDir(char *dir_name, unsigned int max_len);


/*
*Summary:    FAT_GetFSInfo
*Parameters: pszDevName the volume name
             psFSInfo   a buffer used for saving the len
*Return:     0  ok
             -1 err
*/
int FAT_GetFSInfo(char *pszDevName, FS_INFO* psFSInfo);

UINT32 FAT_Getfree(char *path);
//#endif

//WARNING:
//This API would erase all flash , be carefully use
extern void NVMFlashErase(void);
extern char* DUMP_VOL;

#define SULOG_DIR_NAME "Sulog"
#endif

