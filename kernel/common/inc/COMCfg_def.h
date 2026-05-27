/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

/*********************************************************************
*                      M O D U L E     B O D Y                       *
**********************************************************************
*                                                                    *
* Title:  Definiton Header File for the COMCfg package				 *
*                                                                    *
* 		  This file holds internal (non-public) definitions needed   *
*         by both the main c file COMCfg.c and the COMCfg_parser.c   *
*         that implement the parser and is to be used also on 		 *
*         PC platforms for validating COMCfg.csv files               *
*                                                                    *
* Author: Idan Hemdat                                                *
* Target, subsystem: Common Platform, HAL                            *
*                                                                    *
* Notes: 	ATTENTION!                                               *
* For portablility this file must stay dependant on COMCfg_def.h and *
* c standard libs/files only!!!										 *
*															         *
*                                                                    *
*********************************************************************/

#ifndef _COMCFG_DEF_H_
#define _COMCFG_DEF_H_

/* Includes */
//Don't include any non c standard file/lib !!!
/***********/

/* One of the defined checked here must be defined BEFORE including this file for debug prints interface*/
#if defined COMCFG_RTOS_SW
	//#include "diag.h" //Include this in the c file to keep this file clean from non-standard headers
	/* DIAG_FILTER & diagPrintf already definef*/
	#define DEFINE_COMCFG_PRINTS
#elif defined COMCFG_ANSI_C_SW
	#include <stdio.h>
	#define DIAG_FILTER(a,b,c,d) {printf("\n");}
	#define diagPrintf  printf
	#define DEFINE_COMCFG_PRINTS
#else
	#if defined DEFINE_COMCFG_PRINTS
	#undef DEFINE_COMCFG_PRINTS
	#endif
#endif

