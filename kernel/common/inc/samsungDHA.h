#ifndef _SPEECH_ENGINE_H_
#define _SPEECH_ENGINE_H_

/******************************************************************************
 *  TYPEDEFS AND STRUCTURES
 *****************************************************************************/
typedef struct{
short monitor1;
short monitor2;
short monitor3;
short monitor4;
short monitor5;
short monitor6;
short monitor7;
short monitor8;
short monitor9;
short monitor10;
short monitor11;
short monitor12;
short monitor13;
} DHAState_type;

/******************************************************************************
 *  FUNCTION PROTOTYPES
 *****************************************************************************/
extern int SamsungDHAState(void);

#endif	/* _SPEECH_ENGINE_H_ */
