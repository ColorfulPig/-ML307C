
#include "custom_datetime.h"

#define SECOND_OF_DAY (24*60*60)

static const char DayOfMon[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

void custom_seceond_to_datetime(long seconds, cm_tm_t *tTime)
{
	unsigned short i,j,iDay;
	unsigned long lDay;

	lDay = seconds / SECOND_OF_DAY;    
	seconds = seconds % SECOND_OF_DAY;

	i = 1970;
	while(lDay > 365)
	{
		if(((i%4==0)&&(i%100!=0)) || (i%400==0))
		{
			lDay -= 366;
		}
		else
		{
			lDay -= 365;
		}
		i++;
	}
	if((lDay == 365) && !(((i%4==0)&&(i%100!=0)) || (i%400==0)))
	{
		lDay -= 365;
		i++;
	}
	tTime->tm_year = i;   
	for(j=0;j<12;j++)   
	{
		if((j==1) && (((i%4==0)&&(i%100!=0)) || (i%400==0)))
		{
			iDay = 29;
		}
		else
		{
			iDay = DayOfMon[j];
		}
		if(lDay >= iDay) 
			lDay -= iDay;
		else 
			break;
	}
	tTime->tm_mon = j+1;
	tTime->tm_mday = lDay+1;
	tTime->tm_hour = ((seconds / 3600))%24;	//这里注意，世界时间已经加上北京时间差8，
	tTime->tm_min = (seconds % 3600) / 60;
	tTime->tm_sec = (seconds % 3600) % 60;
}

void custom_get_now_datetime(cm_tm_t *dt)
{
    custom_seceond_to_datetime((long)(cm_rtc_get_current_time() + cm_rtc_get_timezone() * 60 * 60), dt);
}

