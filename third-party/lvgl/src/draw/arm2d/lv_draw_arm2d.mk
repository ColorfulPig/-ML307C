OC_FILES += lv_gpu_arm2d.c

DEPPATH += --dep-path $(ETHIRDPARTY_LVGL_DIR)/lvgl/src/draw/arm2d
VPATH += :$(ETHIRDPARTY_LVGL_DIR)/lvgl/src/draw/arm2d

CFLAGS += "-I$(ETHIRDPARTY_LVGL_DIR)/lvgl/src/draw/arm2d"
