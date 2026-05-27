#ifndef _NINGBO_DM_H_
#define _NINGBO_DM_H_
/***********************************************************************
*      CopyRight (C). 2013. Marvell Semi. Co., ltd.
*************************************************************************/

#define PMIC_813                            0x3
#define PMIC_813_A0                         0x30
#define PMIC_813_A1                         0x31
#define PMIC_813_A2                         0x32
#define PMIC_813_A3                         0x3B

/* There are 3 I2C Address for Ningbo.  */
typedef enum {
    NINGBO_BASE_Reg,
    NINGBO_POWER_Reg,
    NINGBO_GPADC_Reg,
    NINGBO_TEST_Reg
} ningBoRegTypeE;

#define PMIC_BAD_VALUE                      0xFFFF
#define PM_AVERAGE_BATTERY_NB               32
#define PM_FILTRATE_BATTERY_STEP            800
#define PM_HYSTERESIS_BATTERY_STEP          50
#define PM_HYSTERESIS_BATTERY_PERCENT_EDGE  7

typedef struct batStableT{
    /// sum the battery measures
    UINT32 sum;
    /// calculated mean level
    UINT16 avg;
    /// histeresys stable value
    UINT16 stable;
    /// last instant value
    UINT16 instant;
    /// array of measures to maintaint the mean
    UINT16 levels[PM_AVERAGE_BATTERY_NB];
    /// current index in the array
    UINT8 idx;
    /// number of measures (for init)
    UINT8 nb;
    /// calculated percent from the stable value
    UINT8 precent;
} batStableTT;

/***************************************************************************
** Register Macro : #define REG_MACROS_NAME         VALUE
** Bit Of Register Macro #define BIT_MACROS_NAME    (0x01 << SHIFT)
****************************************************************************/

#define NINGBO_ID_REG                       0x00            /* Identification Register. */

/***************************************************************************
    ** Faults, Status & Interrupts Module Registers **
****************************************************************************/
#define NINGBO_STATUS_REG1                  0x01
#define NINGBO_ONKEY_STATUS_BIT             (0x01 << 0)
#define NINGBO_VBAT_STATUS_BIT              (0x01 << 3)
#define NINGBO_VBUS_UVLO_BIT                (0x01 << 4)
#define NINGBO_VBUS_OVP_BIT                 (0x01 << 5)
#define NINGBO_VBUS_STATUS_BIT              (0x01 << 6)

#define NINGBO_INT_STATUS_REG1              0x05            /* Interrupt Status Register1. */
#define NINGBO_INT_STATUS_REG2              0x06            /* Interrupt Status Register2. */
#define NINGBO_INT_ENABLE_REG1              0x09            /* Interrupt Enable Register1. */
// need modify
#define NINGBO_INT_ENABLE_REG2              0x0A            /* Interrupt Enable Register2. */

#define NINGBO_MISC_CFG_REG1                0x0D            /* Misc config Register1. */
#define NINGBO_SW_PDOWN_BIT                 (0x01 << 5)
#define NINGBO_PWR_HOLD_BIT                 (0x1 << 7)

#define NINGBO_PWRUP_LOG_REG                0x10            /* Power up log register. */
#define NINGBO_ONKEY_WAKEUP_BIT             (0x01 << 0)
#define NINGBO_EXTON1_WAKEUP_BIT            (0x01 << 1)
#define NINGBO_BAT_WAKEUP_BIT               (0x01 << 3)
#define NINGBO_IVBUS_DETECT_BIT             (0x01 << 6)

#define NINGBO_WD_TIMER_REG                 0x11            /* WD config register. */
#define NINGBO_WD_TIMER_ACT_BITS            (0x1 << 0 | 0x1 << 1 | 0x1 << 2 | 0x1 << 3)
#define NINGBO_WD_TIMER_1s                  (0x0 << 3 | 0x0 << 2 | 0x0 << 1 | 0x0 << 0)
#define NINGBO_WD_TIMER_2s                  (0x0 << 3 | 0x0 << 2 | 0x0 << 1 | 0x1 << 0)
#define NINGBO_WD_TIMER_4s                  (0x0 << 3 | 0x0 << 2 | 0x1 << 1 | 0x0 << 0)
#define NINGBO_WD_TIMER_8s                  (0x0 << 3 | 0x0 << 2 | 0x1 << 1 | 0x1 << 0)

