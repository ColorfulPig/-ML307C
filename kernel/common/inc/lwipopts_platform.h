/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
#ifndef __LWIP_OPTS_PLAT_H__
#define __LWIP_OPTS_PLAT_H__

/*****************************************
*lwip macro define for different platform
*default settings is according to LWIP_PLATFORM_DEFINE
******************************************/
#define LWIP_PLATFORM_DEFINE_1802           (0x0001)
#define LWIP_PLATFORM_DEFINE_1802S          (0x0002)
#define LWIP_PLATFORM_DEFINE_CRANE          (0x0004)

#define LWIP_PLATFORM_DEFINE_MINIS          (0x0010)  /*based on crane*/
#define LWIP_PLATFORM_DEFINE_NBIOT   	    (0x0020)  /*based on crane*/
#define LWIP_PLATFORM_DEFINE_ALIOS   	    (0x0040)  /*based on crane*/
#define LWIP_PLATFORM_DEFINE_C4M     	    (0x0080)  /*based on crane*/
#define LWIP_PLATFORM_DEFINE_FP_LITE     	(0x0100)  /*based on crane*/
#define LWIP_PLATFORM_DEFINE_DM     	    (0x0200)  /*based on crane*/
#define LWIP_PLATFORM_DEFINE_C2M     	    (0x0400)  /*based on crane*/
#define LWIP_PLATFORM_DEFINE_SOCKET   	    (0x0800)  /*based on crane*/
#define LWIP_PLATFORM_DEFINE_CRANEW   	    (0x1000)  /*based on crane*/
#define LWIP_PLATFORM_DEFINE_KESTREL   	    (0x2000)  /*based on crane*/
#define LWIP_PLATFORM_DEFINE_2CHIP   	    (0x4000)  /*based on 1826 and kestrel*/

/*set all option config to default value 0*/
#define LWIP_DEFVAL_OPT_PLAT              	(0x0000)
#define LWIP_DEFVAL_OPT_ALIOS             	(0x0000)
#define LWIP_DEFVAL_OPT_MINIS            	(0x0000)
#define LWIP_DEFVAL_OPT_NBIOT            	(0x0000)
#define LWIP_DEFVAL_OPT_C4M            		(0x0000)
#define LWIP_DEFVAL_OPT_FP_LITE            	(0x0000)
#define LWIP_DEFVAL_OPT_DM            		(0x0000)
#define LWIP_DEFVAL_OPT_C2M            		(0x0000)
#define LWIP_DEFVAL_OPT_SOCKET         		(0x0000)
#define LWIP_DEFVAL_OPT_CRANEW         		(0x0000)
#define LWIP_DEFVAL_OPT_KESTREL       		(0x0000)
#define LWIP_DEFVAL_OPT_2CHIP           	(0x0000)

/*fix platform defined here*/
#ifdef LAPWING
#undef LWIP_DEFVAL_OPT_PLAT
#define LWIP_DEFVAL_OPT_PLAT              	LWIP_PLATFORM_DEFINE_CRANE
#undef LWIP_DEFVAL_OPT_KESTREL
#define LWIP_DEFVAL_OPT_KESTREL             LWIP_PLATFORM_DEFINE_KESTREL
#define LWIP_IMS_SUPPORT                    (1)
#else

/*****************************/
/*set platform type by platform macro check*/
/*****************************/
/*default set platform to crane*/
#undef LWIP_DEFVAL_OPT_PLAT
#define LWIP_DEFVAL_OPT_PLAT              	LWIP_PLATFORM_DEFINE_CRANE

/*PLAT_NEZHAC macro is defined from tavor/build/arbel.mak*/
#ifdef PLAT_NEZHAC
#undef LWIP_DEFVAL_OPT_PLAT
#define LWIP_DEFVAL_OPT_PLAT                LWIP_PLATFORM_DEFINE_1802S
#endif

/*****************************/
/*set platform option config by platform macro check*/
/*****************************/
/*compiler for platform option: sub1, use system defined macro */
#ifdef LWIP_PLAT_NORMAL

