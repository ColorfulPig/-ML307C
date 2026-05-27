#define BLOCK_LEN  16
#define REGS_GEU_BASE  0xd4201000
#define TRUE 1
#define FALSE 0
#define NULL 0

#define INFO_LOG(fmt,...) //uart_printf(fmt, ##__VA_ARGS__);
#define DEBG_LOG(fmt,...) uart_printf(fmt, ##__VA_ARGS__)
#define WARN_LOG(fmt,...) uart_printf(fmt, ##__VA_ARGS__)
#define ERRO_LOG(fmt,...) uart_printf(fmt, ##__VA_ARGS__)

#define AES_REG32(addr)  ((volatile unsigned int *)(addr))
#define aes_writel(v, a) (*AES_REG32(a) = (v))
#define aes_readl(a)     (*AES_REG32(a))
#define AES_ADDR(addr,offset) ((volatile unsigned int *)(addr + offset))

#define REG_GEU_STATUS                             AES_ADDR(REGS_GEU_BASE, 0X000)/*GEU Status Register*/
#define REG_GEU_CONFIG                             AES_ADDR(REGS_GEU_BASE, 0X004)/*GEU Configuration Register*/

#define REG_GEU_INIT_KEY_VALUE                     AES_ADDR(REGS_GEU_BASE, 0X008)/*GEU Initial Key Value Register*/
#define REG_GEU_INPUT_DATA_ENC_DEC                 AES_ADDR(REGS_GEU_BASE, 0X028)/*Input Data for Encryption/Decryption Register*/

#define REG_GEU_FUSE_PROG_VAL1                     AES_ADDR(REGS_GEU_BASE, 0X038)/*Fuse Programming Value[127:0] in Software Fuse Programming Mode Register*/
#define REG_GEU_FUSE_PROG_VAL2                     AES_ADDR(REGS_GEU_BASE, 0X048)/*Fuse Programming Value[255:128] in Software Fuse Programming Mode Register*/
#define REG_GEU_OUT_DATA_AFTER_ENC_DEC             AES_ADDR(REGS_GEU_BASE, 0X058)/*Output Data after Encryption/Decryption Register*/
#define REG_GEU_SRAM_SPACE_ROUND_KEY_TABLE         AES_ADDR(REGS_GEU_BASE, 0X068)/*SRAM Space for Round Key Table Register*/
#define REG_GEU_ROM_SPACE_SBOX_TABLE                   AES_ADDR(REGS_GEU_BASE, 0X168)/*ROM Space for SBOX Table Register*/
#define REG_GEU_ROM_SPACE_INV_SBOX_TABLE           AES_ADDR(REGS_GEU_BASE, 0X268)/*ROM Space for Inverse SBOX Table Register*/
#define REG_GEU_OTP_DATA1                          AES_ADDR(REGS_GEU_BASE, 0X368)/*One Time Programmable Data Register 1*/
#define REG_GEU_OTP_DATA2                          AES_ADDR(REGS_GEU_BASE, 0X36C)/*One Time Programmable Data Register 2*/
#define REG_GEU_OTP_DATA3                          AES_ADDR(REGS_GEU_BASE, 0X370)/*One Time Programmable Data Register 3*/
#define REG_GEU_OTP_DATA4                          AES_ADDR(REGS_GEU_BASE, 0X374)/*One Time Programmable Data Register 4*/
#define REG_GEU_PRNG_CTRL_REG                          AES_ADDR(REGS_GEU_BASE, 0X3a8)/*PRNG Control Register*/
#define REG_GEU_FUSE_VAL_APCFG1                    AES_ADDR(REGS_GEU_BASE, 0X404)/*Fuse Value for Application Subsystem Config Register 1*/
#define REG_GEU_FUSE_VAL_APCFG2                    AES_ADDR(REGS_GEU_BASE, 0X408)/*Fuse Value for Application Subsystem Config Register 2*/
#define REG_GEU_FUSE_VAL_APCFG3                    AES_ADDR(REGS_GEU_BASE, 0X40C)/*Fuse Value for Application Subsystem Config Register 3*/
#define REG_GEU_FUSE_BANK0_127_96                  AES_ADDR(REGS_GEU_BASE, 0X410)/*Fuse Bank0 127 96 Register*/
#define REG_GEU_FUSE_BANK0_239_208                 AES_ADDR(REGS_GEU_BASE, 0X414)/*Fuse Bank0 239 208 Register*/
#define REG_GEU_FUSE_BANK0_207_192                 AES_ADDR(REGS_GEU_BASE, 0X418)/*Fuse Bank0 207 192 Register*/
#define REG_GEU_FUSE_BANK3_223_192                 AES_ADDR(REGS_GEU_BASE, 0X41c)/*Fuse Bank3 223 192 Register*/
#define REG_GEU_FUSE_BANK3_255_224                 AES_ADDR(REGS_GEU_BASE, 0X420)/*Fuse Bank3 255 224 Register*/

