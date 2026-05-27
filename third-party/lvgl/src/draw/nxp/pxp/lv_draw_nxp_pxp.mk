OC_FILES += lv_draw_pxp.c
OC_FILES += lv_draw_pxp_blend.c
OC_FILES += lv_gpu_nxp_pxp_osa.c
OC_FILES += lv_gpu_nxp_pxp.c

DEPPATH += --dep-path $(ETHIRDPARTY_LVGL_DIR)/lvgl/src/draw/nxp/pxp
VPATH += :$(ETHIRDPARTY_LVGL_DIR)/lvgl/src/draw/nxp/pxp

CFLAGS += "-I$(ETHIRDPARTY_LVGL_DIR)/lvgl/src/draw/nxp/pxp"
