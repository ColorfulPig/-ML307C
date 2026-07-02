#ifndef __CUSTOM_TEST_H__
#define __CUSTOM_TEST_H__

#include "cm_os.h"


/* 初始化测试命令处理任务。 */
int custom_test_init(void);
/* 处理 USB 测试命令并模拟蓝牙、BMS、云端或 FOTA 输入。 */
int custom_test_OnBlock(uint8_t *buf,uint32_t len);

#endif