#define REG_GEU_FUSE_VAL_ROOT_KEY1                 AES_ADDR(REGS_GEU_BASE, 0X424)/*Fuse Value for Root Key Register 1*/
#define REG_GEU_FUSE_VAL_ROOT_KEY2                 AES_ADDR(REGS_GEU_BASE, 0X428)/*Fuse Value for Root Key Register 2*/
#define REG_GEU_FUSE_VAL_ROOT_KEY3                 AES_ADDR(REGS_GEU_BASE, 0X42C)/*Fuse Value for Root Key Register 3*/
#define REG_GEU_FUSE_VAL_ROOT_KEY4                 AES_ADDR(REGS_GEU_BASE, 0X430)/*Fuse Value for Root Key Register 4*/
#define REG_GEU_FUSE_VAL_ROOT_KEY5                 AES_ADDR(REGS_GEU_BASE, 0X434)/*Fuse Value for Root Key Register 5*/
#define REG_GEU_FUSE_VAL_ROOT_KEY6                 AES_ADDR(REGS_GEU_BASE, 0X438)/*Fuse Value for Root Key Register 6*/
#define REG_GEU_FUSE_VAL_ROOT_KEY7                 AES_ADDR(REGS_GEU_BASE, 0X43C)/*Fuse Value for Root Key Register 7*/
#define REG_GEU_FUSE_VAL_ROOT_KEY8                 AES_ADDR(REGS_GEU_BASE, 0X440)/*Fuse Value for Root Key Register 8*/
#define REG_GEU_FUSE_VAL_OEM_HASH_1_8              AES_ADDR(REGS_GEU_BASE, 0X444)/*Fuse Value for OEM Hash Key Registers 1- 8*/
#define REG_GEU_FUSE_STATUS                        AES_ADDR(REGS_GEU_BASE, 0X484)/*Fuse Operation Status Register*/
#define REG_GEU_HW_RANDOM_NUM_GEN                  AES_ADDR(REGS_GEU_BASE, 0X488)/*Hardware Random Number Generator Register*/
#define REG_GEU_FUSE_BANK0_191_160                 AES_ADDR(REGS_GEU_BASE, 0X48C)/*Fuse Bank0 191 160 Register*/
#define REG_GEU_BLOCK3_RESERVED_0                      AES_ADDR(REGS_GEU_BASE, 0X490)/*Block 3 Reserved Fuse Bit Register 0*/
#define REG_GEU_BLOCK3_RESERVED_1                      AES_ADDR(REGS_GEU_BASE, 0X494)/*Block 3 Reserved Fuse Bit Register 1*/
#define REG_GEU_BLOCK3_RESERVED_2                      AES_ADDR(REGS_GEU_BASE, 0X498)/*Block 3 Reserved Fuse Bit Register 2*/
#define REG_GEU_BLOCK3_RESERVED_3                      AES_ADDR(REGS_GEU_BASE, 0X49c)/*Block 3 Reserved Fuse Bit Register 3*/
#define REG_GEU_BLOCK3_RESERVED_4                      AES_ADDR(REGS_GEU_BASE, 0X4A0)/*Block 3 Reserved Fuse Bit Register 4*/
#define REG_GEU_BLOCK3_RESERVED_5                      AES_ADDR(REGS_GEU_BASE, 0X4A4)/*Block 3 Reserved Fuse Bit Register 5*/
#define REG_GEU_FUSE_BANK0_159_128                 AES_ADDR(REGS_GEU_BASE, 0X4A8)/*Fuse Bank0 159 128 Register*/
#define REG_GEU_FUSE_BANK0_255_240                 AES_ADDR(REGS_GEU_BASE, 0X4C8)/*Fuse Bank0 255 240 Register*/

#define EFUSE_UUID_BASE               REG_GEU_FUSE_VAL_APCFG1                     /* bank 0,chip UUID */
#define EFUSE_HUK_BASE                REG_GEU_FUSE_VAL_ROOT_KEY1                  /* bank 1,Hardware Unique Key */
#define EFUSE_SSK_BASE                REG_GEU_FUSE_VAL_OEM_HASH_1_8               /* bank 2,Secret Symmetric Key */
#define EFUSE_BLOCK3_BASE             REG_GEU_BLOCK3_RESERVED_0                       /* bank3,Block 3 Reserved Fuse Bit Register 0*/

#define GEU_CONFIG                    REG_GEU_CONFIG
// add end

#define FUSE_BANK_SIZE               (256/32)
//clock
#define REG_PMU_AES_CLK_RES_CTRL  0xD4282868
#define BIT_PMU_AES_RST           (0x1<<0)  //Bit_0: AES_AXI_RST
#define BIT_PMU_AES_CLK_EN        (0x1<<3)  //Bit_3: AES_AXICLK_EN

