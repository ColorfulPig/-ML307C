#ifndef __CUSTOM_GNSS_H__
#define __CUSTOM_GNSS_H__

#include <stdint.h>
#include <string.h>

#include "cm_gnss.h"
#include "cm_mem.h"
#include "cm_os.h"
#include "cm_sys.h"

enum
{
	GNSS_FIX_INVALID = 1,
	GNSS_FIX_2D = 2,
	GNSS_FIX_3D = 3,
};

typedef struct{
    float latitude;                 /** latitude,单位度,默认值为0 */
    float longitude;                /** longitude,单位度,默认值为0 */
    float hdop;                     /** 水平精度因子，保留1位小数,默认值为0 */
    float altitude;                 /** 海拔高度，单位米，保留1位小数,默认值为0 */
    uint8_t fix;                    /** 定位类型。Fix status. 1=Fix not available, 2=2D, 3=3D */
    float cog;                      /** 运动角度，真北参照系，单位:度,默认值为0 */
    float spkm;                     /** 水平运动速度，单位Km/h,默认值为0 */
    float spkn;                     /** 水平运动速度，单位Knots,默认值为0 */
    uint8_t nsat;                   /** 参与定位的卫星数,默认值为0 */
    uint8_t dtype;                  /** 差分定位标识,默认值为0 *//*不支持*/
	
    uint8_t vsat;                   /** 可视的卫星数,默认值为0 */
} gnss_location_info_t;


extern gnss_location_info_t gnss_location;

int custom_gnss_init(void);
int	custom_gnss_enable(uint8_t enable);

#endif

