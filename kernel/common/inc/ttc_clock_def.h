#ifndef __TTC_CLOCK_DEF_H__
#define __TTC_CLOCK_DEF_H__

// The base address of the APB Application Subsystem Clock Control Registers 
#define APB_AP_CLOCK_CTRL_BASE    0xD4015000

#define APBC_UART0_CLK_RST      (APB_AP_CLOCK_CTRL_BASE + 0x0000)   //Clock/Reset Control Register for UART 0
#define APBC_UART1_CLK_RST      (APB_AP_CLOCK_CTRL_BASE + 0x0004)   //Clock/Reset Control Register for UART 1
#define APBC_GPIO_CLK_RST       (APB_AP_CLOCK_CTRL_BASE + 0x0008)   //Clock/Reset Control Register GPIO
#define APBC_PWM0_CLK_RST       (APB_AP_CLOCK_CTRL_BASE + 0x000C)   //Clock/Reset Control Register for PWM 0
#define APBC_PWM1_CLK_RST       (APB_AP_CLOCK_CTRL_BASE + 0x0010)   //Clock/Reset Control Register for PWM 1
#define APBC_PWM2_CLK_RST       (APB_AP_CLOCK_CTRL_BASE + 0x0014)   //Clock/Reset Control Register for PWM 2
#define APBC_PWM3_CLK_RST       (APB_AP_CLOCK_CTRL_BASE + 0x0018)   //Clock/Reset Control Register for PWM 3
#define APBC_SSP0_CLK_RST       (APB_AP_CLOCK_CTRL_BASE + 0x001C)   //Clock/Reset Control Register for SSP 0
#define APBC_SSP1_CLK_RST       (APB_AP_CLOCK_CTRL_BASE + 0x0020)   //Clock/Reset Control Register for SSP 1
#define APBC_IPC_CLK_RST        (APB_AP_CLOCK_CTRL_BASE + 0x0024)   //Clock/Reset Control Register for IPC
#define APBC_RTC_CLK_RST        (APB_AP_CLOCK_CTRL_BASE + 0x0028)   //Clock/Reset Control Register for RTC
#define APBC_TWSI_CLK_RST       (APB_AP_CLOCK_CTRL_BASE + 0x002C)   //Clock/Reset Control Register for TWSI
#define APBC_KPC_CLK_RST        (APB_AP_CLOCK_CTRL_BASE + 0x0030)   //Clock/Reset Control Register for Keypad Controller
#define APBC_TIMERS_CLK_RST     (APB_AP_CLOCK_CTRL_BASE + 0x0034)   //Clock/Reset Control Register for Timer 0
#define APBC_TB_ROTARY_CLK_RST  (APB_AP_CLOCK_CTRL_BASE + 0x0038)   //Clock/Reset Control Register for Trackball and Rotary
#define APBC_AIB_CLK_RST        (APB_AP_CLOCK_CTRL_BASE + 0x003C)   //Clock/Reset Control Register for AIB
#define APBC_SW_JTAG_CLK_RST    (APB_AP_CLOCK_CTRL_BASE + 0x0040)   //Clock/Reset Control Register for JTAG Software Emulation
#define APBC_TIMERS1_CLK_RST    (APB_AP_CLOCK_CTRL_BASE + 0x0044)   //Clock/Reset Control Register for Timer 1
#define APBC_ONEWIRE_CLK_RST    (APB_AP_CLOCK_CTRL_BASE + 0x0048)   //Clock/Reset Control Register for One-Wire
#define APBC_SSP2_CLK_RST       (APB_AP_CLOCK_CTRL_BASE + 0x004C)   //Clock/Reset Control Register for SSP 2


