
#ifndef _AUDIO_SSP_DMA_H
#define _AUDIO_SSP_DMA_H

#ifdef __cplusplus
extern "C"{
#endif


#define MAX_DMA_TIME_LEN_MS         (20)
#define MAX_SAMPLE_RATE             (48000)
//#define MAX_SAMPLE_RATE             (48000 * 2)
//#define PWM_MODE_SAMPLE_RATE        (960000)
#define PWM_MODE_SAMPLE_RATE        (800000)

//    duration_time *  sampleRate * 2 channel
#define DMA_BUFFER_UINT16_SIZE   (MAX_DMA_TIME_LEN_MS * MAX_SAMPLE_RATE / 1000 * 2)



extern __align(16) UINT32 dma_desc_tx_nocache[8];
extern __align(16) UINT32 dma_desc_rx_nocache[8];

// 2(tx and rx) *   DMA_BUFFER_UINT16_SIZE * 2 (ping-pong  )
extern __align(16)  UINT16  dma_buf_nocache[ 2 * DMA_BUFFER_UINT16_SIZE * 2];



#ifdef __cplusplus
}
#endif

#endif //_AUDIO_SSP_DMA_H
