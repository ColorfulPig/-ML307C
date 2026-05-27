/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

#ifndef _CCU_H
#define _CCU_H   (1)
#define  CCCR_ADDR (0xF00E0000UL)
#define  CCCR_REG     (* (volatile UINT32*) CCCR_ADDR)
#define  CCCR_REG_SET(val)  CCCR_REG = (val)

#define APB_TMR_ADDR (0xd4060104UL)
#define APB_TMR_REG     (* (volatile UINT32*) APB_TMR_ADDR)
#define APB_TMR_REG_SET(val)  APB_TMR_REG = (val)

#define GCKEN_ADDR			(0xf00e000cUL)
#define GCKEN_REG			(* (volatile UINT32*) GCKEN_ADDR)
#define GCKEN_REG_SET(val)	GCKEN_REG = (val)
#define GCKEN_REG_GET()		(GCKEN_REG)

#define GCKEN_GSSP1_UNIT_CLK_EN      (0x1UL << 1)
#define GCKEN_GSSP2_UNIT_CLK_EN      (0x1UL << 2)
#define GCKEN_GBCK_PERIPH_BUS_CLK_EN 0x40
#define GCKEN_SSP_MASK		(GCKEN_GSSP2_UNIT_CLK_EN | GCKEN_GBCK_PERIPH_BUS_CLK_EN)

 //GMCGC
// R/W
// 0xF020_400C
// GPB controlled GSM Modem Clock Gator Register
// 0000_0000
#define GMCGC_ADDR			(0xf020400cUL)
#define GMCGC_REG			 (* (volatile UINT32*) GMCGC_ADDR)

#define GMCGC2_ADDR        	(0xf0204014UL)
#define GMCGC2_REG			 (* (volatile UINT32*) GMCGC2_ADDR)

#define GMCGC_REG_SET(val)	 GMCGC_REG = (val)
#define GMCGC_REG_GET()		(GMCGC_REG)

#define GMCGC_DSSP3_EN       (0x1UL << 6  )
#define GMCGC_GLS0_EN        (0x1UL << 12 )
#define GMCGC_DSSP3_SET_MASK       (GMCGC_DSSP3_EN | GMCGC_GLS0_EN)



/* External prototypes */
extern void CCUPhase1Init(void) ;
extern void CCU_GSSP1_BusClk_Enable(void)   ;
extern void CCU_GSSP2_BusClk_Enable(void)   ;
extern void CCU_PeriphBusClk_Enable(void);



#endif
/*  END OF MODULE : ccu.h */
