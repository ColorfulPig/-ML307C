OC_FILES += lv_draw_sw.c
OC_FILES += lv_draw_sw_arc.c
OC_FILES += lv_draw_sw_blend.c
OC_FILES += lv_draw_sw_dither.c
OC_FILES += lv_draw_sw_gradient.c
OC_FILES += lv_draw_sw_img.c
OC_FILES += lv_draw_sw_letter.c
OC_FILES += lv_draw_sw_line.c
OC_FILES += lv_draw_sw_polygon.c
OC_FILES += lv_draw_sw_rect.c
OC_FILES += lv_draw_sw_transform.c
OC_FILES += lv_draw_sw_layer.c

DEPPATH += --dep-path $(ETHIRDPARTY_LVGL_DIR)/lvgl/src/draw/sw
VPATH += :$(ETHIRDPARTY_LVGL_DIR)/lvgl/src/draw/sw

CFLAGS += "-I$(ETHIRDPARTY_LVGL_DIR)/lvgl/src/draw/sw"
