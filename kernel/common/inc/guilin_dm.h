#ifndef     _GUILIN_DM_H_
#define     _GUILIN_DM_H_
/***********************************************************************
*      CopyRight (C). 2013. Marvell Semi. Co., ltd.
*************************************************************************/
#define PMIC_802                    0x1 /* PM802: 0x12(B0), 0x13(B1)... */

/* Guilin I2c address*/
#define GUILIN_WRITE_BASE_SLAVE_ADDRESS             0x60
#define GUILIN_READ_BASE_SLAVE_ADDRESS              0x61

#define GUILIN_WRITE_POWER_SLAVE_ADDRESS            0x62
#define GUILIN_READ_POWER_SLAVE_ADDRESS             0x63

#define GUILIN_WRITE_GPADC_SLAVE_ADDRESS            0x64
#define GUILIN_READ_GPADC_SLAVE_ADDRESS             0x65

/* There are 3 I2C Address for Guilin.  */
typedef enum {
    GUILIN_BASE_Reg,
    GUILIN_POWER_Reg,
    GUILIN_GPADC_Reg
} guiLinRegTypeE;

#endif /* _GUILIN_DM_H_        */
