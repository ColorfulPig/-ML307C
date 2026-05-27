#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

    typedef enum media_play_event {
        media_play_event_tick,
        media_play_event_overrun,
        media_play_event_underrun,
        media_play_event_end,
        media_play_event_started,
    }media_play_event;

    void on_media_play_start(int32_t rate, int32_t ch, int32_t option, void(*trigger)(int));
    void on_media_play_update(const int16_t* buf, int32_t size);
    void on_media_play_stop(void);
    int on_media_play_suspend(void);
    int on_media_play_resume(void);
    void on_media_play_drain(int drain);
#ifdef __cplusplus
}
#endif
