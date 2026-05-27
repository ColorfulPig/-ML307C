#ifndef __DMP_H__
#define __DMP_H__

#include <stdlib.h>
#include <stdio.h>
#include "cm_dmp.h"

#define CMDMP_GENERAL_STRING_MAX_LEN      16
#define CMDMP_APPINFO_STRING_MAX_LEN      128

typedef struct{
    int enable;
    int interval;
    char appkey[CMDMP_GENERAL_STRING_MAX_LEN];//M100000052
    char pswd[3*CMDMP_GENERAL_STRING_MAX_LEN];
    int tmltype;
    char host[3*CMDMP_GENERAL_STRING_MAX_LEN];
    int port;
    char appinfo[3*CMDMP_GENERAL_STRING_MAX_LEN];
    char mac[3*CMDMP_GENERAL_STRING_MAX_LEN];
    char cpu[CMDMP_GENERAL_STRING_MAX_LEN];
    char rom[8];
    char ram[8];
    char osver[CMDMP_GENERAL_STRING_MAX_LEN];
    char swver[CMDMP_GENERAL_STRING_MAX_LEN];
    char swname[CMDMP_GENERAL_STRING_MAX_LEN];
    char volte[8];
    char nettype[CMDMP_GENERAL_STRING_MAX_LEN];
    char account[CMDMP_GENERAL_STRING_MAX_LEN];
    char phonenum[CMDMP_GENERAL_STRING_MAX_LEN];
    char location[CMDMP_GENERAL_STRING_MAX_LEN];
#ifdef DM40_SUPPORT
	char brand[CMDMP_GENERAL_STRING_MAX_LEN];
	char model[32];
	char tmplID[CMDMP_GENERAL_STRING_MAX_LEN];
	char BatCap[CMDMP_GENERAL_STRING_MAX_LEN];
	char scrnSz[CMDMP_GENERAL_STRING_MAX_LEN];
	char rtMac[3*CMDMP_GENERAL_STRING_MAX_LEN];
	char btMac[3*CMDMP_GENERAL_STRING_MAX_LEN];
	char gpu[CMDMP_GENERAL_STRING_MAX_LEN];
	char board[CMDMP_GENERAL_STRING_MAX_LEN];
	char RES[CMDMP_GENERAL_STRING_MAX_LEN];
	int wearing;
#endif
} cmdmp_nv_cfg_t;

#endif