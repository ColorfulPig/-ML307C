/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

#ifndef _LEVANTE_HW_H_

  #define _LEVANTE_HW_H_

/*----------------------------------------------------
        SANREMO
 * --------------------------------------------------*/
/******   In LEVANTE == total votage is 1.80 volt and 12 bits  ===>> so basically      ************/
/******               each bit in the  register represent a 0.000439 volt    ************/
#define LEVANTE_COEFFICIENT                 439      // (1.8 / 2^12) * 10^6 (each bit in the  register represent 439 microvolt)

/*----------------------------------------------------
        SANREMO REGISTERS
 * --------------------------------------------------*/

#define LEVANTE_CHIP_ID             0x00
#define LEVANTE_STS_PWDOWN_STATUS   0x01
#define LEVANTE_STS_OVER_TEMP           (1 << 0)
#define LEVANTE_STS_PWDOWN              (1 << 3)
#define LEVANTE_STS_SYSEN               (1 << 4)
#define LEVANTE_STS_WD                  (1 << 5)
#define LEVANTE_STS_LONG_ONKEY          (1 << 6)
#define LEVANTE_STS_PWR_HOLD            (1 << 7)
#define LEVANTE_STS_RESET               (1 << 8)

#define LEVANTE_STATUS              0x02
#define LEVANTE_STS_ONKEY               (1 << 0)
#define LEVANTE_STS_EXTON               (1 << 1)
#define LEVANTE_STS_CHG                 (1 << 2)
#define LEVANTE_STS_BAT                 (1 << 3)
#define LEVANTE_STS_PEN                 (1 << 4)
#define LEVANTE_STS_HEADSET             (1 << 5)
#define LEVANTE_STS_HOOK                (1 << 6)
#define LEVANTE_STS_MICIN               (1 << 7)
#define LEVANTE_STS_NA					0

#define LEVANTE_INT_STATUS_1        0x03
#define LEVANTE_INT_STS1_ONKEY          (1 << 0)
#define LEVANTE_INT_STS1_EXTON          (1 << 1)
#define LEVANTE_INT_STS1_CHG            (1 << 2)
#define LEVANTE_INT_STS1_BAT            (1 << 3)
#define LEVANTE_INT_STS1_RTC            (1 << 4)

#define LEVANTE_INT_STATUS_2        0x04
#define LEVANTE_INT_STS2_VBAT            (1 << 0)
#define LEVANTE_INT_STS2_VCHG            (1 << 1)
#define LEVANTE_INT_STS2_VSYS            (1 << 2)
#define LEVANTE_INT_STS2_TINT            (1 << 3)
#define LEVANTE_INT_STS2_GPADC0          (1 << 4)
#define LEVANTE_INT_STS2_GPADC1          (1 << 5)
#define LEVANTE_INT_STS2_GPADC2          (1 << 6)
#define LEVANTE_INT_STS2_GPADC3          (1 << 7)

#define LEVANTE_INT_STATUS_3        0x05
//#define LEVANTE_INT_AUDIO_SHORT            (1 << 0)
#define LEVANTE_INT_STS3_PEN             (1 << 1)
#define LEVANTE_INT_STS3_HEADSET         (1 << 2)
#define LEVANTE_INT_STS3_HOOK            (1 << 3)
#define LEVANTE_INT_STS3_MICIN           (1 << 4)
#define LEVANTE_INT_STS3_CHG_FAIL        (1 << 5)
#define LEVANTE_INT_STS3_CHG_DONE        (1 << 6)
#define LEVANTE_INT_STS3_CHG_IOVER       (1 << 7)

#define LEVANTE_INT_EN_1            0x06
#define LEVANTE_INT_EN_ONKEY            (1 << 0)
#define LEVANTE_INT_EN_EXTON            (1 << 1)
#define LEVANTE_INT_EN_CHG              (1 << 2)
#define LEVANTE_INT_EN_BAT              (1 << 3)
#define LEVANTE_INT_EN_RTC              (1 << 4)