#ifdef PLAT_USE_ALIOS /*this macro is defined from tavor/build/arbel.mak*/
#undef LWIP_DEFVAL_OPT_ALIOS
#define LWIP_DEFVAL_OPT_ALIOS             	LWIP_PLATFORM_DEFINE_ALIOS
#endif

#ifdef CRANE_MCU_DONGLE /*this macro is defined from tavor/build/arbel.mak*/
#undef LWIP_DEFVAL_OPT_DM
#define LWIP_DEFVAL_OPT_DM            		LWIP_PLATFORM_DEFINE_DM
#endif

#ifdef LWIP_NBIOT /*this macro is defined from tavor/build/arbel.mak*/
#undef LWIP_DEFVAL_OPT_NBIOT
#define LWIP_DEFVAL_OPT_NBIOT               LWIP_PLATFORM_DEFINE_NBIOT
#endif

#if (defined(LTEONLY_THIN) || defined(LTEGSM_THIN)) /*this macro is defined from tavor/build/arbel.mak*/
#undef LWIP_DEFVAL_OPT_C4M
#define LWIP_DEFVAL_OPT_C4M                LWIP_PLATFORM_DEFINE_C4M
#endif

#ifdef MIN_SYS /*this macro is defined from tavor/build/arbel.mak*/
#undef LWIP_DEFVAL_OPT_MINIS
#define LWIP_DEFVAL_OPT_MINIS              LWIP_PLATFORM_DEFINE_MINIS
#endif

#ifdef SINGLE_SIM /*this macro is defined from tavor/build/arbel.mak*/
#undef LWIP_DEFVAL_OPT_C2M
#define LWIP_DEFVAL_OPT_C2M                LWIP_PLATFORM_DEFINE_C2M
#endif

#ifdef NOIMS
#define LWIP_IMS_SUPPORT                (0)
#else
#define LWIP_IMS_SUPPORT                (1)
#endif

/*compiler for platform option: sub2, use lwip self defined macro*/
#else

#ifdef LWIP_PLAT_USE_ALIOS
#undef LWIP_DEFVAL_OPT_ALIOS
#define LWIP_DEFVAL_OPT_ALIOS             	LWIP_PLATFORM_DEFINE_ALIOS
#endif

#ifdef LWIP_PLAT_CRANE_MCU_DONGLE
#undef LWIP_DEFVAL_OPT_DM
#define LWIP_DEFVAL_OPT_DM            		LWIP_PLATFORM_DEFINE_DM
#endif

#ifdef LWIP_PLAT_NBIOT
#undef LWIP_DEFVAL_OPT_NBIOT
#define LWIP_DEFVAL_OPT_NBIOT               LWIP_PLATFORM_DEFINE_NBIOT
#endif

#ifdef LWIP_PLAT_LTEONLY_THIN
#undef LWIP_DEFVAL_OPT_C4M
#define LWIP_DEFVAL_OPT_C4M                LWIP_PLATFORM_DEFINE_C4M
#endif

#ifdef LWIP_PLAT_MIN_SYS
#undef LWIP_DEFVAL_OPT_MINIS
#define LWIP_DEFVAL_OPT_MINIS              LWIP_PLATFORM_DEFINE_MINIS
#endif

#ifdef LWIP_PLAT_LTEONLY_THIN_SINGLE_SIM
#undef LWIP_DEFVAL_OPT_C2M
#define LWIP_DEFVAL_OPT_C2M                LWIP_PLATFORM_DEFINE_C2M
#endif

#ifdef LWIP_PLAT_CRANE_FP_LITE
#undef LWIP_DEFVAL_OPT_FP_LITE
#define LWIP_DEFVAL_OPT_FP_LITE            LWIP_PLATFORM_DEFINE_FP_LITE
#endif

#ifdef LWIP_PLAT_SOCKET
#undef LWIP_DEFVAL_OPT_SOCKET
#define LWIP_DEFVAL_OPT_SOCKET             LWIP_PLATFORM_DEFINE_SOCKET
#endif