#define NINGBO_WD_REG                       0x1D            /* Watchdog Register */
#define NINGBO_WD_DIS_BIT                   (0x01 << 0)

#define NINGBO_TEST_PAGE_REG                0x1F
#define NINGBO_UNLOCK_TEST_PAGE_BIT         (0x01 << 0)

/***************************************************************************
    ** Charger Module Registers **
****************************************************************************/
#define NINGBO_CHG_CTRL_CURRENT1_REG        0x21
#define NINGBO_CHG_PRE_CUR_SHIFT            2
#define NINGBO_CHG_TERM_CUR_SHIFT           0
#define NINGBO_CHG_PRE_CUR_MASK             (0x1 << 3 | 0x1 << 2)
#define NINGBO_CHG_TERM_CUR_MASK            (0x1 << 1 | 0x1 << 0)
            
#define NINGBO_CHG_CTRL_MISC2_REG           0x22
            
#define NINGBO_CHG_CTRL_CURRENT2_REG        0x23
#define NINGBO_CHG_MAX_CURRENT_SHIFT        4
#define NINGBO_CHG_CURRENT_SHIFT            0
#define NINGBO_CHG_MAX_CURRENT_MASK         (0x1 << 7 | 0x1 << 6 | 0x1 << 5 | 0x1 << 4)
#define NINGBO_CHG_CURRENT_MASK             (0x1 << 3 | 0x1 << 2 | 0x1 << 1 | 0x1 << 0)
            
#define NINGBO_CHG_CTRL_TIMER1_REG          0x24
#define NINGBO_CHG_CTRL_TIMER_CHK_VOL_SHIFT 6
#define NINGBO_CHG_CTRL_TIMER_CHK_CUR_SHIFT 4
#define NINGBO_CHG_CTRL_TIMER_TRI_CHG_SHIFT 2
#define NINGBO_CHG_CTRL_TIMER_PRE_CHG_SHIFT 0
#define NINGBO_CHG_CTRL_TIMER_CHK_VOL_MASK  (0x1 << 7 | 0x1 << 6)
#define NINGBO_CHG_CTRL_TIMER_CHK_CUR_MASK  (0x1 << 5 | 0x1 << 4)
#define NINGBO_CHG_CTRL_TIMER_TRI_CHG_MASK  (0x1 << 3 | 0x1 << 2)
#define NINGBO_CHG_CTRL_TIMER_PRE_CHG_MASK  (0x1 << 1 | 0x1 << 0)
            
#define NINGBO_CHG_CTRL_TIMER2_REG          0x25
#define NINGBO_CHG_CTRL_TIMER_CCCV_SHIFT    6
#define NINGBO_CHG_CTRL_BAT_VOLT_SHIFT      4
#define NINGBO_CHG_CTRL_TIMER_CCCV_MASK     (0x1 << 7 | 0x1 << 6)
#define NINGBO_CHG_CTRL_BAT_VOLT_MASK       (0x1 << 5 | 0x1 << 4)
#define NINGBO_CHG_CTRL_TIMERKEEP_MASK      (0x1 << 3)
#define NINGBO_CHG_CTRL_DRV_SEGMENT_EN      (2)
            
#define NINGBO_CHG_CTRL_MISC3_REG           0x26
#define FSM_NORMAL                          0x00
#define FSM_FORCE_SHUTDOWN                  0x08
#define FSM_FORCE_CHECK                     0x09
#define FSM_FORCE_TRICKLE                   0x0A
#define FSM_FORCE_PRE_CHARGE                0x0B
#define FSM_FORCE_CC_CHARGE                 0x0C
#define FSM_FORCE_CV_CHARGE                 0x0D
#define FSM_FORCE_TERMINATION               0x0E
#define FSM_FORCE_FAULT                     0x0F

#define NINGBO_READWRITE_DMY_4_REG          0x53
#define NINGBO_DIS_VBUS_UV_BIT              (0x1 << 6)

