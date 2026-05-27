/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

#ifndef _TIMER_HW_H
#define _TIMER_HW_H   (1)






/*-----------------------------------------------------------*/


#define APBC_TIMERS_CLK_RST_ADDR 0xD4015034
#define APBC_TIMERS1_CLK_RST_ADDR 0xD4015044

#define TIMER_BASE_ADDRESS_APB_0 0xD4014000
#define TIMER_BASE_ADDRESS_APB_1 0xD4016000

typedef  enum
{
   	APB_TIMER_0,
    APB_TIMER_1,
	MAX_APB_TIMER_NUMBER

}APB_TIMER_NUMBER;

typedef  enum
{
   	APB_SUB_TIMER_0,
    APB_SUB_TIMER_1,
    APB_SUB_TIMER_2,
	MAX_APB_SUB_TIMER_NUMBER

}APB_SUB_TIMER_NUMBER;


typedef union {
	UINT32 reg;
	struct {
      unsigned
      tmode_1     :1,            /* R/W   TCR_0 count mode */
      tmode_2     :1,            /* R/W   TCR_1 count mode */
      tmode_3     :1,            /* R/W   TCR_2 count mode */
      null        :29;
	} tcmr_bits;
} T_TCMR_APB;

typedef union{
	UINT32 reg ;
	struct {
      unsigned
      t0rs       :1,            /* R/W */
      t1rs       :1,            /* R/W */
      t2rs       :1,            /* R/W */
      null       :29;
	} tcsr_bits;
} T_TCSR_APB;

typedef union{
	UINT32 reg ;
	struct {
      unsigned
      cs_0       :2,            /* R/W */
      cs_1       :3,            /* R/W */
      cs_2       :2,            /* R/W */
      null       :25;
	} tccr_bits;
} T_TCCR_APB;

typedef struct {
      UINT32  match_reg_0 ;    /* R/W */
      UINT32 match_reg_1 ;    /* R/W */
      UINT32 match_reg_2 ;    /* R/W */
      UINT32 RSVD;
} T_MATCH_STR_N_APB; //New

typedef union {
	UINT32 reg;
	struct {
    	  unsigned
      	  ie_0       :1,            /* R/W  interrupt enable for Match register 0 of TRCn */
          ie_1       :1,            /* R/W  interrupt enable for Match register 1 of TRCn */
          ie_2       :1,            /* R/W  interrupt enable for Match register 2 of TRCn */
          null       :29;
    } tier_bits;
}T_TIER_APB;

typedef union {
	UINT32 reg;
	struct {
       unsigned
       tclr_0       :1,            /* W  clear interrupt and status bit, for Match register 0 of TRCn */
       tclr_1       :1,            /* W  clear interrupt and status bit, for Match register 1 of TRCn */
       tclr_2       :1,            /* W  clear interrupt and status bit, for Match register 2 of TRCn */
       null         :29;
    }ticr_bits;
} T_TICR_APB;

typedef union
{
	UINT32 reg;
	struct {
	      unsigned
	      m_0       :1,            /* R/w */
	      m_1       :1,            /* R/w */
	      m_2       :1,            /* R/w */
	      null      :29;
	} tsr_bits;
}
T_TSR_APB;

