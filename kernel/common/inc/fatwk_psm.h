#ifndef FAT_WK_PSM

#include "platform.h"
#include "psm.h"

#define FAT_WK_PSM
#define PSM_BLK_SIZE				0x800  //2k
//#define FLASH_PSM_START_ADDR	0x80FF8000
//#define FLASH_PSM_START_ADDR	0xFF8000
#define FLASH_PSM_START_ADDR	0x3C000
#define FLASE_SECTOR_SIZE   512

//#define FLASH_BLK_SIZE				(GetFlashProperties(BOOT_FLASH)->BlockSize)
#define FLASH_BLK_SIZE				(PSM_BLK_SIZE)
#if 0
//use this address temporarily
#define FLASHPARTITION_PSM_BLK0 	(GetFlashLayoutConfig()->PSMStartAddress)
#define FLASHPARTITION_PSM_BLK1 	(FLASHPARTITION_PSM_BLK0 + FLASH_BLK_SIZE)
#define FLASHPARTITION_PSM_BLK2     (FLASHPARTITION_PSM_BLK1 + FLASH_BLK_SIZE)
#define FLASHPARTITION_PSM_BLK3     (FLASHPARTITION_PSM_BLK2 + FLASH_BLK_SIZE)

#define FLASHPARTITION_PSM_REV0		(FLASHPARTITION_PSM_BLK3 + FLASH_BLK_SIZE)
#define FLASHPARTITION_PSM_REV1		(FLASHPARTITION_PSM_REV0 + FLASH_BLK_SIZE)
#define FLASHPARTITION_PSM_REV2		(FLASHPARTITION_PSM_REV1 + FLASH_BLK_SIZE)
#endif
#define FLASHPARTITION_PSM_BLK0		(FLASH_PSM_START_ADDR)

//#define SMS_ME_SUPPORT
//#ifdef LTEONLY_THIN_SINGLE_SIM_2MFLASH
//#define PSM_FS_COMPACT
//#endif

#define TOTAL_PSMFILE_NUM		    1
#define PSMBLK_REV_NUM				1

#if PSM_PLATFORM_NEZHA_DEFINED
#ifdef SMS_ME_SUPPORT
#undef TOTAL_PSMFILE_NUM
#define TOTAL_PSMFILE_NUM		    2
#endif
#endif

#if PSM_PLATFORM_CRANE_DEFINED
#define PSM_DISABLE_RES
#if defined (PSM_FS_COMPACT) || defined(PSM_DISABLE_RES)
#undef PSMBLK_REV_NUM
#define PSMBLK_REV_NUM				0
#undef TOTAL_PSMFILE_NUM
#define TOTAL_PSMFILE_NUM				1
#endif
#endif

//total psm block number
#define TOTAL_PSMBLK_NUM 			(TOTAL_PSMFILE_NUM + PSMBLK_REV_NUM)

#define PSM_HEADER_LEN 				sizeof(Psm_Block_Info)

#define PSM_DATA_OFFSET				(PSM_HEADER_LEN - 4)
#define PSM_DATA_LEN				(PSM_BLK_SIZE - PSM_HEADER_LEN)

//for reserved psm block
#define INVALID_PSM_LEN				0xff00ff00

#define PSM_MAGIC_NUM1					0x5a5a5a5a
#define PSM_MAGIC_NUM2					0xa5a5a5a5

//#define PSM_Debug(format,...) 			do{}while(0)
#define PSM_Debug(fmt,args...) CPUartLogPrintf("[psm]"fmt, ##args)
#define PSM_Error(fmt,args...) CPUartLogPrintf("[psm]"fmt, ##args)

//store in flash
typedef struct
{
	unsigned int magic_num1;
//	unsigned int dirty;		// 1 psm wirte back to flash, 0 flash block not program
//	int idx; 			//point to the index of physical psm file, 0xa5a5a5a5 for reserver
	unsigned char filename[12];
	unsigned int len;
	unsigned int crc;
	unsigned int random;
	unsigned int magic_num2;		//store in the end of flash block
}Psm_Block_Info;

typedef enum
{
    BOOT_FLASH			= 0,
    SAVE_STATE_FLASH	= 1
} FlashBootType_T;

void psm_lowlevel_init(void);
void psm_lock_high(void);
void psm_unlock_high(void);
void psm_lock_low(void);
void psm_unlock_low(void);
int FDI_remove_psm(const char *filename_ptr);
FILE_ID FDI_fopen_psm(const char *filename_ptr, const char *mode);
size_t FDI_fread_defaultpsm(void *buff, size_t element_size, size_t count, FILE_ID stream);
size_t FDI_fwrite_defaultpsm(void *buff, size_t element_size, size_t count, FILE_ID stream);
#if PSM_PLATFORM_NEZHA_DEFINED
size_t FDI_fread_psm(size_t element_size, size_t count, FILE_ID stream);
size_t FDI_fwrite_psm(size_t element_size, size_t count, FILE_ID stream);
int FDI_fseek_psm(FILE_ID stream, long offset, int wherefrom);
#endif
int FDI_fclose_psm(FILE_ID stream);
unsigned int get_psm_len(char handleIndex);

#endif