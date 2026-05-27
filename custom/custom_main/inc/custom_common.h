#ifndef __CUSTOM_COMMON_H__
#define __CUSTOM_COMMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stdarg.h"

#if !defined(_countof)
	#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif

#define	HIBYTE(w) 				((uint8_t)(((w)>>8)&0xff))
#define	LOBYTE(w)				((uint8_t)((w)&0xff))

uint16_t calc_crc16(uint8_t *data, uint16_t len);
uint8_t	HexCharToByte(char *pBuffer);
uint8_t calc_xor(uint8_t *buf, uint16_t len);
int make_base64(uint8_t flag, uint8_t *in, uint16_t in_len, uint8_t *out, uint16_t out_len);
int32_t common_sprintf(uint8_t* str, const char* format, ...);
void CopyWord(uint8_t *pDest,uint16_t value);
void CopyDword(uint8_t *pDest,uint32_t value);


#endif


