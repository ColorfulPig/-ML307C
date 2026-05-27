#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

    typedef struct mp3_dec_config {
        const char* name;
        const char* out_name;
        int block_size;
        int skip_count;
    }mp3_dec_config;
    typedef uint32_t mp3_dec_handle;

    int mp3_decode_open(const mp3_dec_config* config, mp3_dec_handle* handle);
    int mp3_decode_read(mp3_dec_handle handle);
    int mp3_decode_read_raw(mp3_dec_handle handle, uint8_t**raw_data, uint32_t* size);
    int mp3_decode_loop(mp3_dec_handle handle);
    int mp3_decode_get_pcm_ptr(mp3_dec_handle handle, int16_t** output_data);
    int mp3_decode_get_pcm_size(mp3_dec_handle handle, uint32_t* size);
    int mp3_decode_get_pcm(mp3_dec_handle handle, int16_t* output_data, uint32_t* size);
    int mp3_decode_get_rate(mp3_dec_handle handle, uint32_t* rate);
    int mp3_decode_get_channel(mp3_dec_handle handle, uint32_t* channel);
    int mp3_decode_get_bitrate(mp3_dec_handle handle, uint32_t* bitrate);
    int mp3_decode_get_id3offset(mp3_dec_handle handle, uint32_t* id3offset);
    int mp3_decode_set_raw(mp3_dec_handle handle, const uint8_t* raw_data, uint32_t size);
    int mp3_decode_close(mp3_dec_handle handle);

#ifdef __cplusplus
}
#endif