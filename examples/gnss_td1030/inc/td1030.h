/**
 * @copyright Copyright (C) 2012-2024 ChinaMobile IoT
 * 
 * @file td1030.h
 * @brief 
 * @author zhaoji
 * @version 1.0
 * @date 2024-11-18
 */

#ifndef __TD1030_H__
#define __TD1030_H__


#define TD1030_DEMO_UART        CM_UART_DEV_0
#define TD1030_DEMO_RX_SIZE     (2 * 1024)

#define NMEA_KEYWORD_RMC        "GNRMC"
#define NMEA_KEYWORD_GSA        "GNGSA"
#define NMEA_KEYWORD_GSV        "GPGSV"

typedef struct
{
    int state;        // 定位状态 0 表示定位失败， 1 表示 GPS 定位成功， 2 表示 GPS 经纬度信息非实时，而是最近保存的经纬度信息
    char E_W;         // 东经、西经
    double longitude; // 经度
    char N_S;         // 南纬、北纬
    double latitude;  // 纬度
    double speed;     // 速度(km/h)
    double accuracy;  // 定位精确度(m)
    int sate_num;     // 可用卫星数量(35db以上)
} gnss_info_t;


#endif  // __TD1030_H__