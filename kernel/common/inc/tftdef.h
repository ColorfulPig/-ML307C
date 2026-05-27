#ifndef __TFT_DEF_H__
#define __TFT_DEF_H__
#include "global_types.h"

#define LWIP_IP_DEF

typedef struct {
        unsigned int  min;           //low limit
        unsigned int  max;          //max limit
} NumericRange;

typedef struct _TFTNode{
	struct _TFTNode *pre;
	struct _TFTNode *next;
}TFTNode;
typedef struct {
        unsigned int  direction;
		Bool directionPresent;
		UINT8            pIdNextHdr;    		/**< Protocol number (ipv4) / next header (ipv6) */
		Bool        pIdNextHdrPresent; 	/**< TRUE: if present \sa CCI API Ref Manual */ //Michal
        NumericRange localPortRange;
        NumericRange RemotePortRange;
        Bool  localPortRangePresent;
        Bool  RemotePortRangePresent;
        unsigned char  SecondaryCID;
        unsigned char  PfIdx;        // Packet Filter ID
        unsigned char  EpIdx;       // Evaluation Precedence Index
        Bool  TosPresent;
        unsigned char  Tos;         // Type of Service
        unsigned char  TosMask;         // Type of Service Mask
        Bool  SpiPresent;
        Bool  FlowLablePresent;
        unsigned int SPI;
        unsigned int  FlowLable;
		unsigned char IPType;
        unsigned int  RemoteAddress[4];
		Bool  RemoteAddressPresent;
        unsigned int  SubnetMask[4];
		Bool  SubnetMaskPresent;
		unsigned int	maxULRate;			/**<UL_MBR Max Bit Rate, Uplink, in kbps */
		unsigned int	maxDLRate;			/**<DL_MBR Max Bit Rate, Downlink, in kbps (MAX 256000kbps) */
		unsigned int	guaranteedULRate;	/**<UL_GBR Guaranteed Bit Rate, Uplink, in kbps */
		unsigned int	guaranteedDLRate;	/**<DL_GBR Guaranteed Bit Rate, Downlink, in kbps */
		void *next;
} PacketFilterInfo;

//used by TFT module
typedef struct {
		unsigned char bearer_id;
		unsigned char cid;
		unsigned char Pcid;
		unsigned char bearer_type;
		PacketFilterInfo  packetinfo;
} TftInfoList;

typedef struct _PacketFilterInfoNode{
	TFTNode header;
	TftInfoList info;
}PacketFilterInfoNode;


#endif