#define NINGBO_READONLY_DATA_2_REG          0x59
#define NINGBO_RONLY_DATA_VBUS_OV           (0x01 << 5)
#define NINGBO_RONLY_DATA_VBUS_UV           (0x01 << 4)
#define NINGBO_RONLY_DATA_CHG_OK            (0x01 << 3)

/***************************************************************************
    ** RTC control Module Registers **
****************************************************************************/
#define NINGBO_RTC_CTRL_REG                 0xD0            /* RTC_Control */
#define NINGBO_RTC_USE_XO_BIT               (0x01 << 7)
#define NINGBO_READONLY_DATA_4_REG          0x5B
#define NINGBO_CHG_FSM_SHIFT                5
/* 0 -- Shutdown;       1 -- Check;
   2 -- Trickle Charge; 3 -- Pre-Charge;
   4 -- CC-Charge;      5 -- CV-Charge;
   6 -- Termination;    7 -- Fault.  */
#define NINGBO_CHG_FSM_CC                   0x4
#define NINGBO_CHG_FSM_TERM                 0x6
#define NINGBO_CHG_FSM_FAULT                0x7

#define NINGBO_RTC_COUNT_REG1               0xD1            /* RTC_COUNTER<0:7>   */
#define NINGBO_RTC_COUNT_REG2               0xD2            /* RTC_COUNTER<8:15>  */
#define NINGBO_RTC_COUNT_REG3               0xD3            /* RTC_COUNTER<16:23> */
#define NINGBO_RTC_COUNT_REG4               0xD4            /* RTC_COUNTER<24:31> */
#define NINGBO_RTC_MISC_3_REG               0xE3            /* No actual register name, just name it as is. */
#define NINGBO_CLK_32K_SEL_REG              0xE4
#define NINGBO_CLK_32K_SEL                  (0x01 << 4)
    
#define NINGBO_POWERDOWN_LOG_REG            0xE5
#define NINGBO_RTC_MISC_5_REG               0xE7
#define NINGBO_RTC_FAULT_WU_EN_BIT          (0x01 << 0)
#define NINGBO_RTC_FAULT_WU_BIT             (0x01 << 3)
        
#define NINGBO_CRYSTAL_CAP_SET_REG          0xF0
#define NINGBO_CRYSTAL_CAP_20PF_BIT         (0x1 << 6 | 0x1 << 5)
//#define NINGBO_CRYSTAL_CAP_20PF_BIT         0x60

#define NINGBO_RTC_CONTROL_REG              0xF1
#define NINGBO_VBAT_4P4_EN_BIT              (0x1 << 6)
#define NINGBO_RTC_XO_BIT                   (0x1 << 2)            /* 32kHz clock and buffer clock generated form XO.  */

#define NINGBO_RESET_DISCHARGE_REG           0xE2
#define NINGBO_RESET_DISCHARGE_MASK         (0x1 << 3 | 0x1 << 2 | 0x1 << 1 | 0x1 << 0)

#define NINGBO_FAULT_WU_REG                 0xE7
#define NINGBO_FAULT_WU_BIT                 (0x1 << 3)
#define NINGBO_FAULT_WU_ENABLE_BIT          (0x1 << 0)

/***************************************************************************
    ** GPADC Main Config Module Registers **
****************************************************************************/
#define NINGBO_GPADC_MEAS_EN_REG1           0x01            /* GPADC Measurement Enable Register1.          */
#define NINGBO_GPADC_MEAS_EN_REG2           0x02            /* GPADC Measurement Enable Register2.          */
#define NINGBO_GPADC_MEAS_EN_REG3           0x03            /* GPADC Measurement Enable Register3.          */
#define NINGBO_GPADC_MEAS_EN_REG4           0x04            /* GPADC Measurement Enable Register4.          */
            
#define NINGBO_GPADC_MODE_CONTROL_REG       0x06
#define NINGBO_GPADC_SW_TRIG_BIT            (0x01 << 2)
#define NINGBO_NON_STOP_BIT                 (0x01 << 1)            /* Enable NON_STOP GPADC operation mode.         */
#define NINGBO_GPADC_EN_BIT                 (0x01 << 0)            /* Enable the GPADC periodic modes.              */

