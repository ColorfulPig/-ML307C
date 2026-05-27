
#ifndef _AUDIO_PORT_API_H
#define _AUDIO_PORT_API_H

#ifdef __cplusplus
extern "C"{
#endif



typedef void  (*AUDIO_PORT_CB)(char *pData, unsigned int len8);
typedef void (*SSP_SpeakerPA_CB) (UINT32 on);
typedef void (*SSP_Codec_CB) (UINT32 on);

void SSP_BindCodec_CB(SSP_Codec_CB cb);
void SSP_BindSpeakerPA_CB(SSP_SpeakerPA_CB cb);

int AudioPort_Register_CB(AUDIO_PORT_CB playback_cb, AUDIO_PORT_CB capture_cb);
int AudioPort_Start(void);
int AudioPort_Stop(void);


/**********************************************************************
 *
 * Function: AudioPort_SetBCLKType
 *
 * Description: set PCM interface SSP's bitclk frequency
 *
 * type:  
    0: 32fs; 
    1: 64fs; 
    2: 128fs; 
    3: 256fs; 


 *  return value: 
    0: success
    -1:fail 
 *  NOTE:  
 *
 * --------------------------------------------------------------------
 * Revision History
 *
 * Date         Who        Version           Description
 * --------------------------------------------------------------------
 * 11-April-2023	Chaoliu                      Initial version
 **********************************************************************/
int AudioPort_SetBCLKType(unsigned int type) ;

/**********************************************************************
 *
 * Function: AudioPort_SetSampleRate
 *
 * Description: set PCM interface SSP's sample Rate
 *
 * rate:  
    1: 8k; 
    2: 16k 
    4: 32k 
    6: 48k
    100: 800k  for PWM mode
    120: 960k  for PWM mode

 *  return value: 
    0: success
    -1:fail 
 *  NOTE:  
 *
 * --------------------------------------------------------------------
 * Revision History
 *
 * Date         Who        Version           Description
 * --------------------------------------------------------------------
 * 11-April-2023	Chaoliu                      Initial version
 **********************************************************************/
int AudioPort_SetSampleRate(unsigned int rate) ;


/**********************************************************************
 *
 * Function: AudioPort_SetChannels
 *
 * Description: set PCM as 1 channel or 2 channel
 *
 * rate:  
    1: 1 channel; 
    2: 2 channel; 

 *  return value: 
    0: success
    -1:fail 
 *  NOTE:  
 *
 * --------------------------------------------------------------------
 * Revision History
 *
 * Date         Who        Version           Description
 * --------------------------------------------------------------------
 * 11-April-2023	Chaoliu                      Initial version
 **********************************************************************/
int AudioPort_SetChannels(unsigned int channels) ;
int AudioPort_Select(unsigned int index) ;
/**********************************************************************
 *
 * Function: AudioPort_SetI2S
 *
 * Description: set SSP 's PCM/I2S format
 *
 * i2s:  
    0: PCM; 
    1: I2S; 
 

 *  return value: 
    0: success
    -1:fail 
 *  NOTE:  
 *
 * --------------------------------------------------------------------
 * Revision History
 *
 * Date         Who        Version           Description
 * --------------------------------------------------------------------
 * 7-April-2023	Chaoliu                      Initial version
 **********************************************************************/
int AudioPort_SetI2S(unsigned int i2s) ;
/**********************************************************************
 *
 * Function: AudioPort_SetBCLKInvert
 *
 * Description: set SSP 's bitclk INVERT
 *
 * invert:  
    0: keep default; 
    1: invert; 


 *  return value: 
    0: success
    -1:fail 
 *  NOTE:  
 *
 * --------------------------------------------------------------------
 * Revision History
 *
 * Date         Who        Version           Description
 * --------------------------------------------------------------------
 * 7-April-2023	Chaoliu                      Initial version
 **********************************************************************/
int AudioPort_SetBCLKInvert(unsigned int invert) ;
/**********************************************************************
 *
 * Function: AudioPort_SetFrameClkWidth
 *
 * Description: set SSP's frame clock  width
 *
 * width: 1~0x3f 
    1: frame clock  width is  1 bitclk;  
    2: frame clock  width is  2 bitclk; 
    ..........................
    0x10: frame clock  width is  16 bitclk; 
    ..........................
    0x3F: frame clock  width is  63 bitclk; 


 *  return value: 
    0: success
    -1:fail 
 *  NOTE:  
 *
 * --------------------------------------------------------------------
 * Revision History
 *
 * Date         Who        Version           Description
 * --------------------------------------------------------------------
 * 7-April-2023	Chaoliu                      Initial version
 **********************************************************************/
int AudioPort_SetFrameClkWidth(unsigned int width) ;
/**********************************************************************
 *
 * Function: AudioPort_SetFrameClkInvert
 *
 * Description: set SSP's FrameClk INVERT
 *
 * invert:  
    0: keep default; 
    1: invert; 


 *  return value: 
    0: success
    -1:fail 
 *  NOTE:  
 *
 * --------------------------------------------------------------------
 * Revision History
 *
 * Date         Who        Version           Description
 * --------------------------------------------------------------------
 * 7-April-2023	Chaoliu                      Initial version
 **********************************************************************/
int AudioPort_SetFrameClkInvert(unsigned int invert) ;
/**********************************************************************
 *
 * Function: AudioPort_SetDelay
 *
 * Description: set SSP's data delay
 *
 * delay:  
    0: no delay;  
    1: 1 bitclk delay; (default) 


 *  return value: 
    0: success
    -1:fail 
 *  NOTE:  
 *
 * --------------------------------------------------------------------
 * Revision History
 *
 * Date         Who        Version           Description
 * --------------------------------------------------------------------
 * 7-April-2023	Chaoliu                      Initial version
 **********************************************************************/
int AudioPort_SetDelay(unsigned int delay) ;

#ifdef __cplusplus
}
#endif

#endif //_AUDIO_PORT_API_H