#define LEVANTE_INT_EN_2            0x07
#define LEVANTE_INT_EN_VBAT             (1 << 0)
#define LEVANTE_INT_EN_VCHG             (1 << 1)
#define LEVANTE_INT_EN_VSYS             (1 << 2)
#define LEVANTE_INT_EN_TINT             (1 << 3)
#define LEVANTE_INT_EN_GPADC0           (1 << 4)
#define LEVANTE_INT_EN_TBAT             (1 << 5)
#define LEVANTE_INT_EN_GPADC2           (1 << 6)
#define LEVANTE_INT_EN_GPADC3           (1 << 7)

#define LEVANTE_INT_EN_3            0x08
//#define LEVANTE_INT_EN_AUDIO_SHORT        (1 << 0)
#define LEVANTE_INT_EN_PEN              (1 << 1)
#define LEVANTE_INT_EN_HEADSET          (1 << 2)
#define LEVANTE_INT_EN_HOOK             (1 << 3)
#define LEVANTE_INT_EN_MICIN            (1 << 4)
#define LEVANTE_INT_EN_CHG_FAIL         (1 << 5)
#define LEVANTE_INT_EN_CHG_DONE         (1 << 6)
#define LEVANTE_INT_EN_CHG_IOVER        (1 << 7)

#define LEVANTE_INT_EN_ALL              0x00FFFFFF

#define LEVANTE_LDO4                0x13
#define LEVANTE_LDO4_VLDO4_SET          (1 << 0)
#define LEVANTE_LDO4_VLDO4_SET_SLP      (1 << 3)

#define LEVANTE_MCLK				0x41
#define LEVANTE_CHG_CTRL1			0x48
#define LEVANTE_CHG_CTRL2			0x49
#define LEVANTE_CHG_CTRL3			0x4A
#define LEVANTE_CHG_CTRL4			0x4B
#define LEVANTE_CHG_CTRL5			0x4D
#define LEVANTE_CHG_CTRL7         	0x4E

#define LEVANTE_CHG_CTRL2 			0x49

#define LEVANTE_MEAS_ENABLE1        0x50
#define LEVANTE_MEAS_EN1_VBAT           (1 << 0)
#define LEVANTE_MEAS_EN1_VCHG           (1 << 1)
#define LEVANTE_MEAS_EN1_VSYS           (1 << 2)
#define LEVANTE_MEAS_EN1_TINT           (1 << 3)
#define LEVANTE_MEAS_EN1_RFTMP          (1 << 4)
#define LEVANTE_MEAS_EN1_TBAT           (1 << 5)
#define LEVANTE_MEAS_EN1_GPADC2        	(1 << 6)
#define LEVANTE_MEAS_EN1_GPADC3         (1 << 7)


#define LEVANTE_MEAS_ENABLE3        0x52
#define LEVANTE_MEAS_EN3_IBAT           (1 << 0)
#define LEVANTE_MEAS_EN3_IBAT_COMP 	    (1 << 1)
#define LEVANTE_MEAS_EN3_COULOMB_COUNTER (1 << 2)
#define LEVANTE_MEAS_EN3_PENDET         (1 << 3)
#define LEVANTE_MEAS_EN3_TSIX			(1 << 4)
#define LEVANTE_MEAS_EN3_TSIY           (1 << 5)
#define LEVANTE_MEAS_EN3_TSIZ1        	(1 << 6)
#define LEVANTE_MEAS_EN3_TSIZ2         	(1 << 7)

#define LEVANTE_TSI_PREBIAS_TIME	0x55
#define LEVANTE_PD_PREBIAS_TIME		0x56

#define LEVANTE_GPADC_MISC1         0x57
#define LEVANTE_GPADC_MISC1_GPFSM_EN    (1 << 0)
#define LEVANTE_GPPADC_GP_PREBIAS_TIME  (01 << 1)
#define LEVANTE_GPADC_MISC1_MASK  (LEVANTE_GPADC_MISC1_GPFSM_EN | LEVANTE_GPPADC_GP_PREBIAS_TIME)

