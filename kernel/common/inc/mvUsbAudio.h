#pragma once
#include <stdint.h>
#include "usb_descriptor.h"
#include "USBAudio_Types.h"
#include "USBDescriptor.h"
#include "USBDevice_Types.h"

#ifndef _uac_dynamic_config
#define _uac_dynamic_config (0)
#endif

#define _rx_channel_count   (1)
#define _tx_channel_count   (1)
#define _rx_freq            (16000)
#define _tx_freq            (8000)
#define _channel_config(a)      ((a) == 1 ? CHANNEL_M : (CHANNEL_L + CHANNEL_R))
#define _packet_size_max(a,b)   ((a) * (b) / 1000 * sizeof(short))
#define _channel_config_rx      (_channel_config(_rx_channel_count))
#define _channel_config_tx      (_channel_config(_tx_channel_count))
#define _rx_packet_size_max     (_packet_size_max(_rx_channel_count, _rx_freq))
#define _tx_packet_size_max     (_packet_size_max(_tx_channel_count, _tx_freq))

#define _episo_out          (AudioRxEP)
#define _episo_in           (AudioTxEP)

#define _audio_control_interface        (8)
#define _audio_streaming_interface_out  (_audio_control_interface + 1)
#define _audio_streaming_interface_in   (_audio_control_interface + 2)
#define _audio_streaming_zero_bandwidth (0)
#define _audio_streaming_operational    (1)

#define _has_feature_unit   (0)
#define _has_streaming_out  (1)
#define _has_streaming_in   (1)
#define _audio_interface_num        ((_has_streaming_out == 1 && _has_streaming_in == 1) ? (3) : (2))

typedef enum UAC_CONFIG_ITEM {
    UAC_RATE_RX,
    UAC_RATE_TX,
    UAC_CHAN_RX,
    UAC_CHAN_TX,

    NUM_UAC_ITEM
}UAC_CONFIG_ITEM;

void mvUsbAudioInitialize(void);
void mvUsbAudioSetMode(uint32_t mode);
void mvUsbAudioSetItem(UAC_CONFIG_ITEM item, uint32_t item_value);
uint32_t mvUsbAudioGetItem(UAC_CONFIG_ITEM item);
void mvUsbAudioStreamOutStart(void);
void mvUsbAudioStreamOutStop(void);

void mvUsbAudioStreamInStart(void);
void mvUsbAudioStreamInStop(void);
