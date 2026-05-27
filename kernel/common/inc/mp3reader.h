/*
 * Copyright (C) 2014 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef MP3READER_H_
#define MP3READER_H_
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include "audio_file.h"

extern "C" {
    bool resyncStream(
        char* buffer, char* buffer_end, uint32_t match_header,
        uint32_t *inout_pos, uint32_t *out_header);

    bool parseHeader(
        uint32_t header, size_t *frame_size,
        uint32_t *out_sampling_rate, uint32_t *out_channels,
        uint32_t *out_bitrate, uint32_t *out_num_samples);

    uint32_t U32_AT(const uint8_t *ptr);

    class Mp3Reader {
    public:
        Mp3Reader();
        bool init(const char *file, bool sync2frame = true);
        bool getFrame(void *buffer, uint32_t *size);
        bool getBuffer(uint32_t require_size, void *buffer, uint32_t *size);

        uint32_t getSampleRate() { return mSampleRate; }
        uint32_t getNumChannels() { return mNumChannels; }
        uint32_t getBitRate() { return mBitrate; }
        uint32_t getFixedHeader() { return mFixedHeader; }
        uint32_t getCurrentHeader() { return mCurrentHeader; }
        uint32_t getCurrentOffset() { return mCurrentPos; }

        void close();
        ~Mp3Reader();
    private:
        AUDIO_FILE_ID    mFp;

        uint32_t mFixedHeader;
        uint32_t mCurrentHeader;
        uint32_t mCurrentPos;
        uint32_t mLastPos;
        uint32_t mSampleRate;
        uint32_t mNumChannels;
        uint32_t mBitrate;
    };
}

#endif /* MP3READER_H_ */
