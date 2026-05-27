OC_FILES += lv_draw_sdl.c
OC_FILES += lv_draw_sdl_arc.c
OC_FILES += lv_draw_sdl_bg.c
OC_FILES += lv_draw_sdl_composite.c
OC_FILES += lv_draw_sdl_img.c
OC_FILES += lv_draw_sdl_label.c
OC_FILES += lv_draw_sdl_line.c
OC_FILES += lv_draw_sdl_mask.c
OC_FILES += lv_draw_sdl_polygon.c
OC_FILES += lv_draw_sdl_rect.c
OC_FILES += lv_draw_sdl_stack_blur.c
OC_FILES += lv_draw_sdl_texture_cache.c
OC_FILES += lv_draw_sdl_utils.c
OC_FILES += lv_draw_sdl_layer.c

DEPPATH += --dep-path $(ETHIRDPARTY_LVGL_DIR)/lvgl/src/draw/sdl
VPATH += :$(ETHIRDPARTY_LVGL_DIR)/lvgl/src/draw/sdl

CFLAGS += "-I$(ETHIRDPARTY_LVGL_DIR)/lvgl/src/draw/sdl"
