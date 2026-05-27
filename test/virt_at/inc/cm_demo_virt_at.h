
#include <stdlib.h>
#include <stdio.h>
#include "cm_sys.h"
#include "cm_virt_at.h"
#include "cm_common.h"
#ifndef __CM_DEMO_VIRT_AT_H__
#define __CM_DEMO_VIRT_AT_H__


void cm_virt_at_test_sync(unsigned char **cmd,int len);
void cm_virt_at_test_async(unsigned char **cmd,int len);

#endif
