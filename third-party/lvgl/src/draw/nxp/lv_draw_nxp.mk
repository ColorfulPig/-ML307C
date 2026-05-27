DEPPATH += --dep-path $(ETHIRDPARTY_LVGL_DIR)/lvgl/src/draw/nxp
VPATH += :$(ETHIRDPARTY_LVGL_DIR)/lvgl/src/draw/nxp

CFLAGS += "-I$(ETHIRDPARTY_LVGL_DIR)/lvgl/src/draw/nxp"

include $(ETHIRDPARTY_LVGL_DIR)/lvgl/src/draw/nxp/pxp/lv_draw_nxp_pxp.mk
include $(ETHIRDPARTY_LVGL_DIR)/lvgl/src/draw/nxp/vglite/lv_draw_nxp_vglite.mk