typedef struct TIMER_HW_REGISTERS_APB   //actually new timer module
{
  UINT32       tcer;                 /* R/W           Timer Count Enable Reg */
  T_TCMR_APB       tcmr;                 /* R/W           Timer Count Mode Reg */
  T_TCSR_APB       tcsr;                 /* R/W           Timer Count Restart Reg, newly added */   
  T_TCCR_APB       tccr;                 /* R/W           Timer Clock Control Reg */
  T_MATCH_STR_N_APB tmr   [MAX_APB_SUB_TIMER_NUMBER];  /* R/W           3-match Reg per timer */
  UINT32       tplvr [MAX_APB_SUB_TIMER_NUMBER];  /* R/W           Timer Preload Value Reg */
  UINT32       rsvd1;
  UINT32       tplcr [MAX_APB_SUB_TIMER_NUMBER]; /* R/W           Timer Preload Control Reg */
  UINT32       rsvd2;
  T_TIER_APB       tier  [MAX_APB_SUB_TIMER_NUMBER];  /* R/W           Timer Interrupt Enable Reg */
  UINT32       rsvd3;
  T_TICR_APB       ticr  [MAX_APB_SUB_TIMER_NUMBER];  /* Write only    Timer Interrupt Clear Reg */
  UINT32       rsvd4;
  T_TSR_APB        tsr   [MAX_APB_SUB_TIMER_NUMBER];  /* Read only     Timer Status Register */
  UINT32       rsvd5;
  UINT32       tcr   [MAX_APB_SUB_TIMER_NUMBER];  /* Read Only     Timer Count Reg */
  UINT32       rsvd6[5];    /* 0x0x9C~0xAC */
  UINT32       dummy_twfar;
  UINT32       dummy_twsar;
  UINT32       wd_regs[6];           /*               Implemented in saperate WD driver */
}TIMER_HW_REGISTERS_APB_T;

typedef enum
{
  C_TPLCR_FREE_RUNNING_APB,
  C_TPLCR_ENABLE_PRELOAD_WITH_MATCH_O_APB  // while using pre-load, the other two match registers are obsolete!!
  //C_TPLCR_ENABLE_PRELOAD_WITH_MATCH_1_APB,
  //C_TPLCR_ENABLE_PRELOAD_WITH_MATCH_2_APB
}TPLCR_MODES_APB;

typedef enum
{
	FAST_CLK_13MHZ_APB,
	FAST_CLK_6_5MHZ_APB,
	FAST_CLK_3_25MHZ_APB,
	FAST_CLK_1MHZ_APB,
	SLOW_CLK_32KHZ_APB,
	
} CLOCK_MODES_APB;

typedef enum
{
  C_TCMR_HW_FREE_RUN_MODE_APB = 0x0,
  C_TCMR_HW_PERIODIC_MODE_APB = 0x1,
  C_TCMR_HW_ONE_SHOT_MODE_APB = 0x2,

} TCMR_MODES_APB;

typedef void (*TIMER_CALLBACK_FUNCTION_APB)(void);


int apb_timer_start(APB_TIMER_NUMBER timer_idx, APB_SUB_TIMER_NUMBER sub_timer_idx, CLOCK_MODES_APB clock_mode, TCMR_MODES_APB run_mode, UINT32 uSec, TIMER_CALLBACK_FUNCTION_APB callback);
void apb_timer_stop(APB_TIMER_NUMBER timer_idx, APB_SUB_TIMER_NUMBER sub_timer_idx);
UINT32 apb_timer_get_count(APB_TIMER_NUMBER timer_idx, APB_SUB_TIMER_NUMBER sub_timer_idx);




typedef struct
{
	TIMER_CALLBACK_FUNCTION_APB timerNotificationPtr;	   /*a pointer to the callback function at timer experation */
	CLOCK_MODES_APB  clock_mode;						   /* fast = 13M, 32Khz*/
	TCMR_MODES_APB run_mode;
	UINT8   busy;
	
}APB_TIMER_REF_T;


#define APB_C_TCCR_0_CS_13_DIV_MHz   0x00
#define APB_C_TCCR_1_CS_13_DIV_MHz   0x00
#define APB_C_TCCR_2_CS_13_DIV_MHz   0x00

#define APB_C_TCCR_0_CS_32KHz   0x01
#define APB_C_TCCR_1_CS_32KHz   0x01
#define APB_C_TCCR_2_CS_32KHz   0x02






