
#ifndef	SamsungCNG_H
#define SamsungCNG_H

/****************************************************************************************
 *  CNG Rx Vol
 ****************************************************************************************/

//ICAT EXPORTED ENUM
typedef enum
{	
ARTIFICIALBACKGROUNDNOISE_LOW	=	1,		// RMS -90dB
ARTIFICIALBACKGROUNDNOISE_MED	=	2,		// RMS -84dB
ARTIFICIALBACKGROUNDNOISE_HIGH	=	3,		// RMS -81dB

DIAMONDVOICE_ARTIFICIALBACKGROUNDNOISE_ENUM_32_BIT	  = 0x7FFFFFFF //32bit enum compiling enforcement 
}DiamondVoice_cngLevel;

/******************************************************************************
 *  TYPEDEFS AND STRUCTURES
 *****************************************************************************/
typedef struct{
short monitor1;				// CNG Gain
short monitor2;
short monitor3;
short monitor4;
short monitor5;
} CNGState_type;

/******************************************************************************
 *  FUNCTION PROTOTYPES
 *****************************************************************************/
extern int SamsungCNGState(void);

#endif		// SamsungCNG_H

