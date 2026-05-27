# LCD APP 简介

## **屏幕和LVGL介绍**

ST7735S，是一款1.8英寸128*160分辨率的TFT全彩屏，采用SPI通信，本示例中采用RGB565 16bit的色块编译模式（RGB565即高五位为Red，低五位为Blue，中间六位为Green，共16位）。

LVGL(轻量级和通用图形库)是一个免费和开源的图形库，它提供了创建嵌入式GUI所需的一切，具有易于使用的图形元素，美丽的视觉效果和低内存占用。

LVGL是完全开源的，没有外部依赖，这使得它的移植非常简单。它可与任何现代MCU或MPU配合使用，并可与任何（RT）操作系统或裸机设置配合使用，以驱动ePaper、单色、OLED或TFT显示器。

更多LVGL相关内容及开发指导详见[LVGL](https://lvgl.io/)或[LVGL百问网](https://lvgl.100ask.net/8.4/)

## **APP实现功能**

实现LVGL示例展示功能，包括以下子功能：

1. 移植LVGL，将图像接口耦合，可使用LVGL库中的接口来实现UI；
2. 使用LVGL库中的接口，实现一个简单的LVGL应用。

## **APP执行流程**

1. 开机进入主线程，st7735s初始化，lvgl初始化，lvgl显示屏初始化，lvgl任务创建。循环执行lcd_lvgl_show()

   ```c
   /** app线程入口*/
   static void __task_loop(void *arg)
   {
       (void)arg;
       __lcd_st7735s_init();       // lcd初始化
       lv_init();                  // lvgl初始化，如果这个没有初始化，那么下面的初始化会崩溃
       lv_port_disp_init();        // lvgl显示屏初始化
       __lvgl_task_create();       // lvgl任务创建
       while (1)
       {
           lcd_lvgl_show();         // lvgl显示屏刷新
           osDelay(200);
       }
   }
   ```
2. 其中LCD初始化命令可根据屏幕厂商所提供的命令进行配置，需注意引脚的设置。
3. 一切准备就绪后，屏幕中将会出现两个循环变化的图表，可根据LVGL库接口自行修改。
4. 必须注意的是，LVGL需要定期执行操作系统任务，因此需要创建两个线程，一个用于执行lv_tick_inc同步时钟，一个用于执行lv_task_handler刷新屏幕。lv_tick_inc同步时钟可以用定时器实现，本示例中直接用线程delay调度实现。lv_task_handler所在线程优先级需高于lv_tick_inc所在线程，以保证屏幕及时刷新。

   ```c
   /**
    * @brief lvgl tick task
    */
   static void __lvgl_tick_task()
   {
   	while(1) 
       {
           /** lv_tick_inc时间需于系统时钟周期一致，否则会导致刷新屏幕不及时   */
   		lv_tick_inc(LVGL_TICK);
   		osDelay(LVGL_TICK / 5);
   	}
   }
   /**
    * @brief lvgl task
    */
   static void __lvgl_task()
   {
   	while(1) 
       {
   		lv_task_handler();
   		osDelay(LVGL_TICK / 5);
   	}
   }
   ```
5. 客户若需移植其他屏幕，可前往修改third-party\\lvgl\lv_conf.h。其中屏幕像素高度、宽度、色深、缓存区大小等，都可以自行修改。若需修改驱动耦合接口，可前往修改third-party\lvgl\examples\porting\lv_port_disp.c。 其中buff模式可修改，但是本模组空间有限，实际只能支持模式1和模式2。

   ```c
   #ifndef BUFFER_MODE
       #define BUFFER_MODE 1 /*此处默认选择1模式*/
   #endif

   #if BUFFER_MODE == 1
       /* Example for 1) */
       static lv_disp_draw_buf_t draw_buf_dsc_1;
       static lv_color_t buf_1[MY_DISP_HOR_RES * 10];                          /*A buffer for 10 rows*/
       lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, NULL, MY_DISP_HOR_RES * 10);   /*Initialize the display buffer*/
   #elif BUFFER_MODE == 2
       /* Example for 2) */
       static lv_disp_draw_buf_t draw_buf_dsc_2;
       static lv_color_t buf_2_1[MY_DISP_HOR_RES * 10];                        /*A buffer for 10 rows*/
       static lv_color_t buf_2_2[MY_DISP_HOR_RES * 10];                        /*An other buffer for 10 rows*/
       lv_disp_draw_buf_init(&draw_buf_dsc_2, buf_2_1, buf_2_2, MY_DISP_HOR_RES * 10);   /*Initialize the display buffer*/
   #else
       /* Example for 3) also set disp_drv.full_refresh = 1 below*/
       static lv_disp_draw_buf_t draw_buf_dsc_3;
       static lv_color_t buf_3_1[MY_DISP_HOR_RES * MY_DISP_VER_RES];            /*A screen sized buffer*/
       static lv_color_t buf_3_2[MY_DISP_HOR_RES * MY_DISP_VER_RES];            /*Another screen sized buffer*/
       lv_disp_draw_buf_init(&draw_buf_dsc_3, buf_3_1, buf_3_2,
                             MY_DISP_VER_RES * LV_VER_RES_MAX);   /*Initialize the display buffer*/
   #endif
   ```

   LVGL默认画点的方式相对速度较慢，本示例中采用缓存DMA方式，刷新速度略好与画点。用户若对刷新频率有更高要求，亦可以自行优化。

   ```c
   static uint16_t s_lcdbuffer[MY_DISP_HOR_RES * 10] = {0};
   static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
   {
       if(disp_flush_enabled) {
           /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/

           memset(s_lcdbuffer, 0, sizeof(s_lcdbuffer));
           int width = area->x2 - area->x1 + 1;
           int height = area->y2 - area->y1 + 1;

   #if 0   //自定义填充方式
           int32_t x;
           int32_t y;
           for(y = 0; y <= height; y++) {
               for(x = 0; x < width; x++) {
                   /* Put a pixel to the display. For example: */
                   /* put_px(x, y, *color_p)*/
                   *(s_lcdbuffer + x + y * width) = color_p->full;
                   color_p++;
               }
           }
   #else
           memcpy(s_lcdbuffer, disp_drv->draw_buf->buf1, height * width * 2);
   #endif
           lcd_write_address(area->x1, area->y1, area->x2, area->y2);
           cm_lcd_write_buf((unsigned char*)s_lcdbuffer, height * width * 2);

       }

       /*IMPORTANT!!!
        *Inform the graphics library that you are ready with the flushing*/
       lv_disp_flush_ready(disp_drv);
   }
   ```

## **APP使用说明**

- 支持的模组（子）型号：ML307R-DC/ML307C-DC-CN
- 支持的SDK版本：ML307R OpenCPU SDK 2.0.0/ML307C OpenCPU SDK 1.0.0版本及其后续版本
- 是否需要外设支撑：需要ST7735S LCD屏幕
- 使用注意事项：
  - 本LVGL库版本为8.4.0，如需修改，请参考对应开发接口文档
  - 若需更换其他型号屏幕，建议参考相关文档。前文中已对lvgl移植修改做了一定说明，在无了解的情况下，请勿修改其他相关依赖文件。

## **FAQ（非必要，视客户/FAE咨询情况增列）**

- 无

## **版本更新说明**

### **1.0.1版本**
- 发布时间：2024/12/24 10:26
- 修改记录：
  1. 新增支持的模组（子）型号以及支持的SDK版本

### **1.0.0版本**
- 发布时间：2024/10/23 19:30
- 修改记录：
  1. 初版

---
