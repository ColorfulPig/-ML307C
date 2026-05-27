// 串口协议扫描分析

#include "custom_sps.h"
#include "custom_bms.h"
#include "custom_track.h"
#include "custom_bluetooth.h"
#include "custom_cloud.h"
#include "custom_test.h"
#include "custom_common.h"

//串口0支持的协议-bms
const TPTC_PROC ProcSci0Tab[] = 
{
	{ custom_bms_OnChar, custom_bms_OnFrame, custom_bms_OnFinish, custom_bms_OnBlock },
};
	
//串口1支持的协议-gnss
const TPTC_PROC ProcSci1Tab[] = 
{
	{ NULL, NULL, NULL, NULL },
};

//串口2支持的协议-bluetooth
const TPTC_PROC ProcSci2Tab[] = 
{
	{ NULL, NULL, NULL, custom_bluetooth_OnBlock },
};

//usb支持的协议-debug
const TPTC_PROC ProcUsbTab[] = 
{
	{ NULL, NULL, NULL, custom_test_OnBlock },
};

//串口cloud支持的协议-onenet
const TPTC_PROC ProcCloudTab[] = 
{
	{ custom_cloud_OnChar, custom_cloud_OnFrame, custom_cloud_OnFinish, NULL },
};

//串口0协议扫描
void Sps_InUart0Scan(uint8_t *pv_Buffer,uint16_t v_Length)
{
	uint16_t i;
	int aI,modbus;
	uint8_t result;
	const TPTC_PROC * pItem;

	SYSTEM_printRaw("Sps_InUart0Scan(BMS):", pv_Buffer, v_Length);
	SYSTEM_printHex("Sps_InUart0Scan(BMS):", pv_Buffer, v_Length);
	
	pItem = &ProcSci0Tab[0];
	for ( i=0; i<_countof(ProcSci0Tab); ++i,++pItem )
	{
		modbus = 0;
		if (pItem->inBlockScan)
		{
			modbus = (*pItem->inBlockScan)(pv_Buffer,v_Length);
		}

		if(modbus == 0)		// 非modbus协议
		{
			for(aI=0;aI<v_Length;aI++)
			{
				if (pItem->inCharScan)
				{
					result = (*pItem->inCharScan)(pv_Buffer[aI]);
				}
			
				if(result==TPTC_R_FRAME)
				{
					if (pItem->inFrameScan)
						(*pItem->inFrameScan)();
					if (pItem->inFinishScan)
						(*pItem->inFinishScan)();				
				}
				else if(result==TPTC_R_FALSE)
				{
					if (pItem->inFinishScan)
						(*pItem->inFinishScan)();
				}
			}
		}
	}	
}

//串口1协议扫描
void Sps_InUart1Scan(uint8_t *pv_Buffer,uint16_t v_Length)
{
	uint16_t i;
	int aI;
	uint8_t result;
	const TPTC_PROC * pItem;
	
	SYSTEM_printRaw("Sps_InUart1Scan:", pv_Buffer, v_Length);
	
	pItem = &ProcSci1Tab[0];
	for ( i=0; i<_countof(ProcSci1Tab); ++i,++pItem )
	{
		if (pItem->inBlockScan)
		{
			(*pItem->inBlockScan)(pv_Buffer,v_Length);
		}

		for(aI=0;aI<v_Length;aI++)
		{
			if (pItem->inCharScan)
			{
				result = (*pItem->inCharScan)(pv_Buffer[aI]);
			}
		
			if(result==TPTC_R_FRAME)
			{
				if (pItem->inFrameScan)
					(*pItem->inFrameScan)();
				if (pItem->inFinishScan)
					(*pItem->inFinishScan)();				
			}
			else if(result==TPTC_R_FALSE)
			{
				if (pItem->inFinishScan)
					(*pItem->inFinishScan)();
			}
		}
	}	
}

//串口2协议扫描
void Sps_InUart2Scan(uint8_t *pv_Buffer,uint16_t v_Length)
{
	uint16_t i;
	int aI;
	uint8_t result;
	const TPTC_PROC * pItem;
	
	SYSTEM_printRaw("Sps_InUart2Scan(BT):", pv_Buffer, v_Length);
	
	pItem = &ProcSci2Tab[0];
	for ( i=0; i<_countof(ProcSci2Tab); ++i,++pItem )
	{
		if (pItem->inBlockScan)
		{
			(*pItem->inBlockScan)(pv_Buffer,v_Length);
		}

		for(aI=0;aI<v_Length;aI++)
		{
			if (pItem->inCharScan)
			{
				result = (*pItem->inCharScan)(pv_Buffer[aI]);
			}
		
			if(result==TPTC_R_FRAME)
			{
				if (pItem->inFrameScan)
					(*pItem->inFrameScan)();
				if (pItem->inFinishScan)
					(*pItem->inFinishScan)();				
			}
			else if(result==TPTC_R_FALSE)
			{
				if (pItem->inFinishScan)
					(*pItem->inFinishScan)();
			}
		}
	}	
}

//usb协议扫描
void Sps_InUsbScan(uint8_t *pv_Buffer,uint16_t v_Length)
{
	uint16_t i;
	int aI;
	uint8_t result;
	const TPTC_PROC * pItem;
	
	pItem = &ProcUsbTab[0];
	for ( i=0; i<_countof(ProcUsbTab); ++i,++pItem )
	{
		if (pItem->inBlockScan)
		{
			(*pItem->inBlockScan)(pv_Buffer,v_Length);
		}

		for(aI=0;aI<v_Length;aI++)
		{
			if (pItem->inCharScan)
			{
				result = (*pItem->inCharScan)(pv_Buffer[aI]);
			}
		
			if(result==TPTC_R_FRAME)
			{
				if (pItem->inFrameScan)
					(*pItem->inFrameScan)();
				if (pItem->inFinishScan)
					(*pItem->inFinishScan)();				
			}
			else if(result==TPTC_R_FALSE)
			{
				if (pItem->inFinishScan)
					(*pItem->inFinishScan)();
			}
		}
	}	
}

// cloud协议扫描
void Sps_InCloudScan(uint8_t *pv_Buffer,uint16_t v_Length)
{
	uint16_t i;
	int aI;
	uint8_t result;
	const TPTC_PROC * pItem;
	
	pItem = &ProcCloudTab[0];
	for ( i=0; i<_countof(ProcCloudTab); ++i,++pItem )
	{
		if (pItem->inBlockScan)
		{
			(*pItem->inBlockScan)(pv_Buffer,v_Length);
		}

		for(aI=0;aI<v_Length;aI++)
		{
			if (pItem->inCharScan)
			{
				result = (*pItem->inCharScan)(pv_Buffer[aI]);
			}
		
			if(result==TPTC_R_FRAME)
			{
				if (pItem->inFrameScan)
					(*pItem->inFrameScan)();
				if (pItem->inFinishScan)
					(*pItem->inFinishScan)();				
			}
			else if(result==TPTC_R_FALSE)
			{
				if (pItem->inFinishScan)
					(*pItem->inFinishScan)();
			}
		}
	}	
}

