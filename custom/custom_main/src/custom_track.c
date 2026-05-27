
#include "custom_track.h"

#define	TRACK_BUF_SIZE		4096

static char *track_buf = NULL;
static uint8_t track_onoff = 0;

int custom_track_printf(const char *_fmt, ...)
{
	va_list ap;
	uint16_t pos = 0;

	if((track_buf != NULL) && (track_onoff != 0))
	{
		memset(track_buf, 0, TRACK_BUF_SIZE);
		
		va_start(ap,_fmt);   
		pos = vsprintf(track_buf,_fmt,ap);  
		va_end(ap);
		
		track_buf[pos++] = '\r';
		track_buf[pos++] = '\n';
		custom_track_send((uint8_t *)track_buf, pos);
	}
	
	return pos;

}

int custom_track_printHex(char *_msg, uint8_t *_data, uint16_t _len)
{
	uint16_t k,pos;
	uint8_t hi,low;
	
	if((track_buf != NULL) && (track_onoff != 0))
	{
		if(_len>(TRACK_BUF_SIZE/3-64))
		{
			return -1;
		}
		
		memset(track_buf, 0, TRACK_BUF_SIZE);
		pos = sprintf(track_buf,"%s,len=%d,hex=", _msg, _len);
		
		for(k=0;k<_len;k++)
		{
			hi 	= (_data[k]>>4)&0x0F;
			low  = (_data[k]>>0)&0x0F;
			track_buf[pos++] = BYTETOHEX(hi);
			track_buf[pos++] = BYTETOHEX(low);
			track_buf[pos++] = ' ';
		}
		track_buf[pos++] = '\r';
		track_buf[pos++] = '\n';	
		custom_track_send((uint8_t *)track_buf, pos);
	}
	
	return 0;
}

int	custom_track_printRaw(char *_msg, uint8_t *_data, uint16_t _len)
{
	uint16_t pos;

	if((track_buf != NULL) && (track_onoff != 0))
	{
		if(_len>(TRACK_BUF_SIZE-64))
		{
			return -1;
		}

		memset(track_buf, 0, TRACK_BUF_SIZE);
		pos = sprintf(track_buf, "%s,len=%d,raw=", _msg,  _len);
		
		memcpy(&track_buf[pos], _data, _len);
		pos += _len;
		track_buf[pos++] = '\r';
		track_buf[pos++] = '\n';
		custom_track_send((uint8_t *)track_buf, pos);
	}
	
	return 0;
}

int custom_track_enable(uint8_t _onoff)
{
	track_onoff = _onoff;
	
	return 0;
}

int custom_track_init(void)
{    
	track_buf = cm_malloc(TRACK_BUF_SIZE);

	// test
	custom_track_enable(1);

    return 0;
}

