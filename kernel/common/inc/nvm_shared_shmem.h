/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

#ifndef _NVM_SHARED_SHMEM
#define _NVM_SHARED_SHMEM

#include    "nvm_shared.h"

#if (defined OSA_NUCLEUS)
    #include "csw_mem.h"
#else
    #define     SIZEOF_CACHE_LINE       32
    #define     CACHE_LINE_MASK         (SIZEOF_CACHE_LINE-1)
#endif

#define     ROUND_UP_TO_FULL_CACHE_LINES(sIZE)      (((sIZE) + CACHE_LINE_MASK) & ~CACHE_LINE_MASK)

/*
File Name : nvm_shared_shmem.h
Desc	  : Shared file of NVM Server-Client SW Components, define Protocal Messages
Both SW Entities should use this file
*/

typedef struct
{
    NVM_OP_CODE     op_code ;
    NVM_STATUS_T    error_code ;    //  error code of the requested operation
} NVM_HeaderS ;

typedef struct
{
    NVM_HeaderS     Header ;        //  Must be first.
    CHAR            szFileName[NVM_FILE_NAME_MAX_LENGHT];
    CHAR            szFileAttributes[NVM_FILE_ATTRIBUTES_MAX_LENGHT];
    UINT32          hFile ;
}NVM_FileOpenS ;

typedef struct
{
    NVM_HeaderS     Header ;        //  Must be first.
    UINT32          hFile ;
}NVM_FileCloseS ;

typedef struct
{
    NVM_HeaderS         Header ;    //  Must be first.
    UINT32              hFile ;
    UINT32              nOffset;    // offset relative to origin indicator
    NVM_SEEK_ORIGIN     nOrigin;    // where to seek from
}NVM_FileSeekS ;

typedef struct
{
    NVM_HeaderS     Header ;        //  Must be first.
    CHAR            szFileName[NVM_FILE_NAME_MAX_LENGHT];
}NVM_FileRemoveS ;

typedef struct
{
    NVM_HeaderS     Header ;        //  Must be first.
    NVM_FILE_INFO   fileParms ;
}NVM_FileFindS ;

typedef struct
{
    NVM_HeaderS     Header ;                    //  Must be first.
    UINT32          hFile;
    UINT32          nNumberOfBytes ;
    UINT32          nNumberActualBytes ;
    UINT8           *DataBuffer ;
}NVM_FileIOS ;

#endif  //  _NVM_SHARED_SHMEM