#define NINGBO_GPADC_BIAS_2_REG             0x0C            /* gpadc3/2 */
#define NINGBO_GPADC_2_BIAS_SHIFT           0
#define NINGBO_GPADC_3_BIAS_SHIFT           4

#define NINGBO_BAT_TEMP_60D_REG1            0x27             /* Battery temperature values for comparison.    */
#define NINGBO_BAT_TEMP_60D_REG2            0x28             /* Battery temperature values for comparison.    */
#define NINGBO_BAT_TEMP_47D_REG1            0x29             /* Battery temperature values for comparison.    */
#define NINGBO_BAT_TEMP_47D_REG2            0x2A             /* Battery temperature values for comparison.    */
#define NINGBO_BAT_TEMP_43D_REG1            0x2B             /* Battery temperature values for comparison.    */
#define NINGBO_BAT_TEMP_43D_REG2            0x2C             /* Battery temperature values for comparison.    */
#define NINGBO_BAT_TEMP_12D_REG1            0x2D             /* Battery temperature values for comparison.    */
#define NINGBO_BAT_TEMP_12D_REG2            0x2E             /* Battery temperature values for comparison.    */
#define NINGBO_BAT_TEMP_8D_REG1             0x2F             /* Battery temperature values for comparison.    */
#define NINGBO_BAT_TEMP_8D_REG2             0x30             /* Battery temperature values for comparison.    */
#define NINGBO_BAT_TEMP_5D_REG1             0x31             /* Battery temperature values for comparison.    */
#define NINGBO_BAT_TEMP_5D_REG2             0x32             /* Battery temperature values for comparison.    */
#define NINGBO_BAT_TEMP_0D_REG1             0x33             /* Battery temperature values for comparison.    */
#define NINGBO_BAT_TEMP_0D_REG2             0x34             /* Battery temperature values for comparison.    */
#define NINGBO_VGPADC0                      0x4C
#define NINGBO_GPADC_MEAS_TINT_1_REG        0x44
#define NINGBO_GPADC_MEAS_TINT_2_REG        0x45
#define NINGBO_GPADC_MEAS_VPWR_1_REG        0x4A

/***************************************************************************
    ** GPADC Measurements Module Registers **
****************************************************************************/
#define NINGBO_GPADC_MEAS_VCHG_1_REG        0x42
#define NINGBO_GPADC_MEAS_TINT_REG          0x44
#define NINGBO_GPADC_MEAS_VBATTEMP_1_REG    0x48
#define NINGBO_GPADC_MEAS_VPWR_REG          0x4A
#define NINGBO_VINLDO_AVE_REG               0x68
#define NINGBO_VINLDO_SLP_REG               0x6A

#define NINGBO_GPADC_BIAS_ENABLE            0x0D
#define NINGBO_GPADC_0_BIAS_OUT             (0x1 << 0)
#define NINGBO_GPADC_1_BIAS_OUT             (0x1 << 1)
#define NINGBO_GPADC_2_BIAS_OUT             (0x1 << 2)
#define NINGBO_GPADC_3_BIAS_OUT             (0x1 << 3)

typedef enum {
    //gpadc bias out en reg,addr=0x0D, no bias en reg
  NINGBO_GPADC0_BIAS_OUT,
  NINGBO_GPADC1_BIAS_OUT,
  NINGBO_GPADC2_BIAS_OUT, /* Battery ID */
  NINGBO_GPADC3_BIAS_OUT, /* Battery Temperature */
} ningBoBiasOutE;

typedef enum {
    NINGBO_TINT_INT = 0,
    NINGBO_GPADC0_INT,
    NINGBO_GPADC1_INT,
    NINGBO_VINLDO_INT,
    NINGBO_VBAT_INT,
    NINGBO_CP_START_ERROR_DET_INT,
    NINGBO_CLASSD_OUT_DET_INT,
    NINGBO_RTC_INT,

    NINGBO_ONKEY_INT = 8,
    NINGBO_EXTON1_INT,
    NINGBO_EXTON2_INT,
    NINGBO_BAT_INT,
    NINGBO_CHG_INT,
    NINGBO_VBUS_OVOU_INT,
    NINGBO_VBUS_DET_INT,
    NINGBO_CP_START_DONE_DET_INT,
} ningBoIntcE ;


