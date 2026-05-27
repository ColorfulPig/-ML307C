#ifndef _PM812_DM_H_
#define _PM812_DM_H_

#define PMIC_812                                   0x6 /* PM812: 0x64 */

/* PM812 I2c address*/
#define PM812_WRITE_BASE_SLAVE_ADDRESS             0x60
#define PM812_READ_BASE_SLAVE_ADDRESS              0x61

#define PM812_WRITE_POWER_SLAVE_ADDRESS            0x62
#define PM812_READ_POWER_SLAVE_ADDRESS             0x63

#define PM812_WRITE_GPADC_SLAVE_ADDRESS            0x64
#define PM812_READ_GPADC_SLAVE_ADDRESS             0x65

/* There are 3 I2C Address for PM812.  */
typedef enum {
    PM812_BASE_Reg,
    PM812_POWER_Reg,
    PM812_GPADC_Reg
} pm812RegTypeE;

#endif /* _PIMC_DM_H_ */
