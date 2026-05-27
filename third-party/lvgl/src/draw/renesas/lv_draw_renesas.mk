OC_FILES += lv_gpu_d2_ra6m3.c
OC_FILES += lv_gpu_d2_draw_label.c

DEPPATH += --dep-path $(ETHIRDPARTY_LVGL_DIR)/lvgl/src/draw/renesas
VPATH += :$(ETHIRDPARTY_LVGL_DIR)/lvgl/src/draw/renesas

CFLAGS += "-I$(ETHIRDPARTY_LVGL_DIR)/lvgl/src/draw/renesas"