#define NINGBO_INTC_TO_STATUS_BIT(intc) (0x01 << intc)
#define NINGBO_INTC_TO_ENABLE_BIT(intc) (1 << (intc % 8))
#define NINGBO_INTC_TO_ENABLE_REG(intc) (NINGBO_INT_ENABLE_REG2 - (intc / 8))

typedef enum {
  //gpadc meas en reg 2 ,addr=0x04
  NINGBO_AVDD_MEAS_EN,
  NINGBO_DVDD_MEAS_EN,
  NINGBO_VSUP_MEAS_EN,
  NINGBO_GPADC1_MEAS_EN,
  //gpadc meas en reg 2 ,addr=0x03
  NINGBO_LDO8_MEAS_EN=8,
  NINGBO_LDO9_MEAS_EN,
  NINGBO_LDO10_MEAS_EN,
  NINGBO_LDO11_MEAS_EN,
  NINGBO_LDO12_MEAS_EN,
  NINGBO_LDO13_MEAS_EN,
  NINGBO_VPWR_MEAS_EN,
  NINGBO_VRTC_MEAS_EN,
  //gpadc meas en reg 2 ,addr=0x02
  NINGBO_GPADC0_MEAS_EN, /*16*/
  NINGBO_LDO1_MEAS_EN,
  NINGBO_LDO2_MEAS_EN,
  NINGBO_LDO3_MEAS_EN,
  NINGBO_LDO4_MEAS_EN,
  NINGBO_LDO5_MEAS_EN,
  NINGBO_LDO6_MEAS_EN,
  NINGBO_LDO7_MEAS_EN,
  //gpadc meas en reg 1 ,addr=0x01
  NINGBO_BUCK1_MEAS_EN, /*24*/
  NINGBO_BUCK2_MEAS_EN,
  NINGBO_BUCK3_MEAS_EN,
  NINGBO_VCHG_MEAS_EN, /*Vbus*/
  NINGBO_VBAT_MEAS_EN,
  NINGBO_TINT_MEAS_EN,
  NINGBO_BATID_MEAS_EN,
  NINGBO_BATTEMP_MEAS_EN,
} ningBoMeasureE ;


#define NINGBO_MEAS_TO_ENABLE_REG(meas_en) ((meas_en >= NINGBO_BUCK1_MEAS_EN) ? NINGBO_GPADC_MEAS_EN_REG1 : \
                                            (meas_en >= NINGBO_GPADC0_MEAS_EN) ? NINGBO_GPADC_MEAS_EN_REG2: \
                                            (meas_en >= NINGBO_LDO8_MEAS_EN) ? NINGBO_GPADC_MEAS_EN_REG3: \
                                            NINGBO_GPADC_MEAS_EN_REG4)

#define NINGBO_MEAS_TO_ENABLE_BIT(meas_en) (0x01 << (meas_en % 8))

#define VOLT_CONVERT_12BIT_MV(meas) ((meas)*(1300*128)/(129*4096)) //(CODE)/4096*1.3*128/129 *1000 ->mv, CODE is the adc output, 12bit
#define TEMP_CONVERT_16BIT_MV(meas) ((meas)*(13*128)/(12*129)-273) //(CODE)*1.3/1.2*128/129-273, CODE is tsen output, high 10bit (total 16bit)

typedef enum 
{
  PRE_CHG_CUR_75MA,
  PRE_CHG_CUR_50MA,
  PRE_CHG_CUR_100MA,
  PRE_CHG_CUR_150MA,
  PRE_CHG_CUR_MAX = PRE_CHG_CUR_150MA
} ningBoPreChgCurrentE;

typedef enum {
  CC_CHG_CUR_100MA,
  CC_CHG_CUR_200MA,
  CC_CHG_CUR_300MA,
  CC_CHG_CUR_400MA,
  CC_CHG_CUR_450MA,
  CC_CHG_CUR_500MA,
  CC_CHG_CUR_550MA,
  CC_CHG_CUR_600MA,
  CC_CHG_CUR_650MA,
  CC_CHG_CUR_700MA,
  CC_CHG_CUR_750MA,
  CC_CHG_CUR_800MA,
  CC_CHG_CUR_850MA,
  CC_CHG_CUR_900MA,
  CC_CHG_CUR_950MA,
  CC_CHG_CUR_1000MA,

  CC_CHG_CUR_150MA,
  CC_CHG_CUR_250MA,
  CC_CHG_CUR_350MA,
  
  CC_CHG_CUR_MAX = CC_CHG_CUR_350MA,
  CC_CHG_CUR_INVALID
} ningBoCcChgCurrentE;

