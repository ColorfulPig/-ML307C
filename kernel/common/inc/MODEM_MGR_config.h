/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/


/* ===========================================================================
File        : MODEM_MGR_config.h
Description : Configuration parameters for the 
              aplp/MODEM_MGR package.

Notes       : These values can be overridden in gbl_config.h
              The range checks should be updated for each
              parameter.

Copyright (c) 2001 Intel CCD. All Rights Reserved
=========================================================================== */

#if !defined(_MODEM_MGR_CONFIG_H_)
#define _MODEM_MGR_CONFIG_H_

/* ---------------------------------------------------------------------------
Parameter   : MODEM_MGR <Example> Parameter
Description : MODEM_MGR parameter description 
Notes       : Why the range is what it is etc.
--------------------------------------------------------------------------- */
#define MODEM_MGR_<EXAMPLE>       <value>
#define MODEM_MGR_<EXAMPLE>_MIN	<min>
#define MODEM_MGR_<EXAMPLE>_STEP	<step>
#define MODEM_MGR_<EXAMPLE>_MAX	<max>


/* Include the global configuration file, so these values
   can be overridden */
#if defined(_GBL_CONFIG_H_)
#undef _GBL_CONFIG_H_
#endif
#include "gbl_config.h"

/* Check the <Example> Parameter Range */
#if (MODEM_MGR_<Example> < MODEM_MGR_<Example>_MIN)|| \
    (MODEM_MGR_<Example> > MODEM_MGR_<Example>_MAX)
#error "MODEM_MGR Package <Example> parameter out of range."
#endif

#endif /* _MODEM_MGR_CONFIG_H_ */


/*                      end of MODEM_MGR_config.h
--------------------------------------------------------------------------- */





