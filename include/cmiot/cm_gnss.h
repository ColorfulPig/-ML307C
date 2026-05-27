/**
 * @file        cm_gnss.h
 * @brief       GNSS接口
 * @copyright   Copyright © 2021 China Mobile IOT. All rights reserved.
 * @GNSS API
      注意：
      1、不支持使用差分定位
 * @author      By cmiot
 * @date        2025/07/30
 *
 * @defgroup gnss
 * @ingroup PI
 * @{
 */

#ifndef __CM_GNSS_H__
#define __CM_GNSS_H__

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdint.h>
#include <time.h>
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
 /**NMEA 协议信息输出类型宏定义*/
#define CM_GNSS_NAME_GGA    (1<<0)
#define CM_GNSS_NAME_GSV    (1<<1)
#define CM_GNSS_NAME_GSA    (1<<2)
#define CM_GNSS_NAME_RMC    (1<<3)
#define CM_GNSS_NAME_VTG    (1<<4)
#define CM_GNSS_NAME_GLL    (1<<5)
#define CM_GNSS_NAME_ZDA    (1<<6)
#define CM_GNSS_NAME_DTM    (1<<7)
#define CM_GNSS_NAME_GNS    (1<<8)
#define CM_GNSS_NAME_GRS    (1<<9)
#define CM_GNSS_NAME_GST    (1<<10)
#define CM_GNSS_NAME_GBS    (1<<11)
#define CM_GNSS_NAME_TXT    (1<<12)

/*导航系统选择宏定义*/
#define CM_GNSS_TYPE_GPS  	0x01        // GPS(GPxxx)
#define CM_GNSS_TYPE_BDS  	0x02        // BDS
#define CM_GNSS_TYPE_QZSS 	0x04        // 准天顶-日本
#define CM_GNSS_TYPE_GLO  	0x08        // GLONASS-俄罗斯
#define CM_GNSS_TYPE_GAL  	0x10        // 伽利略-欧盟

/****************************************************************************
 * Public Types
 ****************************************************************************/

 /** agnss output mode */
typedef enum{
    CM_AGNSS_DISABLE = 1,
    CM_AGNSS_ENABLE, 
} cm_agnss_enable_e;
     
 typedef struct{
     float latitude;                 /** latitude,单位度， 保留4位小数 */
     float longitude;                /** longitude,单位度，保留4位小数 */
     uint8_t mode;                   /** 辅助位置模式，整型，1-单次有效(使用之后清除）;2-自动更新(允许根据位置信息自动更新） */
 } cm_agnss_location_t;

 /** gnss config type */
 typedef enum{
    CM_GNSS_CONFIG_TYPE_NMEA_MASK = 1,       /** NMEA输出配置，设置值(value)对应的bit置1表示开启对应输出*/
                                             /** 例如： CM_GNSS_NAME_GGA|CM_GNSS_NAME_GSV */
    CM_GNSS_CONFIG_TYPE_NMEA_CYCLE,          /** NMEA输出周期（1-60）秒，整型*/
    CM_GNSS_CONFIG_TYPE_NMEA_PORT,           /** 调整NMEA数据输出接口（字符串） *//*不支持*/
    CM_GNSS_CONFIG_TYPE_AGNSS_LOCATION,      /** AGNSS辅助定位注入位置信息设置,cm_agnss_location_t */
    CM_GNSS_CONFIG_TYPE_AGNSS_TIME,          /** AGNSS辅助定位注入时间信息设置,struct tm,UTC time *//*不支持*/
    CM_GNSS_CONFIG_TYPE_AGNSS_URL,           /** AGNSS辅助定位服务器url，字符串（hostname:port,≤64字节） *//*不支持设置,ASR默认服务器*/
    CM_GNSS_CONFIG_TYPE_AGNSS_INTERVAL,      /** AGNSS辅助定位数据自动更新间隔，（30-240）分钟，整型,默认值为0（禁止自动更新） */
    CM_GNSS_CONFIG_TYPE_DGNSS_URL,           /** DGNSS差分定位服务器url，字符串（hostname:port，≤64字节） *//*不支持*/
    CM_GNSS_CONFIG_TYPE_DGNSS_USER,          /** DGNSS差分定位用户名，字符串（≤64字节） *//*不支持*/
    CM_GNSS_CONFIG_TYPE_DGNSS_PWD,           /** DGNSS差分定位密码，字符串（≤64字节） *//*不支持*/
    CM_GNSS_CONFIG_TYPE_DGNSS_MOUNT,         /** DNGSS差分定位挂载点，字符串（≤64字节） *//*不支持*/
    CM_GNSS_CONFIG_TYPE_DGNSS_INTERVAL,      /** DGNSS差分定位数据接收间隔，秒，整型,默认值为1（>=1） *//*不支持*/
    CM_GNSS_CONFIG_TYPE_SLEEP,               /** GPS芯片低功耗设置 ，0，全功能运行；1，休眠，整型*/  /*不支持*/
} cm_gnss_config_type_e;
    

