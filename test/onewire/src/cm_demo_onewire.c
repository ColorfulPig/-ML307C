/*********************************************************
 *  @file    cm_demo_onewire.c
 *  @brief   OpenCPU ONEWIRE示例
 *  Copyright (c) 2021 China Mobile IOT.
 *  All rights reserved.
 *  created by zyf 2021/08/30
 ********************************************************/

 /****************************************************************************
 * Included Files
 ****************************************************************************/
#include "stdlib.h"
#include "cm_demo_onewire.h"
#include "cm_onewire.h"
#include "cm_common.h"
#include "cm_iomux.h"
#include "cm_demo_uart.h"
#include "cm_os.h"
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define SKIP_ROM (0xCC)//跳过ROM
#define SEARCH_ROM (0xF0)//搜索ROM
#define READ_ROM (0x33)//读ROM
#define MATCH_ROM (0x55)//匹配ROM
#define ALARM_ROM (0xEC)//告警ROM
#define START_CONVERT (0x44)//开始温度转换，在温度转换期间总线上输出0，转换结束后输出1
#define RAED_SCRATCHPAD (0xBE)//读暂存器的9个字节
#define WRITE_SCRATCHPAD (0x4E)//写暂存器的温度告警TH和TL
#define COPY_SCRATCHPAD (0x48)//将暂存器的温度告警复制到EEPROM，在复制期间总线上输出0，复制完后输出1
#define RECA_llEEPROM (0xB8)//将EEPROM的温度告警复制到暂存器中，复制期间输出0，复制完成后输出1
#define READ_POWER (0xB4)//读电源的供电方式：0为寄生电源供电；1为外部电源供电
#define DS_PRECISION (0x7f)//精度配置寄存器 1f=9位; 3f=10位; 5f=11位; 7f=12位;
#define DS_ALARMTH (0x64)
#define DS_ALARMTL (0x8a)

/****************************************************************************
 * Private Types
 ****************************************************************************/

 
/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
 
 
/****************************************************************************
 * Private Data
 ****************************************************************************/
static unsigned char tempx_tab[16]={0x00,0x01,0x01,0x02,0x03,0x03,0x04,0x04,0x05,0x06,0x06,0x07,0x08,0x08,0x09,0x09};

/****************************************************************************
 * Private Functions
 ****************************************************************************/     
static void cm_ds18b20_init(unsigned char precision,unsigned char alarm_th,unsigned char alarm_tl)
{
    cm_onewire_setup_reset();
    cm_onewire_write_byte(SKIP_ROM); 
    cm_onewire_write_byte(WRITE_SCRATCHPAD);
    cm_onewire_write_byte(alarm_tl);
    cm_onewire_write_byte(alarm_th);
    cm_onewire_write_byte(precision);
    cm_onewire_setup_reset();
    cm_onewire_write_byte(SKIP_ROM); 
    cm_onewire_write_byte(COPY_SCRATCHPAD);
}
     
static void cm_dc18b20_start_convert(void)
{
    cm_onewire_setup_reset();
    cm_onewire_write_byte(SKIP_ROM); 
    cm_onewire_write_byte(START_CONVERT); 
}
     
static void cm_onewire_test_init(void)
{
    cm_iomux_set_pin_func(CM_IOMUX_PIN_76,CM_IOMUX_FUNC_FUNCTION2);//pin76第二功能为smart_bat
    cm_onewire_init();
    cm_ds18b20_init(DS_PRECISION, DS_ALARMTH, DS_ALARMTL);
    cm_dc18b20_start_convert();
}
     
static short cm_ds18b20_read(void)
{
    uint32_t temperature_l = 0,temperature_h = 0;
    uint32_t temperature = 0;

    cm_onewire_setup_reset();
    cm_onewire_write_byte(SKIP_ROM); 
    cm_onewire_write_byte(RAED_SCRATCHPAD);
    cm_onewire_read_byte((uint32_t *)&temperature_l);
    cm_onewire_read_byte((uint32_t *)&temperature_h); 
    cm_onewire_setup_reset();

    if(temperature_h & 0x80)
    {
        temperature_h=(~temperature_h) | 0x08;
        temperature_l=~temperature_l+1;
        if(temperature_l==0)
        temperature_h+=1;
    }

    temperature_h=(temperature_h<<4)+((temperature_l&0xf0)>>4);
    temperature_l=tempx_tab[temperature_l&0x0f];

    /*bit0-bit7为小数位，bit8-bit14为整数位，bit15为正负位*/
    temperature=temperature_h;
    temperature=(temperature<<8) | temperature_l; 

    cm_dc18b20_start_convert();
    
    /*返回16位数据  bit0-bit7为小数位，bit8-bit14为整数位，bit15为正负位*/
    return (short)temperature;
}

/****************************************************************************
 * Public Functions
****************************************************************************/

void cm_test_onewire(unsigned char **cmd, int len)
{
    int fun = 0;
    short temperature = 0;
    
    if( (cmd == NULL) || (len != 3))
    {
        cm_demo_printf(" cmd = NULL or cmd len = %d error!\n", len);
        return;
    }
    
    fun = atoi((char *)cmd[2]);
    
    if(fun == 0)//init
    {
        cm_onewire_test_init();
    }
    else if(fun == 1)//read
    {
        temperature = cm_ds18b20_read();
        if(((temperature>>14)&0x01) ==1)
        {
            cm_demo_printf("temperature=-");
        }
        else
        {
            cm_demo_printf("temperature=+");
        }
        
        cm_demo_printf("%d.",((temperature>>8)&0x7F));//int part
        cm_demo_printf("%d degrees Celsius\r\n",((temperature)&0x00FF ));//decimal part
    }
}

