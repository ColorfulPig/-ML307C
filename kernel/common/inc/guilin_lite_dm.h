#ifndef _GUILIN_LITE_DM_H_
#define _GUILIN_LITE_DM_H_
/***********************************************************************
*      CopyRight (C). 2013. Marvell Semi. Co., ltd.
*************************************************************************/

/* GuilinLite I2c address*/
#define GUILIN_LITE_WRITE_BASE_SLAVE_ADDRESS             0x60
#define GUILIN_LITE_READ_BASE_SLAVE_ADDRESS              0x61

#define GUILIN_LITE_WRITE_POWER_SLAVE_ADDRESS            0x62
#define GUILIN_LITE_READ_POWER_SLAVE_ADDRESS             0x63

#define GUILIN_LITE_WRITE_GPADC_SLAVE_ADDRESS            0x64
#define GUILIN_LITE_READ_GPADC_SLAVE_ADDRESS             0x65

/* There are 3 I2C Address for Guilin.  */
typedef enum {
    GUILIN_LITE_BASE_Reg,
    GUILIN_LITE_POWER_Reg,
} guiLinLiteRegTypeE;

#define GUILIN_LITE_ID_REG                       0x00
#define GUILIN_LITE_INT_ENABLE_REG1              0x09

typedef enum {
    GUILIN_LITE_ONKEY_INT=0,
    GUILIN_LITE_EXTON1_INT,
    GUILIN_LITE_RTC_ALARM_INT,
    GUILIN_LITE_BAT_INT,
} GUILIN_LITE_INTC ;

#define GUILIN_LITE_INTC_TO_STATUS_BIT(intc) (0x01<<intc)
#define GUILIN_LITE_INTC_TO_ENABLE_BIT(intc) (1 << intc)
#define GUILIN_LITE_INTC_TO_ENABLE_REG(intc) GUILIN_LITE_INT_ENABLE_REG1


#define USB_DEV_BASE                0xD4208000
#define USB_PORTSC                  0x184


#define PMUA_BASE                   0xD4282800
#define PMUA_SD_ROT_WAKE_CLR		(PMUA_BASE+0x007C)
/* Vbus Detect */
#define VBUS_DETECT                 (1 << 15)


#define PMIC_803    0x18

void guiLinLiteICRegister(void);
#endif /* _GUILIN_LITE_DM_H_ */