/*--------- Local defines ---------------------------------------------------*/
/* Manitoba VTCXO = 13M   ; fast_clk = 3,250,000 hz = 13M/4   ==>  1 Clk = 307.692307 nSec = 0.3077 MicroSec */
/* Full Cycle = 32bit * 307.692307 nSec =   ~21.9 Minutes. */
//#define MAX_MICRO_SEC_INTERVAL_IN_32_BIT_REGISTER  0xFFFFFFFFL/307*100

/*alex added*/
/*#define MAX_MICRO_SEC_INTERVAL_32kHz_IN_32_BIT_REGISTER (0xFFFFFFFFL-1)/327*10000*/ // why these actions can leav it fffff..ff
/*lena changed alex addings*/
#define MAX_MICRO_SEC_INTERVAL_32kHz_IN_32_BIT_REGISTER (0xFFFFFFFFUL)
#define MAX_MICRO_SEC_INTERVAL_FAST_IN_32_BIT_REGISTER  (TIMER_USECS_IN_SECOND*(0xFFFFFFFFUL/FAST_TIMER_CLOCK_RATE_IN_HZ))

//#define FAST_TIMER_CLOCK_RATE_IN_HZ                3250000
#define TIMER_USECS_IN_SECOND                      1000000
#define FAST_TIMER_CLOCK_RATE_IN_HZ                13000000
#define SLOW_TIMER_CLOCK_RATE_IN_HZ                32768
#define SECONDS_TO_MICRO_SEC_RATIO                 1000000
#define MILISEC_TO_MICRO_SEC_RATIO                 1000L

//Fast clock (13Mhz/4)
/*alex added*/
#define MICRO_SEC_TO_CLOCK_TICKS(uSec)             (uSec*13)

#define CLOCK_TICKS_TO_MICRO_SEC(clockTicks)       (clockTicks/13)
#define CLOCK_TICKS_TO_MICRO_SEC_CLK_32KHZ(x)  ((x)>(0xFFFFFFFF/15625)?((x/512)*15625+((x%512)*15625)/512):(((UINT32)(x)*15625)/512))
#define CLOCK_TICKS_TO_MICRO_SEC_32K             CLOCK_TICKS_TO_MICRO_SEC_CLK_32KHZ

#define MICRO_SEC_TO_CLOCK_TICKS_CLK_32KHZ(x)  ((x)>(0xFFFFFFFF/512)?((x/15625)*512+((x%15625)*512)/15625):(((UINT32)(x)*512)/15625))
#define MICRO_SEC_TO_CLOCK_TICKS_CLK_FAST               MICRO_SEC_TO_CLOCK_TICKS
#define MICRO_SEC_TO_CLOCK_TICKS_CLK_1KHZ(uSec)         ((uSec*1)/1000)    // 0.001 * Tick = 1USec, Min value = 100 Usec, resolution 100 Usec
/*#define MICRO_SEC_TO_CLOCK_TICKS_CLK_EXT(microSecInterval_4)        ((microSecInterval_4 * 1) / 6) */ /* There is no pin for ext. clk */


/************* Timers HW defenitions *****************************************/

#define HALF_MAX_TCR 3                  // First half is NDR, second - drowsy (if exists)
#define NON_DROWSY_TIMERS_BASE_ADDRESS 0x40A00000UL
#define DROWSY_TIMERS_BASE_ADDRESS     0x41A00000UL  // _HERMON_, not implemented in Manitoba

#define TIMER_BASE_ADDRESS          0xd4080000
#define TIMER_EXT_BASE_ADDRESS      0xd403a000



/******* 3 Match registers per TCR ******************************************/
typedef struct {
      UINT32  match_reg_0 ;    /* R/W */
      UINT32 match_reg_1 ;    /* R/W */
      UINT32 match_reg_2 ;    /* R/W */
} T_MATCH_STR;


/******* TCCR = Timer Clock Control Register  *******************************/
/*clock Source of tcr 0 */
/*#define C_TCCR_CS_0_MASK  0xfffffffc */

