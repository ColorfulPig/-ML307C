/******************************************************************************
 *
 *  (C)Copyright ASRMicro. All Rights Reserved.
 *
 *  THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF ASRMicro.
 *  The copyright notice above does not evidence any actual or intended
 *  publication of such source code.
 *  This Module contains Proprietary Information of ASRMicro and should be
 *  treated as Confidential.
 *  The information in this file is provided for the exclusive use of the
 *  licensees of ASRMicro.
 *  Such users have the right to use, modify, and incorporate this code into
 *  products for purposes authorized by the license agreement provided they
 *  include this notice and the associated copyright notice with any such
 *  product.
 *  The information in this file is provided "AS IS" without warranty.
 *
 ******************************************************************************/

#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stdbool.h>

    typedef struct audio_file_ops {
        uint32_t (*open) (const char* filename, const char* mode);
        int (*close) (uint32_t stream);
        int (*seek) (uint32_t stream, long offset, int wherefrom);
        size_t (*read) (void* buffer_ptr, size_t element_size, size_t count, uint32_t stream);
        size_t (*write) (const void* buffer_ptr, size_t element_size, size_t count, uint32_t stream);
        bool (*access) (const char* filename);
    }audio_file_ops_t;

    typedef uint32_t AUDIO_FILE_ID;
    AUDIO_FILE_ID common_fopen(const char* filename, const char* mode);
    int common_fclose(AUDIO_FILE_ID stream);
    int common_fseek(AUDIO_FILE_ID stream, long offset, int wherefrom);
    size_t common_fread(void* buffer_ptr, size_t element_size, size_t count, AUDIO_FILE_ID stream);
    size_t common_fwrite(const void* buffer_ptr, size_t element_size, size_t count, AUDIO_FILE_ID stream);
    bool common_access(const char* filename);

    void register_audio_fops(audio_file_ops_t* ops);
#ifdef __cplusplus
}
#endif