// The base address of the APB Communication Subsystem Clock Control Registers 
#define APB_CP_CLOCK_CTRL_BASE    0xD404B000//0xD403B000////peichange//
#define TCER                    (APB_CP_CLOCK_CTRL_BASE + 0x0014)   //TCU Clock Control Register
#define ACER                    (APB_CP_CLOCK_CTRL_BASE + 0x0018)   //AIRQ Clock Control Register
#define UCER                    (APB_CP_CLOCK_CTRL_BASE + 0x001C)   //UART Clock Control Register
#define USCER1                  (APB_CP_CLOCK_CTRL_BASE + 0x0020)   //USIM 1 Clock Control Register
#define USCER2                  (APB_CP_CLOCK_CTRL_BASE + 0x0024)   //USIM 2 Clock Control Register
#define ICER                    (APB_CP_CLOCK_CTRL_BASE + 0x0028)   //TWSI Clock Control Register
#define ECCER                   (APB_CP_CLOCK_CTRL_BASE + 0x002C)   //APB E-Cipher Clock Control Register
#define TICER                   (APB_CP_CLOCK_CTRL_BASE + 0x0030)   //APB Timers Clock Control Register
#define GCER                    (APB_CP_CLOCK_CTRL_BASE + 0x0034)   //GSSP Clock Control Register
#define AICER                   (APB_CP_CLOCK_CTRL_BASE + 0x0038)   //IPC/RIPC Clock Control Register
#define SCCR                    (APB_CP_CLOCK_CTRL_BASE + 0x003C)   //Slow Clock Clock Control Register

// The base address of Main PMU
#define PMUM_BASE               0xD4050000

#define PMUM_WDTPCR             (PMUM_BASE + 0x0200)                // WDT Control Register

// The base address of Application Subsystem PMU
#define PMUA_BASE               0xD4282800
#define PMUA_IRE_CLK_RES_CTRL   (PMUA_BASE + 0x0048)                // IRE Clock/Reset Control Register
#define PMUA_LCD_CLK_RES_CTRL   (PMUA_BASE + 0x004C)                // LCD Clock/Reset Control Register
#define PMUA_CCIC_CLK_RES_CTRL  (PMUA_BASE + 0x0050)                // CCIC Clock/Reset Control Register
#define PMUA_SDH1_CLK_RES_CTRL  (PMUA_BASE + 0x0054)                // SDIO Host 0 Clock/Reset Control Register
#define PMUA_SDH2_CLK_RES_CTRL  (PMUA_BASE + 0x0058)                // SDIO Host 1 Clock/Reset Control Register
#define PMUA_USB_CLK_RES_CTRL   (PMUA_BASE + 0x005C)                // USB Clock/Reset Control Register
#define PMUA_NF_CLK_RES_CTRL    (PMUA_BASE + 0x0060)                // NAND Flash Clock/Reset Control Register
#define PMUA_DMA_CLK_RES_CTRL   (PMUA_BASE + 0x0064)                // DMA Clock/Reset Control Register
#define PMUA_GEU_CLK_RES_CTRL   (PMUA_BASE + 0x0068)                // GEU Clock/Reset Control Register
#define PMUA_DTC_CLK_RES_CTRL   (PMUA_BASE + 0x00AC)                // DTC Clock/Reset Control Register
#define PMUA_GC_CLK_RES_CTRL    (PMUA_BASE + 0x00CC)                // 2D/3D Graphics Controller Clock/Reset Control Register
#define PMUA_SMC_CLK_RES_CTRL   (PMUA_BASE + 0x00D4)                // SMC Clock/Reset Control Register


#define COEL_APB_RTU_CLKEN      0xD4070014

#if defined(EDEN_1928) || defined(NEZHA3_1826)
typedef enum
{
	CLK_OFF = 0,
	CLK_ON
}CLK_OnOff;
#else
typedef enum
{
	CLK_ON = 0,
	CLK_OFF
}CLK_OnOff;
#endif

#define CLK_REG_READ(x) (*(volatile unsigned long *)(x))
#define CLK_REG_WRITE(x,y) ((*(volatile unsigned long *)(x)) = y )

#endif 