#define LEVANTE_VBAT_LOW_TH 		0x5B
#define LEVANTE_VCHG_LOW_TH 		0x5C
#define LEVANTE_TINT_LOW_TH  		0x5E
#define LEVANTE_GPADC1_LOW_TH  		0x60

#define LEVANTE_VBAT_UPP_TH			0x63
#define LEVANTE_VCHG_UPP_TH 	  	0x64
#define LEVANTE_TINT_UPP_TH  	  	0x66
#define LEVANTE_GPADC1_UPP_TH      	0x68


#define LEVANTE_IBAT_MEAS1			0x6B
#define LEVANTE_IBAT_MEAS2			0x6C
#define LEVANTE_VBAT_MEAS1          0x6D
#define LEVANTE_VBAT_MEAS2          0x6E
#define LEVANTE_VCHG_MEAS1          0x6F
#define LEVANTE_VCHG_MEAS2          0x70
#define LEVANTE_VSYS_MEAS1          0x71
#define LEVANTE_VSYS_MEAS2          0x72
#define LEVANTE_TINT_MEAS1			0x73
#define LEVANTE_TINT_MEAS2			0x74
#define LEVANTE_GPADC0_MEAS1        0x75
#define LEVANTE_GPADC0_MEAS2        0x76
#define LEVANTE_GPADC1_MEAS1        0x77
#define LEVANTE_GPADC1_MEAS2        0x78
#define LEVANTE_GPADC2_MEAS1        0x79
#define LEVANTE_GPADC2_MEAS2        0x7A
#define LEVANTE_GPADC3_MEAS1        0x7B
#define LEVANTE_GPADC3_MEAS2        0x7C
#define LEVANTE_TSIX_MEAS1 			0x8D
#define LEVANTE_TSIX_MEAS2 			0x8E
#define LEVANTE_TSIY_MEAS1 			0x8F
#define LEVANTE_TSIY_MEAS2 			0x90
#define LEVANTE_TSIZ1_MEAS1 	  	0x91
#define LEVANTE_TSIZ1_MEAS2 	  	0x92
#define LEVANTE_TSIZ2_MEAS1 	  	0x93
#define LEVANTE_TSIZ2_MEAS2 	  	0x94

#define LEVANTE_SUPPLIES_EN11       0x2B
#define LEVANTE_SUPPEN11_LDO5_EN1       (1 << 7)
#define LEVANTE_SUPPEN11_LDO4_EN1       (1 << 6)

#define LEVANTE_TEST_PAGE1_C8     	0xC8
#define LEVANTE_TEST_PAGE1_C9       0xC9
#define LEVANTE_TEST_PAGE1_CA       0xCA
#define LEVANTE_TEST_PAGE1_CB       0xCB
#define LEVANTE_TEST_PAGE1_CC       0xCC
#define LEVANTE_TEST_PAGE1_CD       0xCD
#define LEVANTE_TEST_PAGE1_CE       0xCE
#define LEVANTE_TEST_PAGE1_CF       0xCF

/*----------------------------------------------------
        PORTOFINO
 * --------------------------------------------------*/

#define PORTOFINO_PREREG_CURLIM_1500mA      0xE     // 1110=1500mA
#define PORTOFINO_PREREG_VSYS__4_5V         0x0     // 0000=4.5V

/*----------------------------------------------------
        PORTOFINO REGISTERS
 * --------------------------------------------------*/
#define PORTOFINO_PREREG_REG1       0x10
#define PORTOFINO_PREREG_CURLIM         (1 << 0)
#define PORTOFINO_PREREG_VSYS           (1 << 4)
#define PORTOFINO_PREREG_CURLIM_BIT     (0)
#define PORTOFINO_PREREG_VSYS_BIT       (4)


#define PORTOFINO_USB_MISC          0x16
#define PORTOFINO_USB_SW_OFF            (1 << 0)


/****************************************/
// Internal Definitions
/****************************************/
#define LEVANTE_EVENT_N_STATUS_NUM        5   // (1 status + 4 event)

#endif  /* _LEVANTE_HW_H_ */