/* bits definitions for register REG_GEU_CONFIG */
#define BIT_STICKY_CONTROL_BIT                            ( (0x1<<30) )
#define BIT_CBCECB_MODE                                   ( (0x1<<29) )
#define BIT_FUSE_POWER_BYPASS                             ( (0x1<<28) )
#define BITS_FUSE_CLOCK_DIVIDER(_X_)                      ( (_X_) << 25 & ((0x1<<25)|(0x1<<26)|(0x1<<27)) )
#define BIT_WRITE_IV_CBC_MODE                             ( (0x1<<24) )
#define BIT_FUSE_CLOCK_DISABLE                            ( (0x1<<23) )
#define BIT_FUSE_SOFTWARE_RESET                           ( (0x1<<22) )
#define BITS_FUSE_BLOCK_NUMBER(_X_)                       ( (_X_) << 18 & ((0x1<<18)|(0x1<<19)|(0x1<<20)|(0x1<<21)) )
#define BIT_HIGH_VOLT_ENABLE                              ( (0x1<<17) )
#define BIT_BURN_FUSE_ENABLE                              ( (0x1<<16) )
#define BIT_FUSE_LOCK                                     ( (0x1<<15) )
#define BIT_ENABLE_SOFT_FUSE_PROG                         ( (0x1<<14) )
#define BIT_SEL_FUSE_B                                    ( (0x1<<13) )
#define BIT_ENCDEC                                        ( (0x1<<3) )
#define BIT_OCBBYP                                        ( (0x1<<2) )

#define BIT_GE_FUSE_READY205                              ( (0x1<<1) )
#define BIT_FUSE_BURN_DONE                                ( (0x1<<0) )

/* bits definitions for register REG_GEU_STATUS*/
#define BIT_GEU_STATUS_DOUTREADY                          ( (0x1<<3) )
#define BIT_GEU_STATUS_KEYDONE                            ( (0x1<<2) )
#define BIT_GEU_STATUS_DINREADY                           ( (0x1<<1) )
#define BIT_GEU_STATUS_KEY_READY                          ( (0x1<<0) )

/* bit to detect whether under Security Access Disable status*/
#define BIT_GEU_GEU_FUSE_SECURITY_ACCESS                  ( (0x1<<29) )

/* bit to enable PRNG , to seed a random number generator*/
#define BIT_GEU_PRNG_ENABLE                               ( (0x1<<24) )
#define BIT_GEU_PRNG_REG_RANDOM_DEF_SEED                  ( (0x1A<<16) )

#define STICKY_CONTROL_BIT                  BIT_STICKY_CONTROL_BIT
#define FUSE_POWER_BYPASS                   BIT_FUSE_POWER_BYPASS
#define FUSE_CLOCK_DIVIDER(_X_)             BITS_FUSE_CLOCK_DIVIDER(_X_)
#define FUSE_CLOCK_DISABLE                  BIT_FUSE_CLOCK_DISABLE
#define FUSE_SOFTWARE_RESET                 BIT_FUSE_SOFTWARE_RESET
#define FUSE_BLOCK_NUMBER(_X_)              BITS_FUSE_BLOCK_NUMBER(_X_)
#define HIGH_VOLT_ENABLE                    BIT_HIGH_VOLT_ENABLE
#define BURN_FUSE_ENABLE                    BIT_BURN_FUSE_ENABLE
#define FUSE_LOCK                           BIT_FUSE_LOCK
#define ENABLE_SOFT_FUSE_PROG               BIT_ENABLE_SOFT_FUSE_PROG
#define SEL_FUSE_B                          BIT_SEL_FUSE_B

/* bits definitions for register REG_GEU_FUSE_STATUS */
#define FUSE_READY                          BIT_GE_FUSE_READY205
#define FUSE_BURN_DONE                      BIT_FUSE_BURN_DONE

typedef enum _type_AES_ENC_MODE {
	ECB,
	CBC,
}AES_MODE;

typedef enum _type_AES_KEY_SIZE {
	key_size_128=(0x1<<0),
	key_size_192=(0x1<<1),
	key_size_256=((0x1<<0)|(0x1<<1)),
}AES_KEY_SIZE;
#define AES_KEY_SIZE_TO_U32_LEN(x) (2*(x)+2)

typedef enum _type_AES_KEY_SOURCE {
	FUSE,
	REGS,
}AES_KEY_SOURCE;

typedef struct _type_AES_ARGS {
	AES_MODE          mode;
	AES_KEY_SOURCE    keysource;
	AES_KEY_SIZE      keysize;
	unsigned int      len;
	const char *      ikey;
	const char *      input;
	const char *      output;
	const char *      iv;
}AES_ARGS;

void aes_decrypt(AES_ARGS aes_arg);
void aes_encrypt(AES_ARGS aes_arg);
unsigned int geu_random_number(void);
unsigned int pseudo_random_number(void);