typedef enum
{
	C_TCCR_CS_3_25MHZ,
	C_TCCR_CS_1KHZ,
	C_TCCR_CS_32KHz,
	C_TCCR_CS_EXT
}Timer_TCCR_Clock_Type;

#ifdef PHS_SW_DEMO_TTC
#define C_TCCR_1_CS_3_25MHZ   0x00
#define C_TCCR_2_CS_3_25MHZ   0x00
#define C_TCCR_3_CS_3_25MHZ   0x00

#define C_TCCR_1_CS_1KHZ   NULL
#define C_TCCR_2_CS_1KHZ   0x02
#define C_TCCR_3_CS_1KHZ   0x01

#define C_TCCR_1_CS_32KHz   0x01
#define C_TCCR_2_CS_32KHz   0x01
#define C_TCCR_3_CS_32KHz   0x02

#define C_TCCR_1_CS_EXT   0x02
#define C_TCCR_2_CS_EXT   0x03
#define C_TCCR_3_CS_EXT   NULL
#else
#define C_TCCR_CS_13MHZ   0x01

#define C_TCCR_CS_32KHz   0x00
//#define C_TCCR_CS_0_EXT   0x02
#endif



#define C_TCCR_CS_ERROR      -1

#ifdef PHS_SW_DEMO_TTC
typedef union{
	UINT32 reg ;
	struct {
      unsigned
      cs_0       :2,            /* R/W */
      cs_1       :3,            /* R/W */
      cs_2       :2,            /* R/W */
      cs_wdt     :2,            /* R/W */
      null       :23;
	} tccr_bits;
} T_TCCR;

#else
typedef union{
	UINT32 reg ;
	struct {
      unsigned
      cs_1       :2,            /* R/W */
      cs_2       :2,            /* R/W */
      cs_3       :2,            /* R/W */
      cs_4       :2,            /* R/W */
      null       :24;
	} tccr_bits;
} T_TCCR;
#endif

/******* TPLCR = Timer Preload Control Register  ****************************/
typedef struct {
      unsigned
      mcs        :2,            /* R/W    Match Comperator Select */
      null       :30;
} T_TPLCR;


/******* TSR = Timer Status Register  ***********************/
#define C_TSR_TIMER_NOT_MATCH_THE_COUNTER   0
#define C_TSR_TIMER_MATCH_THE_COUNTER       1

typedef union
{
	UINT32 reg;
	struct {
	      unsigned
	      m_1       :1,            /* R/w */
	      m_2       :1,            /* R/w */
	      m_3       :1,            /* R/w */
	      m_4       :1,            /* R/w */
	      null      :28;
	} tsr_bits;
}
T_TSR;


/******** TIER = Timer Interrupt Enable Register  ************/
#define C_TIER_TIMER_DISABLE_MATCH  0
#define C_TIER_TIMER_ENABLE_MATCH   1

#define C_TIER_MATCH_0_MASK 0x00000001
#define C_TIER_MATCH_1_MASK 0x00000002
#define C_TIER_MATCH_2_MASK 0x00000004

typedef union {
	UINT32 reg;
	struct {
    	  unsigned
      	  ie_0       :1,            /* R/W  interrupt enable for Match register 0 of TRCn */
          ie_1       :1,            /* R/W  interrupt enable for Match register 1 of TRCn */
          ie_2       :1,            /* R/W  interrupt enable for Match register 2 of TRCn */
          null       :29;
    } tier_bits;
}T_TIER;

/******** TICR = Timer Interrupt Clear Register  ************/
#define C_TICR_TIMER_NO_AFFECT 0
#define C_TICR_TIMER_CLEAR_INTERRUPT_AND_STATUS 1


/******** TCER = Timer Coiunt Enable Register  *******************************/
typedef struct {
      unsigned
      ten_0       :1,            /* R/W  TCR_0 count enable */
      ten_1       :1,            /* R/W  TCR_1 count enable */
      ten_2       :1,            /* R/W  TCR_2 count enable */
      null        :29;
} T_TCER;


