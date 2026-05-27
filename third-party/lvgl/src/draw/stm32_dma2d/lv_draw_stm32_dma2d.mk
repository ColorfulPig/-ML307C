OC_FILES += lv_gpu_stm32_dma2d.c

DEPPATH += --dep-path $(ETHIRDPARTY_LVGL_DIR)/lvgl/src/draw/stm32_dma2d
VPATH += :$(ETHIRDPARTY_LVGL_DIR)/lvgl/src/draw/stm32_dma2d

CFLAGS += "-I$(ETHIRDPARTY_LVGL_DIR)/lvgl/src/draw/stm32_dma2d"
