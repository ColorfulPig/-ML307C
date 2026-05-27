#ifndef __CM_DEMO_GPIO_H__
#define __CM_DEMO_GPIO_H__

#include <stdint.h>
 
void cm_test_gpio_read(unsigned char **cmd, int len);
void cm_test_gpio_write(unsigned char **cmd, int len);
void cm_test_gpio_irq(unsigned char **cmd, int len);
void cm_test_gpio_func(unsigned char **cmd, int len);
void cm_test_pin_cmd(unsigned char **cmd, int len);
void cm_test_acqure_irq_count(unsigned char **cmd, int len);
void cm_test_iomux(unsigned char **cmd, int len);

#endif