#if defined DEFINE_COMCFG_PRINTS
	#define		COMCFG_READ_OK_PRINT		{																											\
														DIAG_FILTER(SW_PLAT, COMCfg, ReadOK, DIAG_INFORMATION)                                                 \
	        											diagPrintf ("%s file read OK.", COM_CONFIG_FILE_NAME);      \
														DIAG_FILTER(SW_PLAT, COMCfg, STATUS, DIAG_INFORMATION)                                                 \
	        											diagPrintf ("COMCfg - File size: %dB Line parsed: %d Parameter parsed: %d"    \
																		, comcfgDB.file_length, comcfgDB.line_num, comcfgDB.param_num);      \
	}
	#define		COMCFG_MAX_PARAM_PRINT		{																											\
														DIAG_FILTER(SW_PLAT, COMCfg_ERROR, MAX_PARAM1, DIAG_INFORMATION)								\
														diagPrintf ("ERROR - * Maximal parameters number is %d, Beyond that only comment (,#) lines are allowed! * - ERROR", COMCFG_MAX_PARAMETERS); \
														DIAG_FILTER(SW_PLAT, COMCfg_ERROR, MAX_PARAM2, DIAG_INFORMATION)                                                 \
	    												diagPrintf ("ERROR - * Copy a valid file to COM NVM through flash explorer and wait * - ERROR");         \
	}
	#define		COMCFG_FILE_ACCESS_ERROR_PRINT	{																											\
														DIAG_FILTER(SW_PLAT, COMCfg_ERROR, FILE_ACCESS1, DIAG_INFORMATION)     		\
														diagPrintf ("ERROR - * Failed Open/Read file operation!!! * - ERROR");       \
														DIAG_FILTER(SW_PLAT, COMCfg_ERROR, FILE_ACCESS2, DIAG_INFORMATION)                                                 \
	    												diagPrintf ("ERROR - * Copy a valid file to COM NVM through flash explorer and wait * - ERROR");         \
	}
	#define		COMCFG_FILE_LENGTH_ERROR_PRINT	{																											\
														DIAG_FILTER(SW_PLAT, COMCfg_ERROR, FILE_LENGTH1, DIAG_INFORMATION)			\
														diagPrintf ("ERROR - * %s, File exceed maximal length (%d bytes) * - ERROR", COM_CONFIG_FILE_NAME, COMCFG_MAX_FILE_LENGTH); \
														DIAG_FILTER(SW_PLAT, COMCfg_ERROR, FILE_LENGTH2, DIAG_INFORMATION)                                                 \
	    												diagPrintf ("ERROR - * Copy a valid file to COM NVM through flash explorer and wait * - ERROR");         \
	}
	#define		COMCFG_LOWER_CASE_ERROR_PRINT	{																											\
														DIAG_FILTER(SW_PLAT, COMCfg_ERROR, LOWER_CASE1, DIAG_INFORMATION)  \
														diagPrintf ("ERROR - * %s, line %d, Lower case in NAME,VALUE columns is not allowed * - ERROR", COM_CONFIG_FILE_NAME, comcfgDB.line_num);  \
														DIAG_FILTER(SW_PLAT, COMCfg_ERROR, LOWER_CASE2, DIAG_INFORMATION)                                                 \
	    												diagPrintf ("ERROR - * Copy a valid file to COM NVM through flash explorer and wait * - ERROR");         \
	}

	#define		COMCFG_MEM_ERROR_PRINT			{																											\
														DIAG_FILTER(SW_PLAT, COMCfg_ERROR, MEM1, DIAG_INFORMATION)             \
														diagPrintf ("ERROR - * COMCfg failed in memory allocation * - ERROR");  \
														DIAG_FILTER(SW_PLAT, COMCfg_ERROR, MEM2, DIAG_INFORMATION)                                                 \
	    												diagPrintf ("ERROR - * Copy a valid file to COM NVM through flash explorer and wait * - ERROR");         \
	}
	#define		COMCFG_ILLEGAL_CHAR_ERROR_PRINT		{                                                                                                              \
														DIAG_FILTER(SW_PLAT, COMCfg_ERROR, ILLEGAL_CHAR1, DIAG_INFORMATION)                                                 \
	        											diagPrintf ("ERROR - * %s, line %d, Illegal char in NAME,VALUE columns * - ERROR", COM_CONFIG_FILE_NAME, comcfgDB.line_num);      \
														DIAG_FILTER(SW_PLAT, COMCfg_ERROR, ILLEGAL_CHAR2, DIAG_INFORMATION)                                                 \
	    												diagPrintf ("ERROR - * Copy a valid file to COM NVM through flash explorer and wait * - ERROR");         \
	}
	#define		COMCFG_UNEXPECTED_NEWLINE_ERROR_PRINT {                                                                                                        \
														DIAG_FILTER(SW_PLAT, COMCfg_ERROR, UNEXPECTED_NEWLINE1, DIAG_INFORMATION)                                                 \
	        											diagPrintf ("ERROR - * %s, line %d, Unexpected end-of-line * - ERROR", COM_CONFIG_FILE_NAME, comcfgDB.line_num);      \
														DIAG_FILTER(SW_PLAT, COMCfg_ERROR, UNEXPECTED_NEWLINE2, DIAG_INFORMATION)                                                 \
	    												diagPrintf ("ERROR - * Copy a valid file to COM NVM through flash explorer and wait * - ERROR");         \
	}
	#define		COMCFG_MISSING_VALUE_ERROR_PRINT	{                                                                                                        \
														DIAG_FILTER(SW_PLAT, COMCfg_ERROR, MISSING_VALUE1, DIAG_INFORMATION)                                                 \
	        											diagPrintf ("ERROR - * %s, line %d, Missing Value * - ERROR", COM_CONFIG_FILE_NAME, comcfgDB.line_num);      \
														DIAG_FILTER(SW_PLAT, COMCfg_ERROR, MISSING_VALUE2, DIAG_INFORMATION)                                                 \
	    												diagPrintf ("ERROR - * Copy a valid file to COM NVM through flash explorer and wait * - ERROR");         \
	}
	#define		COMCFG_ILLEGAL_BLANKS_ERROR_PRINT	{                                                                                                        \
														DIAG_FILTER(SW_PLAT, COMCfg_ERROR, ILLEGAL_BLANKS1, DIAG_INFORMATION)                                                 \
	        											diagPrintf ("ERROR - * %s, line %d, Illegal blanks in NAME,VALUE columns * - ERROR", COM_CONFIG_FILE_NAME, comcfgDB.line_num);      \
														DIAG_FILTER(SW_PLAT, COMCfg_ERROR, ILLEGAL_BLANKS2, DIAG_INFORMATION)                                                 \
	    												diagPrintf ("ERROR - * Copy a valid file to COM NVM through flash explorer and wait * - ERROR");         \
	}
	#define		COMCFG_MAX_COLUMNS_ERROR_PRINT		{                                                                                                        \
														DIAG_FILTER(SW_PLAT, COMCfg_ERROR, MAX_COLUMNS1, DIAG_INFORMATION)                                                 \
	        											diagPrintf ("ERROR - * %s, line %d, missing or extra columns that are not comment (,#) are not allowed * - ERROR", COM_CONFIG_FILE_NAME, comcfgDB.line_num);      \
														DIAG_FILTER(SW_PLAT, COMCfg_ERROR, MAX_COLUMNS2, DIAG_INFORMATION)                                                 \
	    												diagPrintf ("ERROR - * Copy a valid file to COM NVM through flash explorer and wait * - ERROR");         \
	}
#endif //DEFINE_COMCFG_PRINTS

#define 	COMCFG_MAX_PARAMETERS			128
#define 	COMCFG_MAX_FILE_LENGTH          0x2000 //8KB
#define 	COMCFG_CHUNK_READ				0x400  //1KB //to prevent warning when reading since file length is unknown
#define 	COM_CONFIG_FILE_NAME	  			"COMCfg.csv"

//this struct holds information for one entry in the file
typedef struct
{
	char 			*name;
	char 			*value;
} COMCfg_Param_Struct;

//this struct holds all information for the file
typedef struct
{
	COMCfg_Param_Struct		*parameters;
	int						parameters_size; //+1 from max parameters allowed to detect parameter overflow
	char					*file_buff;
	int						file_length;
	int						param_num;
	int						line_num;
} COMCfg_File_Struct;

typedef enum
{
	COMCFG_OK=0,
	COMCFG_MAX_PARAM_ERROR,
	COMCFG_FILE_ACCESS_ERROR,
	COMCFG_FILE_LENGTH_ERROR,
	COMCFG_LOWER_CASE_ERROR,
	COMCFG_ILLEGAL_CHAR_ERROR,
	COMCFG_UNEXPECTED_NEWLINE_ERROR,
	COMCFG_MISSING_VALUE_ERROR,
	COMCFG_ILLEGAL_BLANKS_ERROR,
	COMCFG_MAX_COLUMNS_ERROR,
	COMCFG_MEM_ERROR,
}read_COMCfg_RC;

read_COMCfg_RC parse_COMCfg (COMCfg_File_Struct *comcfgDB);

#endif //_COMCFG_DEF_H_
