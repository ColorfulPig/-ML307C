# Keypad APP 简介

## **Keypad介绍**

矩阵键盘（Keypad）是一种常见的输入设备，通常用于电子设备中。它由多个按键组成，按键排列成一个矩阵形式，并通过连接线与控制器相连。

其基本工作原理是通过行列扫描技术来检测按键的状态。具体来说，矩阵键盘中的按键被分为若干行和列，行列交叉部分形成了一个二维矩阵。控制器会依次扫描每一行或每一列，当检测到某一个按键被按下时，在该按键所处的行和列之间建立联系，从而确定该按键的位置和状态。

这种工作原理使得矩阵键盘具有很高的灵活性和可编程性，因为可以通过编程设置哪些按键被映射到哪些行和列上。此外，矩阵键盘还具有体积小、成本低、反应速度快等优点，因此被广泛应用于各种类型的电子设备中。

## **APP实现功能**

本APP使用矩阵键盘，主要实现对矩阵键盘的应用功能，包括以下子功能：

1. 驱动矩阵键盘，本实例中为2*2规格；
2. 按下对应按键后，按键键值通过串口打印；
3. 矩阵键盘带休眠唤醒功能。

## **APP执行流程**

1. 开机进入主线程，开启app线程，初始化PM，UART，KEYPAD

   ```c
   /** app线程入口*/
   static void __task_loop(void *arg)
   {
       (void)arg;
       __cm_pm_init();
       __cm_lcd_lvgl_init();
       __cm_keypad_fuc_init();
       if(0 == cm_keypad_init())
       {
           cm_log_printf(0, "cm_keypad_init OK\n");
       }
       else
       {
           cm_log_printf(0, "cm_keypad_init FAILED\n");
       }
       keypad_queue = osMessageQueueNew(10, sizeof(cm_keypad_map_t), NULL);
       int keypad_event = 0;
   	while(1) 
       {
   		if(osMessageQueueGet(keypad_queue, &keypad_event, NULL, osWaitForever) == osOK)
           {
               cm_log_printf(0, "keypad_event:%x\n", keypad_event);
               __cm_keypad_dell(keypad_event);
           }
   		osDelay(5);
   	}
   }
   ```
2. PM初始化部分，休眠的进入和退出都可以设置回调函数，此处仅初始化打开此功能，先不做特殊处理，设为NULL

   ```c
   static void __cm_pm_init(void)
   {
   	int i=0;

   	for(i = 0;i < CM_PM_MAX_NUM;i++)
   	{
   		test_pm_cfg[i].cb_enter = NULL;
   		test_pm_cfg[i].cb_exit = NULL;
   	}
   }
   ```
3. Keypad初始化部分，首先需要对所使用的矩阵按键进行配置；将矩阵键盘的横纵排列都设为2，将所有PIN脚IO模式配置好，注册按键回调；

   ```c
   #define  OPENCPU_KEYIN_PIN0     CM_IOMUX_PIN_81
   #define  OPENCPU_KEYOUT_PIN0    CM_IOMUX_PIN_29
   #define  OPENCPU_KEYIN_MUX0     CM_IOMUX_PIN_81, CM_IOMUX_FUNC_FUNCTION2
   #define  OPENCPU_KEYOUT_MUX0    CM_IOMUX_PIN_29, CM_IOMUX_FUNC_FUNCTION3

   #define  OPENCPU_KEYIN_PIN1     CM_IOMUX_PIN_28
   #define  OPENCPU_KEYOUT_PIN1    CM_IOMUX_PIN_49
   #define  OPENCPU_KEYIN_MUX1     CM_IOMUX_PIN_28, CM_IOMUX_FUNC_FUNCTION3
   #define  OPENCPU_KEYOUT_MUX1    CM_IOMUX_PIN_49, CM_IOMUX_FUNC_FUNCTION3
   ```

   ```c
   static void __cm_keypad_fuc_init(void)
   {
       cm_keypad_kp_mkin_e col[2] = {0,1};
       cm_keypad_kp_mkout_e row[2] = {0,1};
       cm_keypad_cfg_t cm_keypad_cfg = {0};

       cm_keypad_cfg.cm_col = col;
       cm_keypad_cfg.cm_row = row;
       cm_keypad_cfg.cm_col_len = 2;
       cm_keypad_cfg.cm_row_len = 2;

       cm_iomux_set_pin_func(OPENCPU_KEYIN_MUX0);
       cm_iomux_set_pin_cmd(OPENCPU_KEYIN_PIN0,CM_IOMUX_PINCMD0_SLEEP,CM_IOMUX_PINCMD0_FUNC1_SLEEP_DIR);

       cm_iomux_set_pin_func(OPENCPU_KEYOUT_MUX0);
       cm_iomux_set_pin_cmd(OPENCPU_KEYOUT_PIN0,CM_IOMUX_PINCMD0_SLEEP,CM_IOMUX_PINCMD0_FUNC1_SLEEP_DIR);

       cm_iomux_set_pin_func(OPENCPU_KEYIN_MUX1);
       cm_iomux_set_pin_cmd(OPENCPU_KEYIN_PIN1,CM_IOMUX_PINCMD0_SLEEP,CM_IOMUX_PINCMD0_FUNC1_SLEEP_DIR);

       cm_iomux_set_pin_func(OPENCPU_KEYOUT_MUX1);
       cm_iomux_set_pin_cmd(OPENCPU_KEYOUT_PIN1,CM_IOMUX_PINCMD0_SLEEP,CM_IOMUX_PINCMD0_FUNC1_SLEEP_DIR);
   ```

   ```c
      cm_keypad_register(__cm_keypad_callback);
      cm_keypad_config(&cm_keypad_cfg);
      __cm_pm_cb_register(0,__cm_keypad_pm_enter,__cm_keypad_pm_exit);//需要在初始化cm_test_pm_init函数之后执行，否则会被清除
   ```