typedef enum 
{
  TERM_CHG_CUR_50MA,
  TERM_CHG_CUR_25MA,
  TERM_CHG_CUR_75MA,
  TERM_CHG_CUR_100MA,
  TERM_CHG_CUR_MAX = TERM_CHG_CUR_100MA
} ningBoTerminateChgCurrentE;

typedef enum {
  VBAT_VOL_4V20,
  VBAT_VOL_4V35,
  VBAT_VOL_4V15,
  VBAT_VOL_4V10,
  VBAT_VOL_4V30,
  VBAT_VOL_4V40,
  VBAT_VOL_MAX = VBAT_VOL_4V40
} ningBoVbatVoltageE;

typedef enum {
  PRE_CHG_MINUTE24,
  PRE_CHG_MINUTE36,
  PRE_CHG_MINUTE48,
  PRE_CHG_MINUTE60,
  PRE_CHG_MINUTE_MAX = PRE_CHG_MINUTE60
} ningBoPreChgTimerE;

typedef enum {
  TRICKLE_CHG_MINUTE6,
  TRICKLE_CHG_MINUTE9,
  TRICKLE_CHG_MINUTE12,
  TRICKLE_CHG_MINUTE15,
  TRICKLE_CHG_MINUTE_MAX = TRICKLE_CHG_MINUTE15
} ningBoTrickleChgTimerE;

typedef enum {
  CUR_CHK_SECOND3,
  CUR_CHK_SECOND7,
  CUR_CHK_SECOND11,
  CUR_CHK_SECOND15,
  CUR_CHK_SECOND_MAX=CUR_CHK_SECOND15
} ningBoCurChkTimerE;

typedef enum {
  VOL_CHK_MSECOND3,
  VOL_CHK_MSECOND7,
  VOL_CHK_MSECOND11,
  VOL_CHK_MSECOND15,
  VOL_CHK_MSECOND_MAX = VOL_CHK_MSECOND15
} ningBoVolChkTimerE;

typedef enum {
  CCCV_MINUTE96,
  CCCV_MINUTE144,
  CCCV_MINUTE192,
  CCCV_MINUTE240,
  CCCV_MINUTE_MAX = CCCV_MINUTE240
} ningBoCcCvTimerE;

typedef enum {
  FSM_SHUTDOWN,
  FSM_CHECK,
  FSM_TRI_CHG,
  FSM_PRE_CHG,
  FSM_CC_CHG,
  FSM_CV_CHG,
  FSM_TERM,
  FSM_FAULT,
  CHGFSM_MAX = FSM_FAULT
} ningBoChgFsmE;

typedef enum {
    CHG_FORCE_SHUTDOWN = 0x8,
    CHG_FORCE_CHECK,      
    CHG_FORCE_TRICKLE,
    CHG_FORCE_PRE,
    CHG_FORCE_CC,         
    CHG_FORCE_CV,         
    CHG_FORCE_TERM,       
    CHG_FORCE_FAULT,      
    CHG_FORCE_MAX = CHG_FORCE_FAULT
} ningBoForceChgStateE;

#define NINGBO_WRITE_BASE_SLAVE_ADDRESS                 0x60
#define NINGBO_READ_BASE_SLAVE_ADDRESS                  0x61
#define NINGBO_WRITE_POWER_SLAVE_ADDRESS                0x62
#define NINGBO_READ_POWER_SLAVE_ADDRESS                 0x63
#define NINGBO_WRITE_GPADC_SLAVE_ADDRESS                0x64
#define NINGBO_READ_GPADC_SLAVE_ADDRESS                 0x65
#define NINGBO_WRITE_TEST_SLAVE_ADDRESS                 0x6E
#define NINGBO_READ_TEST_SLAVE_ADDRESS                  0x6F

#endif /* _NINGBO_DM_H_ */
