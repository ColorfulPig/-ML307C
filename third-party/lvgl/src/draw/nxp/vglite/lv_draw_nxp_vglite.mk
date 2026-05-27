OC_FILES += lv_draw_vglite.c
OC_FILES += lv_draw_vglite_arc.c
OC_FILES += lv_draw_vglite_blend.c
OC_FILES += lv_draw_vglite_line.c
OC_FILES += lv_draw_vglite_rect.c
OC_FILES += lv_vglite_buf.c
OC_FILES += lv_vglite_utils.c

DEPPATH += --dep-path $(ETHIRDPARTY_LVGL_DIR)/lvgl/src/draw/nxp/vglite
VPATH += :$(ETHIRDPARTY_LVGL_DIR)/lvgl/src/draw/nxp/vglite

CFLAGS += "-I$(ETHIRDPARTY_LVGL_DIR)/lvgl/src/draw/nxp/vglite"
