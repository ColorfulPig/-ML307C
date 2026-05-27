#ifndef _ECALL_UTILS_H
#define _ECALL_UTILS_H 1
#include "cfw_typedef.h"      // Common FW generic types

#if FEATURE_AUDIO_ECALL_IVS
#include "basic_op_audio.h"
#else
#define MAX_32  ((Word32)0x7fffffffL)
#define MIN_32  ((Word32)0x80000000L)
#define MIN_16  (-32768)
#define MAX_16  (32767)
Word16 add(Word16 var1, Word16 var2);    /* Short add,           1   */
Word16 sub(Word16 var1, Word16 var2);    /* Short sub,           1   */
Word16 abs_s(Word16 var1);               /* Short abs,           1   */
Word16 shl(Word16 var1, Word16 var2);    /* Short shift left,    1   */
Word16 shr(Word16 var1, Word16 var2);    /* Short shift right,   1   */
Word32 L_mult(Word16 var1, Word16 var2); /* Long mult,           1   */
Word16 negate(Word16 var1);              /* Short negate,        1   */

Word16 round_fx(Word32 L_var1);          /* Round,               1   */
Word32 L_mac(Word32 L_var3, Word16 var1, Word16 var2);   /* Mac,  1  */

Word32 L_add(Word32 L_var1, Word32 L_var2);    /* Long add,        1 */
Word32 L_sub(Word32 L_var1, Word32 L_var2);    /* Long sub,        1 */

Word32 L_shl(Word32 L_var1, Word16 var2);      /* Long shift left, 1 */
Word32 L_shr(Word32 L_var1, Word16 var2);      /* Long shift right, 1 */


Word32 L_abs(Word32 L_var1);            /* Long abs,              1  */
Word16 L_sat(Word32 L_var1);            /* Long saturation,       2  */
Word16 norm_s(Word16 var1);             /* Short norm,            1  */
#endif

Word16 div_s(Word16 var1, Word16 var2); /* Short division,       18  */
Word32 MultLL(Word32 Lv1, Word32 lv2);
const void* circptr(const void * i, int m, const void * b, int l);

#define Set_Zero_32(src, lenInshort)    memset(src,0,lenInshort*2)
#define Copy(s, d, len)                 memcpy(d,s, len*2)

#define create_fr2x32(a, b)             (((UINT32)a<<16)|(UINT32)(b)&0xFFFF)
#define negate_fr1x16(a)                sub(0,a)
#define sub_fr1x32(a,b)                 L_sub(a,b)
#define extract_l_fr1x32(a)             ((short)(a&0xFFFF))
#define shr_fr1x32(a, b)                L_shr(a,b)
#define mmult_fr1x32(a, b)              (((int)(b&0xFFFF))*a)
#define MASK32(x)                       ((UINT32)1<<x)
#define max_fr1x16(a,b)                 MAX(a,b)
#define normalize_fr1x16                norm_s
#define max_fr1x32(a,b)                 MAX(a,b)
#define min_fr1x32(a,b)                 MIN(a,b)
#define negate_fr1x32(a)                (-(a))

#define shl_fr1x16                      shl
#define abs_fr1x16                      abs_s
#define shr_fr1x16                      shr
#define add_fr1x16                      add
#define sub_fr1x16                      sub
#define div_32_16_primitive(a,b)        ((short)(a/b)) //? todo   check  b == 0!!!!
#endif // _ECALL_UTILS_H