/******** TCMR = Timer Count Mode Register  **********************************/
#ifdef PHS_SW_DEMO_TTC
typedef union {
	UINT32 reg;
	struct {
      unsigned
      tmode_1     :1,            /* R/W   TCR_0 count mode */
      tmode_2     :1,            /* R/W   TCR_1 count mode */
      tmode_3     :1,            /* R/W   TCR_2 count mode */
      null        :29;
	} tcmr_bits;
} T_TCMR;

#else
typedef union {
	UINT32 reg;
	struct {
      unsigned
      tmode_1     :2,            /* R/W   TCR_0 count mode */
      tmode_2     :2,            /* R/W   TCR_1 count mode */
      tmode_3     :2,            /* R/W   TCR_2 count mode */
      tmode_4     :2,            /* R/W   TCR_2 count mode */
      null        :24;
	} tcmr_bits;
} T_TCMR;
#endif

#define TCR_TCMR_MASK      (0x03)

/******* TILR = Timer Interrupt Mode (Length) Register  *********************/
typedef enum
{
    C_TILR_LEVEL,                       /* 0 */
    C_TILR_EDGE                         /* 1 */
}TILR_MODES;

typedef union
{
unsigned reg;
struct
    {
    unsigned til_1    : 1;   /* R/W  interrupt mode for Match register 1 of TRCn */
    unsigned til_2    : 1;   /* R/W  interrupt mode for Match register 2 of TRCn */
    unsigned til_3    : 1;   /* R/W  interrupt mode for Match register 3 of TRCn */
    unsigned null     : 29;
    } rcrd;
} TIMER_T_TILR;

#ifndef PHS_SW_DEMO_TTC
typedef struct
{
  UINT32   tmr;
  UINT32   tcr ;
}
T_MATCH_COUNT_STR;
#endif

typedef union
{
unsigned reg;
struct
    {
    unsigned WE       : 1;   /* R/W  watchdag enable counting */
    unsigned WMS      : 1;   /* R/W  0 - Generate interrupt , 1 - generate reset */
    unsigned null     : 30;
    } wer_str;
} T_WER_STR; /* watch dog enable reg */

typedef struct {
      unsigned
      tclr_0       :1,            /* W  clear interrupt and status bit, for Match register 0 of TRCn */
      tclr_1       :1,            /* W  clear interrupt and status bit, for Match register 1 of TRCn */
      tclr_2       :1,            /* W  clear interrupt and status bit, for Match register 2 of TRCn */
      null         :29;
} T_TICR;

/*******************************************************************/
#ifdef PHS_SW_DEMO_TTC
struct TIMER_HW_REGISTERS
    {
      T_TCCR       tccr;                 /* R/W           Timer Clock Control Reg */
      T_MATCH_STR  tmr   [MAX_HW_TCR_NUMBER];  /* R/W           3-match Reg per timer */
      UINT32       tcr   [MAX_HW_TCR_NUMBER];  /* Read Only     Timer Count Reg */
      T_TSR        tsr   [MAX_HW_TCR_NUMBER];  /* Read only     Timer Status Register */
      T_TIER       tier  [MAX_HW_TCR_NUMBER];  /* R/W           Timer Interrupt Enable Reg */
      UINT32       tplvr [MAX_HW_TCR_NUMBER];  /* R/W           Timer Preload Value Reg */
      UINT32       tplcr [MAX_HW_TCR_NUMBER]; /* R/W           Timer Preload Control Reg */
      UINT32       wd_regs[4];           /*               Implemented in saperate WD driver */
      T_TICR       ticr  [MAX_HW_TCR_NUMBER];  /* Write only    Timer Interrupt Clear Reg */
      UINT32       wd_twicr;
      UINT32       tcer;                 /* R/W           Timer Count Enable Reg */
      T_TCMR       tcmr;                 /* R/W           Timer Count Mode Reg */
      TIMER_T_TILR tilr  [MAX_HW_TCR_NUMBER];  /* R/W           Timer Interrupt Mode (Level, Edge) */
      UINT32       dummy_twcr;
      UINT32       dummy_twfar;
      UINT32       dummy_twsar;
      UINT32       tcvwr [MAX_HW_TCR_NUMBER];  /* R/W           Timer value wr-rd request */
    };

