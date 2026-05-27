/*
* Copyright (C) 2023 ASR Microelectronic Ltd.
*
* Author: Shuo Dai <shuodai@asrmicro.com>
*
* This file contains proprietary information.
* No dissemination allowed without prior written permission from
* ASR Microelectronic Ltd.
*
* File Description:
*
* This file implements proxy for audio platform driver
*/

/*
 *    对于使用PWM输出模拟音频信号模式的用户，设备上电默认为该模式，无需调用vpathSetMode(0)接口进行设置
 *    对于使用输出输入PCM数字音频信号模式（外置CODEC）的用户，设备上电后需调用vpathSetMode(1)接口进行设置
 *    不建议多次调用vpathSetMode()接口反复切换模式和播放数据，此操作可能导致数据异常
 */
int vpathSetMode(uint32_t mode);//0 for pwm, 1 for external codec