typedef struct{
    float latitude;                 /** latitude,单位度,默认值为0 */
    float longitude;                /** longitude,单位度,默认值为0 */
    float hdop;                     /** 水平精度因子，保留1位小数,默认值为0 */
    float altitude;                 /** 海拔高度，单位米，保留1位小数,默认值为0 */
    uint8_t fix;                    /** 定位类型。Fix status. `0` = invalid, `1` = GPS fix, `2` = DGPS fix, `3` = PPS fix,默认值为0 */
    float cog;                      /** 运动角度，真北参照系，单位:度,默认值为0 */
    float spkm;                     /** 水平运动速度，单位Km/h,默认值为0 */
    float spkn;                     /** 水平运动速度，单位Knots,默认值为0 */
    uint8_t nsat;                   /** 参与定位的卫星数,默认值为0 */
    uint8_t dtype;                  /** 差分定位标识,默认值为0 *//*不支持*/
} cm_gnss_location_info_t;

typedef enum{
    CM_AGNSS_UPDATE_INVALID = 0,       /** 辅助信息无效 */
    CM_AGNSS_UPDATE_VALID,             /** 辅助信息有效 */
    CM_AGNSS_UPDATE_FAIL_JUDGE,        /** 无法判断有效性  */
    CM_AGNSS_UPDATE_VOID,              /** 辅助数据为空 */
} cm_agnss_update_result_e;
 

/**
 * @brief NMEA完整报文回调函数
 *
 * @param [in] nmea  NMEA 报文数据
 * @param [in] len  NMEA 报文长度
 *
 * @return void
 *
 * @details  此回调函数用于上报完整的NMEA报文
 */
typedef void (*cm_gnss_nmea_all_callback)(const char *nmea, uint32_t len);

/**
 * @brief NMEA信息回调函数
 *
 * @param [in] type  NMEA 报文类型(例如：CM_GNSS_NAME_GGA | CM_GNSS_NAME_GSV)
 * @param [in] nmea  NMEA 报文数据
 * @param [in] len  NMEA 报文长度
 *
 * @return void
 *
 * @details  此回调函数用于分类上报NMEA报文，每句NMEA报文会触发一次回调
 */
typedef void (*cm_gnss_nmea_callback)(uint32_t type, const char *nmea, uint32_t len);

/**
 * @brief 辅助信息更新结果回调函数
 *
 * @param [in] mode        辅助数据更新结果：0-辅助信息无效；1-辅助信息有效；2-无法判断有效性；3-辅助数据为空；
 * @param [in] update_time 辅助数据更新时间，年月日时分秒，各占2个字符
 * @param [in] size        辅助数据大小（不支持此查询，永远为0）
 *
 * @return void
 *
 * @details  须在注册回调函数时传入
 */
typedef void (*cm_agnss_data_callback)(cm_agnss_update_result_e mode, const char *update_time, uint32_t size);

/**
 * @brief 发送透传信息给GPS芯片返回结果回调函数
 *
 * @param [in] data 返回结果数据
 *
 * @return void
 *
 * @details  须在注册回调函数时传入
 */
typedef void (*cm_gnss_rawdata_rsp_callback)(const char *data, uint32_t len);

/****************************************************************************
 * Public Data
 ****************************************************************************/


/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/
#ifdef __cplusplus
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/**
 *  @brief gnss 功能打开
 *  
 *  @param [in] gnss_type gnss 导航系统选择
 *  @param [in] agnss_enable   是否打开辅助定位
 *  
 *  @return  
 *    = 0  - 成功 \n
 *    = -1 - 失败
 *
 *  @details 若需使用AGNSS，建议先使用cm_agnss_data_start_update
 *  先下载AGNSS数据。
 */
int32_t cm_gnss_open(uint8_t gnss_type, cm_agnss_enable_e agnss_enable);

/**
 *  @brief gnss 功能关闭
 *  
 *  @param [in] void
 *  
 *  @return void 
 */
void cm_gnss_close(void);

/**
 *  @brief gnss 参数信息配置
 *  
 *  @param [in] config_type 参数类型
 *  @param [in] value       相应参数值
 *  
 *  @return  
 *    = 0  - 成功 \n
 *    = -1 - 失败,
 */
 
