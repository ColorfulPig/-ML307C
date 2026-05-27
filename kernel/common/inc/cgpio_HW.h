#ifndef __NGPIO_HW_H__
#define __NGPIO_HW_H__

#define GPIO_MAX_PORT_HANDLERS				128

#define APB_AP_CLOCK_CTRL_BASE    0xD4015000
#define APBC_GPIO_CLC_RST				(APB_AP_CLOCK_CTRL_BASE+0x8)
#define GPIO_REGISTER_BASE				0xD4019000
#define GPIO_REGISTER_GROUPS				4

#define GPIO_GROUP_BASE1					0
#define GPIO_GROUP_BASE2					4
#define GPIO_GROUP_BASE3					8
#define GPIO_GROUP_BASE4					0x100

#define GPIO_EDGE_BASE		0xD4019800
//mfpr edge reg address = base + int(gpio/32)*4
#define MFPR_EDGE_ADDR(n)		(GPIO_EDGE_BASE + ((n >> 5) << 2));
	
#define GPIO_SHIFT(gpio) (1 << (gpio%32))

/*bit definition for GPIO functions*/
#define GPIO_BIT_ENABLE_RED	(1 << 4)
#define GPIO_BIT_ENABLE_FED	(1 << 5)
#define GPIO_BIT_CLEAR_ED		(1 << 6)
#define GPIO_BIT_SLEEP_DIR	(1 << 7)

#define GPIO_BIT_DRIVE_0		(1 << 11)
#define GPIO_BIT_DRIVE_1		(1 << 12)
#define GPIO_BIT_PULLDN		(1 << 13)
#define GPIO_BIT_PULLUP		(1 << 14)
#define GPIO_BIT_PULL_SEL		(1 << 15)


typedef struct 
{
	UINT32 REG;
	UINT32 padding[2];
}GPIO_Single_Register;

typedef volatile struct
{
	volatile GPIO_Single_Register PLR;//0x0
	volatile GPIO_Single_Register PDR;//0xc
	volatile GPIO_Single_Register PSR;//0X18
	volatile GPIO_Single_Register PCR;//0X24
	volatile GPIO_Single_Register RER;//0X30
	volatile GPIO_Single_Register FER;//0X3C
	volatile GPIO_Single_Register EDR;//0X48
	volatile GPIO_Single_Register SDR;//0X54
	volatile GPIO_Single_Register CDR;//0X60
	volatile GPIO_Single_Register SRER;//0X6C
	volatile GPIO_Single_Register CRER;//0X78
	volatile GPIO_Single_Register SFER;//0X84
	volatile GPIO_Single_Register CFER;//0X90
	volatile GPIO_Single_Register AP_MASK;//0X9C
	volatile GPIO_Single_Register CP_MASK;//0XA8
}GPIORegisters;

typedef enum
{
	VSPI_GPIO_0=0,
	VSPI_GPIO_32=32,
	VSPI_GPIO_33=33,
	VSPI_GPIO_34,
	VSPI_GPIO_35,
	VSPI_GPIO_36,
	VSPI_GPIO_64=64,
	VSPI_GPIO_96=96,
	VSPI_GPIO_121=121,
	VSPI_GPIO_MAX=127,
	VSPI_GPIO_NULL=128
}VSpiGpio;


#endif
