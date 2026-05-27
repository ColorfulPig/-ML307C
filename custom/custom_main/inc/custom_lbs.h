#ifndef __CUSTOM_LBS_H__
#define __CUSTOM_LBS_H__

#include "stdlib.h"
#include "cm_sys.h"
#include "cm_lbs.h"
#include "cm_os.h"

typedef struct
{
    cm_lbs_callback_event_e state;  /*!<定位结果状态*/  
    float longitude;                /*!<经度*/
    float latitude;                 /*!<纬度*/ 
    float radius;                   /*!<精度半径*/  
}custom_lbs_location_t;


extern int lbs_location_Update;
extern custom_lbs_location_t lbs_location;

int custom_lbs_init(void);
int custom_lbs_start(cm_lbs_location_platform_e lbs_platform);


#endif