#ifdef LWIP_PLAT_NOIMS
#define LWIP_IMS_SUPPORT                (0)
#else
#define LWIP_IMS_SUPPORT                (1)
#endif

#endif
#endif



#define LWIP_PLATFORM_DEFINE				( LWIP_DEFVAL_OPT_PLAT \
											| LWIP_DEFVAL_OPT_ALIOS \
											| LWIP_DEFVAL_OPT_DM \
											| LWIP_DEFVAL_OPT_MINIS \
											| LWIP_DEFVAL_OPT_NBIOT \
											| LWIP_DEFVAL_OPT_C4M \
											| LWIP_DEFVAL_OPT_FP_LITE \
											| LWIP_DEFVAL_OPT_C2M \
											| LWIP_DEFVAL_OPT_SOCKET \
											| LWIP_DEFVAL_OPT_CRANEW \
											| LWIP_DEFVAL_OPT_KESTREL \
											| LWIP_DEFVAL_OPT_2CHIP \
											)

#define IS_LWIP_PLATFORM_1802_1802S         (((LWIP_PLATFORM_DEFINE & (LWIP_PLATFORM_DEFINE_1802 | LWIP_PLATFORM_DEFINE_1802S)) > 0) ? 1 : 0 )
#define IS_LWIP_PLATFORM_1802               (((LWIP_PLATFORM_DEFINE & LWIP_PLATFORM_DEFINE_1802) > 0) ? 1 : 0 )
#define IS_LWIP_PLATFORM_1802S              (((LWIP_PLATFORM_DEFINE & LWIP_PLATFORM_DEFINE_1802S) > 0) ? 1 : 0 )
#define IS_LWIP_PLATFORM_CRANE              (((LWIP_PLATFORM_DEFINE & LWIP_PLATFORM_DEFINE_CRANE) > 0) ? 1 : 0 )

#define IS_LWIP_PLATFORM_MINIS              (((LWIP_PLATFORM_DEFINE & LWIP_PLATFORM_DEFINE_MINIS) > 0) ? 1 : 0 )
#define IS_LWIP_PLATFORM_NBIOT              (((LWIP_PLATFORM_DEFINE & LWIP_PLATFORM_DEFINE_NBIOT) > 0) ? 1 : 0 )
#define IS_LWIP_PLATFORM_USE_ALIOS          (((LWIP_PLATFORM_DEFINE & LWIP_PLATFORM_DEFINE_ALIOS) > 0) ? 1 : 0 )
#define IS_LWIP_PLATFORM_C4M                (((LWIP_PLATFORM_DEFINE & LWIP_PLATFORM_DEFINE_C4M) > 0) ? 1 : 0 )
#define IS_LWIP_PLATFORM_FP_LITE            (((LWIP_PLATFORM_DEFINE & LWIP_PLATFORM_DEFINE_FP_LITE) > 0) ? 1 : 0 )
#define IS_LWIP_PLATFORM_DM                 (((LWIP_PLATFORM_DEFINE & LWIP_PLATFORM_DEFINE_DM) > 0) ? 1 : 0 )
#define IS_LWIP_PLATFORM_C2M                (((LWIP_PLATFORM_DEFINE & LWIP_PLATFORM_DEFINE_C2M) > 0) ? 1 : 0 )
#define IS_LWIP_PLATFORM_SOCKET             (((LWIP_PLATFORM_DEFINE & LWIP_PLATFORM_DEFINE_SOCKET) > 0) ? 1 : 0 )
#define IS_LWIP_PLATFORM_CRANEW             (((LWIP_PLATFORM_DEFINE & LWIP_PLATFORM_DEFINE_CRANEW) > 0) ? 1 : 0 )
#define IS_LWIP_PLATFORM_KESTREL            (((LWIP_PLATFORM_DEFINE & LWIP_PLATFORM_DEFINE_KESTREL) > 0) ? 1 : 0 )
#define IS_LWIP_PLATFORM_2CHIP              (((LWIP_PLATFORM_DEFINE & LWIP_PLATFORM_DEFINE_2CHIP) > 0) ? 1 : 0 )


#endif /* __LWIP_OPTS_H__ */
