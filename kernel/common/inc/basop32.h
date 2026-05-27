/*
  ===========================================================================
   File: BASOP32.H                                       v.2.3 - 30.Nov.2009
  ===========================================================================

            ITU-T STL  BASIC OPERATORS

            GLOBAL FUNCTION PROTOTYPES

   History:
   26.Jan.00   v1.0     Incorporated to the STL from updated G.723.1/G.729 
                        basic operator library (based on basic_op.h) and 
                        G.723.1's basop.h.
   05.Jul.00   v1.1     Added 32-bit shiftless mult/mac/msub operators

   03 Nov 04   v2.0     Incorporation of new 32-bit / 40-bit / control
                        operators for the ITU-T Standard Tool Library as 
                        described in Geneva, 20-30 January 2004 WP 3/16 Q10/16
                        TD 11 document and subsequent discussions on the
                        wp3audio@yahoogroups.com email reflector.
                        norm_s()      weight reduced from 15 to 1.
                        norm_l()      weight reduced from 30 to 1.
                        L_abs()       weight reduced from  2 to 1.
                        L_add()       weight reduced from  2 to 1.
                        L_negate()    weight reduced from  2 to 1.
                        L_shl()       weight reduced from  2 to 1.
                        L_shr()       weight reduced from  2 to 1.
                        L_sub()       weight reduced from  2 to 1.
                        mac_r()       weight reduced from  2 to 1.
                        msu_r()       weight reduced from  2 to 1.
                        mult_r()      weight reduced from  2 to 1.
                        L_deposit_h() weight reduced from  2 to 1.
                        L_deposit_l() weight reduced from  2 to 1.
                        L_mls() weight of 5.
                        div_l() weight of 32.
                        i_mult() weight of 3.

   30 Nov 09   v2.3     round() function is now round_fx().
                        saturate() is not referencable from outside application

   13 Mar 12            Add Overflow2 flag for additional overflow checking.
  ============================================================================
*/


#ifndef _BASIC_OP_H
#define _BASIC_OP_H

/* #define BASOP_OVERFLOW2 */

/*___________________________________________________________________________
 |                                                                           |
 |   Constants and Globals                                                   |
 |___________________________________________________________________________|
*/


#define BASOP_SATURATE_WARNING_ON
#define BASOP_SATURATE_WARNING_OFF
#define BASOP_SATURATE_ERROR_ON
#define BASOP_SATURATE_ERROR_OFF
#define BASOP_CHECK()


#define MAX_40 (0x0000007fffffffff)
#define MIN_40 (0xffffff8000000000)

#define MAX_32 ((Word32)0x7fffffffL)
#define MIN_32  ((Word32)0x80000000L)

#define MAX_16 (Word16)0x7fff
#define MIN_16 (Word16)0x8000

typedef signed char Word8;
typedef short Word16;
typedef short INT16;
typedef long Word32;
typedef long INT32;
typedef __int64 Word40;
typedef unsigned short UWord16;
typedef unsigned short UINT16;
typedef unsigned long UWord32;
typedef unsigned long UINT32;
typedef  int bool;
#ifdef  	TRUE
#undef  	TRUE
#endif	
#define 	TRUE	1

#ifdef  	FALSE
#undef  	FALSE
#endif	
#define 	FALSE	0

/*___________________________________________________________________________
 |                                                                           |
 |   Prototypes for basic arithmetic operators                               |
 |___________________________________________________________________________|
*/
Word16 saturate (Word32 L_var1);
Word16 add (Word16 var1, Word16 var2);    /* Short add,           1   */
Word16 sub (Word16 var1, Word16 var2);    /* Short sub,           1   */
Word16 abs_s (Word16 var1);               /* Short abs,           1   */
Word16 shl (Word16 var1, Word16 var2);    /* Short shift left,    1   */
Word16 shr (Word16 var1, Word16 var2);    /* Short shift right,   1   */
Word16 mult (Word16 var1, Word16 var2);   /* Short mult,          1   */
Word32 L_mult (Word16 var1, Word16 var2); /* Long mult,           1   */
Word16 negate (Word16 var1);              /* Short negate,        1   */
Word16 extract_h (Word32 L_var1);         /* Extract high,        1   */
Word16 extract_l (Word32 L_var1);         /* Extract low,         1   */
Word16 round_fx (Word32 L_var1);          /* Round,               1   */
Word32 L_mac (Word32 L_var3, Word16 var1, Word16 var2);   /* Mac,  1  */
Word32 L_msu (Word32 L_var3, Word16 var1, Word16 var2);   /* Msu,  1  */
Word32 L_macNs (Word32 L_var3, Word16 var1, Word16 var2); /* Mac without
                                                             sat, 1   */
