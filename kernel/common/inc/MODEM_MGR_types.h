/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/


/* ===========================================================================
File        : MODEM_MGR_types.h
Description : Data types file for the aplp/MODEM_MGR package

Notes       : 

Copyright (c) 2001 Intel CCD. All Rights Reserved
=========================================================================== */

#if !defined(_MODEM_MGR_TYPES_H_)
#define _MODEM_MGR_TYPES_H_

#include "gbl_types.h"

/* ---------------------------------------------------------------------------
Enum name   : MODEM_MGRExampleEnum
Description : This is an example MODEM_MGR enumerated type
Notes       : Modify as required
--------------------------------------------------------------------------- */
enum _MODEM_MGRExampleEnum
{
  MODEM_MGR_first,
  MODEM_MGR_second,
  MODEM_MGR_MAX
};

typedef UINT8 MODEM_MGRExampleEnum;

/* ---------------------------------------------------------------------------
Struct name : MODEM_MGRExampleType
Description : This is an example MODEM_MGR data type
Notes       : Modify as required
--------------------------------------------------------------------------- */
typedef struct _MODEM_MGRExampleType
{
  UINT8 first;
  UINT8 second;
  BOOL  third;
} MODEM_MGRExampleType;


#endif /* _MODEM_MGR_TYPES_H_ */


/*                      end of MODEM_MGR_types.h
--------------------------------------------------------------------------- */




