#ifndef _BIQUAD_IIR_H
#define _BIQUAD_IIR_H



typedef struct
{	short xL[2];		//xL[0] = x0,xL[1] = x1
	short xR[2];
	int yL[2];
	int yR[2];
} BiquadIIRMemStruct;

typedef struct
{
	BiquadIIRMemStruct EqFilterMemory[5];
}EqFilterMemT;


//ICAT EXPORTED STRUCT
typedef struct
{
	short a[3];    // a[0], a[1], a[2]
	short b[3];   // b[0], b[1], b[2]
} BiQuadCoeffsStruct;

//ICAT EXPORTED STRUCT
typedef struct
{
	unsigned short BiquadEnControl;		//bit 0 ~bit 4 is on/off of each filter.
	//unsigned short reserved0;
	//short     reserve1;
	BiQuadCoeffsStruct Tuningmem[5];	//get from user input or read workspace log
}EqParamIn;


void BiQuadIIRfilter(short *inputPtr_L, 
                            short *inputPtr_R, 
                            int step, 
                            BiquadIIRMemStruct *filterMemoryPtr, 
                            BiQuadCoeffsStruct *coeffsPtr, 
                            short frameSize);


void EQ_Func_48k(short *input_L, 
                 short *input_R, int step, EqParamIn*  EqPtr);
void EQ_Filter_Init(void);


#if 0  // need to move into audio code
EqParamIn  EqParameters ={
        // unsigned short BiquadEnControl; 
    //bit 0 ~bit 4 is on/off of each filter.
    0x1,
        //unsigned short reserved0;
        //short     reserve1;
    // BiQuadCoeffsStruct Tuningmem[5]; 
    //get from user input or read workspace log
    {   //for example a[0] =8192  norm_s(a[0])-1 = 2;
        {2, -7989, 3106, 4425, -3994, 1224},
        {1,  6367, 255 , 5590, 3184,  2729},
        {1,  6367, 255 , 5590, 3184,  2729},
        {1,  6367, 255 , 5590, 3184,  2729},
        {1,  6367, 255 , 5590, 3184,  2729},
    }
};
#endif

#endif