Word32 L_msuNs (Word32 L_var3, Word16 var1, Word16 var2); /* Msu without
                                                             sat, 1   */
Word32 L_add (Word32 L_var1, Word32 L_var2);    /* Long add,        1 */
Word32 L_sub (Word32 L_var1, Word32 L_var2);    /* Long sub,        1 */
Word32 L_add_c (Word32 L_var1, Word32 L_var2);  /* Long add with c, 2 */
Word32 L_sub_c (Word32 L_var1, Word32 L_var2);  /* Long sub with c, 2 */
Word32 L_negate (Word32 L_var1);                /* Long negate,     1 */
Word16 mult_r (Word16 var1, Word16 var2);       /* Mult with round, 1 */
Word32 L_shl (Word32 L_var1, Word16 var2);      /* Long shift left, 1 */
Word32 L_shr (Word32 L_var1, Word16 var2);      /* Long shift right, 1 */
Word16 shr_r (Word16 var1, Word16 var2);        /* Shift right with
                                                   round, 2           */
Word16 mac_r (Word32 L_var3, Word16 var1, Word16 var2); /* Mac with
                                                           rounding, 1 */
Word16 msu_r (Word32 L_var3, Word16 var1, Word16 var2); /* Msu with
                                                           rounding, 1 */
Word32 L_deposit_h (Word16 var1);        /* 16 bit var1 -> MSB,     1 */
Word32 L_deposit_l (Word16 var1);        /* 16 bit var1 -> LSB,     1 */

Word32 L_shr_r (Word32 L_var1, Word16 var2); /* Long shift right with
                                                round,             3  */
Word32 L_abs (Word32 L_var1);            /* Long abs,              1  */
Word16 L_sat (Word32 L_var1);            /* Long saturation,       2  */
Word16 norm_s (Word16 var1);             /* Short norm,            1  */
Word16 div_s (Word16 var1, Word16 var2); /* Short division,       18  */
Word16 norm_l (Word32 L_var1);           /* Long norm,             1  */

#define  MAX_SONIC(a, b)    		((a > b) ? (a):(b))
//#define  MIN(a, b)     		((a <b) ? (a):(b))
#define MASK32(bit)     	( (UWord32)(1<<bit))
#define MASK16(bit)     	( (UWord16)(1<<bit))


/*
 * Additional G.723.1 operators
*/
Word32 L_mls( Word32, Word16 ) ;    /* Weight FFS; currently assigned 5 */
Word32 L_mll (Word32 Lv1, Word32 Lv2);
Word16 div_l( Word32, Word16 ) ;    /* Weight FFS; currently assigned 32 */
Word16 i_mult(Word16 a, Word16 b);  /* Weight FFS; currently assigned 3 */

/*
 *  New shiftless operators, not used in G.729/G.723.1
*/
Word32 L_mult0(Word16 v1, Word16 v2); /* 32-bit Multiply w/o shift         1 */
Word32 L_mac0(Word32 L_v3, Word16 v1, Word16 v2); /* 32-bit Mac w/o shift  1 */
Word32 L_msu0(Word32 L_v3, Word16 v1, Word16 v2); /* 32-bit Msu w/o shift  1 */

/*
* 40bit operation for DSP with 40bit ACC
*/
Word32 L_sat40(Word40 v1); /*saturation Word40  to Word32*/
Word40 Sat40(Word40 L_var1);
Word16 norm_L40 (Word40 L_var1);
Word40 L_shr40(Word40 L_var1, Word16 shift); 
Word40 L_shl40(Word40 L_var1, Word16 shift); 

Word40 L_mac40(Word40 Acc, Word16 a, Word16 b); // suppose guard bit enough for all filter
Word40 L_msu40(Word40 Acc, Word16 a, Word16 b);
Word40 L_mmult( Word16 op1, UWord16 op2) ;
Word40 L_mmac40(Word40 Acc, Word16 op1, UWord16 op2); // suppose guard bit enough for all filter
Word40 L_mmsu40(Word40 Acc, Word16 op1, UWord16 op2); // suppose guard bit enough for all filter


#endif /* ifndef _BASIC_OP_H */


/* end of file */