4. 需要注意的是，最后注册了PM回调，\_\_cm_pm_cb_register(0,\__cm_keypad_pm_enter,__cm_keypad_pm_exit);此时增加了进入休眠和退出休眠的回调函数，以实现矩阵键盘的休眠功能；
5. 当按键按下后，会进入__cm_keypad_callback回调。不建议在回调中做业务功能，所以将得到的键值传递到业务线程再进行处理；

   ```c
   static void __cm_keypad_callback(cm_keypad_map_t key, cm_keypad_event_e event)
   {
       cm_log_printf(0, "keypad key:%x\n", key);
       cm_log_printf(0, "keypad event:%d\n", event);
       if(cm_keypad_sleep_flag == 1)
       {
           cm_keypad_sleep(0);//按键回调函数执行时，按键才退出休眠
           cm_keypad_sleep_flag = 0;
       }
       if(event == CM_KEY_EVENT_PRESS)
       {
           osMessageQueuePut(keypad_queue, &key, 0, 0);
       }
   }
   ```

   ```c
   static void __cm_keypad_dell(int key)
   {
       switch (key)
       {
       case KEY_00:
           __cm_uart_printf("00\n");
           cm_pm_work_unlock();
           break;
       case KEY_01:
           __cm_uart_printf("01\n");
           cm_pm_work_unlock();
           break;
       case KEY_10:
           __cm_uart_printf("10\n");
           cm_pm_work_lock();
           break;
       case KEY_11:
           __cm_uart_printf("11\n");
           cm_pm_work_lock();
           break;
       }
   }
   ```
6. 按下按键后，串口0将打印对应键值，且00、01两个按键会打开休眠模式，10、11两个按键会关闭自动进入休眠模式；
7. 用户可根据示例APP进行修改，以设备更多自己的功能。

## **APP使用说明**

- 支持的模组（子）型号：ML307R-DC/ML307C-DC-CN
- 支持的SDK版本：ML307R OpenCPU SDK 2.0.0/ML307C OpenCPU SDK 1.0.0版本及其后续版本
- 是否需要外设支撑：

  - 需要将KEY0(PIN81 PIN29)，KEY1(PIN28 PIN49)引出到一个2x2的矩阵按键键盘
- 使用注意事项：

  - 本APP仅做开发参考，用户需结合自身项目需求进行优化；
  - 默认打开按键休眠功能，可通过__cm_pm_cb_unregister取消回退，或不执行__cm_pm_init；
  - 键值排列可能需根据ML307R OpenCPU资源进行调整与修改；
  - 默认打开按键休眠功能，可通过__cm_pm_cb_unregister取消回退，或不执行__cm_pm_init；
  - 串口0仅初始化了输出功能，无输入处理。

## **FAQ**

- 无

## **版本更新说明**

### **1.0.1版本**
- 发布时间：2024/12/24 10:26
- 修改记录：
  1. 新增支持的模组（子）型号以及支持的SDK版本

### **1.0.0版本**
- 发布时间：2024/10/24 10:12
- 修改记录：
  1. 初版

---