#else
struct TIMER_HW_REGISTERS
    {
      T_TCMR       tcmr ;  				/* R/W           Timer Count Mode Reg */
      T_TSR        tsr  ;				/*      Timer Status Register */
      T_TCCR       tccr;                /* R/W           Timer Clock Control Reg */
	  T_TIER       tier ; 				/* R/W           Timer Interrupt Enable Reg */
      T_MATCH_COUNT_STR  tmatch_count[MAX_TCR_NUMBER];	/* R/W           match & count regs per timer */
	  /* WatchDog Registers*/
	  UINT32		wtpr;
	  UINT32		wcr;
	  T_WER_STR		wer;
	  UINT32		wfar;
	  UINT32		wsar;
	  UINT32		wcrr;

	  UINT32		Reserved_1;
	  UINT32		Reserved_2;
	  /* Shadow registers for COMM--MSA dual core access*/
	  T_TCMR		tcmr_A;             /* R/W           Timer Count Mode Shadow Reg */
	  T_TSR			tccr_A;             /* R/W           Timer Clock Control Shadow Reg */
	  T_TIER		tier_A;             /* R/W           Timer Interrupt Enable Shadow Reg*/

    };  /* TIMER_HW_REGISTERS */
#endif

/* Setting physical addresses for Timers HW registers */
#define NON_DROWSY_HW_REG  (* (volatile struct TIMER_HW_REGISTERS *) NON_DROWSY_TIMERS_BASE_ADDRESS)
#define DROWSY_HW_REG      (* (volatile struct TIMER_HW_REGISTERS *) DROWSY_TIMERS_BASE_ADDRESS)
//#define TIMER_USE_TCVWR //TCVWR latching time is too long, therefore not recommended


/*--------- Global constants definitions ------------------------------------*/

/*--------- Local constants definitions -------------------------------------*/


/*-------- Local macro definitions ------------------------------------------*/


/*-------- Local type definitions -------------------------------------------*/

/*  This structure holds all important information regarding a specific tcr */
typedef struct
{
    UINT32 microSec;                                   /*timer interval in microseconds */
    UINT32 timerAddingValue;                           /*time interval to add in SW periodic mode */
    TCMR_MODES repetative_mode;                        /*one shot or HW periodic or SW preiodic */
	TCMR_MODES real_mode ;
    TIMER_CALLBACK_FUNCTION timerNotificationPtr;      /*a pointer to the callback function at timer experation */
    UINT8 cookie;                                      /* this parameter is return uchanged from Callback routine */
    /* additions for whitesail */
    /*MATCH_NUMBER match_number; */                    /* range [0..2] */
    CLOCK_MODES  clock_mode;                           /* fast = vtcxo/4, 32Khz, 1Khz, 256 Hz and Ext. */
    BOOL   busy;
    UINT8   interrupt_required;                         /* cause an interrupt when timer expiered ? */
    UINT32  last32kValue;                               /* last read 32kHz value. May be used as "cached"*/
} TIMER_CONFIG_STR;

typedef struct
{
    volatile struct TIMER_HW_REGISTERS *timer_hw_ptr;  /* according to Drowsy Non-Drowsy regions */
	UINT8  ireg;                                       /* register index (0..2) inside the timer module */
    UINT8  intcid;                                     /* INTC interrupt ID */
	UINT8  apbclock;
	UINT8  pmuclock;
}TIMER_CONST_STR;
#endif