int32_t cm_gnss_config(cm_gnss_config_type_e config_type, void *value);

/**
 *  @brief gnss 注册NMEA 消息回调函数
 *  
 *  @param [in] callback  回调函数设置
 *  @param [in] nmea_type 输出报文类型(例如：CM_GNSS_NAME_GGA | CM_GNSS_NAME_GSV)
 *  
 *  @return 
 *    = 0  - 成功 \n
 *    = -1 - 失败
 *
 */
int32_t cm_gnss_set_nmea_callback(cm_gnss_nmea_callback callback, uint32_t nmea_type);

/**
 *  @brief gnss 重启
 *  
 *  @param [in] restart_mode 重启模式
 *  
 *  @return
 *    = 0  - 成功 \n
 *    = other - 失败
 *  @details 仅内置的GNSS支持重启，外置不支持（无法确定外置芯片）。
 */
int32_t cm_gnss_reset(uint8_t restart_mode);


/**
 *  @brief 查询辅助定位数据是否有效
 *  
 *  @param [out] mode        辅助数据更新结果：0-辅助信息无效；1-辅助信息有效；2-无法判断有效性；3-辅助数据为空；
 *  @param [out] update_time 辅助数据更新时间，年月日时分秒
 *  @param [out] update_time_len 辅助数据更新时间长度（14个字节）
 *  @param [out] size        辅助数据大小（不支持）
 *  
 *  @return  
 *    = 0  - 成功 \n
 *    = -1 - 失败
 *
   @details 辅助数据有效性120分钟。
 */
int32_t cm_agnss_state_query(cm_agnss_update_result_e *agnss_mode, char *update_time, uint32_t update_time_len, uint32_t *data_size);


/**
 *  @brief 更新辅助定位数据
 *  
 *  @param [in] callback 更新结果回调函数
 *  
 *  @return  
 *    = 1  - 成功 \n
 *    = 0  - business\n
 *    = -1 - 失败
 *
  *  @details 
 */
int32_t cm_agnss_data_start_update(cm_agnss_data_callback callback);

/**
 *  @brief dgnss 功能打开
 *  
 *  @param [in] void
 *  
 *  @return  
 *    = 0  - 成功 \n
 *    = -1 - 失败
 *
 *  @details 不支持。
 */
int32_t cm_dgnss_enable(void);

/**
 *  @brief dgnss 功能关闭
 *  
 *  @param [in] void
 *  
 *  @return  
 *    = 0  - 成功 \n
 *    = -1 - 失败
*
 *  @details 不支持。
 */
int32_t cm_dgnss_disable(void);

/**
 *  @brief 向GPS芯片发送命令
 *  
 *  @param [in] raw_data 原始数据指针地址
 *  @param [in] len 原始数据长度
 *  
 *  @return  
 *    = 0  - 成功 \n
 *    = -1 - 失败
 *
 *  @details GPS芯片的返回值，在注册GPS芯片原始数据回调中获取。
 */
int32_t cm_gnss_send_raw_data(const char *raw_data, uint32_t len);


/**
*  @brief 注册完整NMEA数据上报
*
*  @return	
*	 = 0  - 成功 \n
*	 = -1 - 失败
*
*  @details 
*/
int32_t cm_gnss_nmea_all_callback_regist(cm_gnss_nmea_all_callback callback);


/**
*  @brief 注册GPS芯片原始数据的上报
*
*  @return	
*	 = 0  - 成功 \n
*	 = -1 - 失败
*
*  @details 回调数据用于调试使用
*/
int32_t cm_gnss_rawdata_callback_regist(cm_gnss_rawdata_rsp_callback callback);

/**
 *  @brief 获取定位信息
 *  
 *  @param [in] info 定位信息
 *  
 *  @return	
 *	 = 0  - 成功 \n
 *	 其他 - 失败
 *
 *  @details 
 */
 int32_t cm_gnss_get_location_info(cm_gnss_location_info_t *info);

/**
 *  @brief 主动请求NMEA数据，请求成功，将触发cm_gnss_set_nmea_callback注册的回调
 *  
 *  @param [in] void
 *  
 *  @return  
 *    = 0  - 成功 \n
 *    = -1 - 失败
 * 
 * @details 自动上报与主动请求NMEA数据请勿同时使用，否则可能会导致NMEA数据出现异常
 */
int32_t cm_gnss_req_nmea(void);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif /* __CM_GNSS_H__ */

/** @}*/

