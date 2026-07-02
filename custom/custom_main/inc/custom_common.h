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

/* 计算 Modbus CRC16 校验值。 */
uint16_t calc_crc16(uint8_t *data, uint16_t len);
/* 将两个 ASCII 十六进制字符转换为一个字节。 */
uint8_t	HexCharToByte(char *pBuffer);
/* 计算协议数据区的异或校验值。 */
uint8_t calc_xor(uint8_t *buf, uint16_t len);
/* 按项目协议格式进行 Base64 编码或解码。 */
int make_base64(uint8_t flag, uint8_t *in, uint16_t in_len, uint8_t *out, uint16_t out_len);
/* 封装格式化输出，返回写入到缓冲区的字节数。 */
int32_t common_sprintf(uint8_t* str, const char* format, ...);
/* 按高字节在前的顺序写入 16 位数值。 */
void CopyWord(uint8_t *pDest,uint16_t value);
/* 按高字节在前的顺序写入 32 位数值。 */
void CopyDword(uint8_t *pDest,uint32_t value);


#endif


