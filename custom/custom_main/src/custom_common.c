
#include "custom_common.h"
#include "base64.h"

uint16_t calc_crc16(uint8_t *data, uint16_t len)
{
    uint16_t crc16 = 0xFFFF;

    while(len--)
    {
        crc16 ^= *data++;

        for(uint8_t i = 0; i < 8; i++)
        {
            if(crc16 & 0x0001)
            {
                crc16 = (crc16 >> 1) ^ 0xA001;
            }
            else
            {
                crc16 >>= 1;
            }
        }
    }

    return crc16;
}

uint8_t	HexCharToByte(char *pBuffer)
{
	char	tmp1,tmp2,dat;
	
	tmp1 = *pBuffer;
	tmp2 = *(pBuffer+1);
		
	if((tmp1>='0')&&(tmp1<='9'))
		tmp1 = tmp1-'0';
	else if((tmp1>='a')&&(tmp1<='f'))
		tmp1 = tmp1-'a'+0x0a;
	else if((tmp1>='A')&&(tmp1<='F'))
		tmp1 = tmp1-'A'+0x0a;
	if((tmp2>='0')&&(tmp2<='9'))
		tmp2 = tmp2-'0';
	else if((tmp2>='a')&&(tmp2<='f'))
		tmp2 = tmp2-'a'+0x0a;
	else if((tmp2>='A')&&(tmp2<='F'))
		tmp2 = tmp2-'A'+0x0a;
	
	dat = tmp1*16 + tmp2;
	
	return dat;
}

uint8_t calc_xor(uint8_t *buf, uint16_t len)
{
	uint8_t check = buf[0];
	uint16_t k;

	for(k = 1; k < len; k++)
	{
		check ^= buf[k];
	}
	return check;
}

// {1+base64}
int make_base64(uint8_t flag, uint8_t *in, uint16_t in_len, uint8_t *out, uint16_t out_len)	// 返回字节长度
{
	size_t w_len=0;

	if(flag == 0)		// 4G
	{
		if(mbedtls_base64_encode(out, out_len, &w_len, &in[1], in_len-2) == 0)	// 原始内容去掉头尾{}
		{
			if(out_len > (w_len + 3))
			{
				for(int k = (w_len - 1); k >= 0; k--)
				{
					out[k + 2] = out[k];	// 数据往后搬移
				}
				w_len += 3;
			
				out[0] = '{';
				out[1] = '1';
				out[w_len - 1] = '}';
			}
		}
	}
	else if(flag == 1)	// BT
	{
		if(mbedtls_base64_encode(out, out_len, &w_len, in, in_len) == 0)
		{
			/*if(out_len > (w_len + 4))
			{
				for(int k = (w_len - 1); k >= 0; k--)
				{
					out[k + 3] = out[k];	// 数据往后搬移
				}
				w_len += 4;
		
				out[0] = '{';
				out[1] = '1';
				out[2] = '+';
				out[w_len - 1] = '}';
			}*/
		}	

	}

	return w_len;
}

int32_t common_sprintf(uint8_t* str, const char* format, ...)
{
	va_list ap;
	int ret = 0;

	va_start(ap, format);
	ret = vsprintf((char*)str, (char*)format, ap);
	va_end(ap);

	return ret;
}

void CopyWord(uint8_t *pDest,uint16_t value)
{
	*pDest = HIBYTE(value);
	*(pDest+1) = LOBYTE(value);
}

void CopyDword(uint8_t *pDest,uint32_t value)
{
	uint16_t	tmp;
	
	tmp = ((uint16_t)(value>>16))&0xffff;
	*pDest = HIBYTE(tmp);
	*(pDest+1) = LOBYTE(tmp);

	tmp = ((uint16_t)value)&0xffff;
	*(pDest+2) = HIBYTE(tmp);
	*(pDest+3) = LOBYTE(tmp);
}


