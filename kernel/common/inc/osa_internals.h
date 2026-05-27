/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

/* ===========================================================================
File        : osa_internals.h
Description : Definition of OSA Software Layer data types that are internal in OSA
                and are OS independent.

Notes       :

=========================================================================== */
#ifndef _OSA_INTERNALS_H
#define _OSA_INTERNALS_H

/*
 * Special OS Definitions for common files.
 */
#if (defined OSA_WINCE)
    #ifdef OSA_USE_ASSERT
        #undef  OSA_USE_ASSERT      //  Asserts in OSA arn't supported in WINCE.
    #endif
    #ifndef NODIAG
        #define NODIAG              //  Diag in OSA isn't supported in WINCE.
    #endif
#endif

#ifdef CODE_COVERAGE
    void codeCoverageTag(unsigned int , unsigned char * );
    #define CODE_COVERAGE_TAG    codeCoverageTag(__LINE__, __FILE__);
#else
    #define CODE_COVERAGE_TAG
#endif

/*
 * ASSERT Macro.
 */
#if defined(OSA_USE_ASSERT)
    #if defined(OSA_NUCLEUS)
        #if defined(OSA_USE_ASSERT)
            #include "utils.h"
            #if defined(OSA_ASSERT)
                #undef  OSA_ASSERT
            #endif
            #define OSA_ASSERT       ASSERT
        #endif
    #else
        #define OSA_ASSERT(cOND)    if ( !(cOND) ) Osa_AssertFail(__FILE__, __LINE__);
    #endif
#else
      #define   OSA_ASSERT(cOND)
#endif

/*
 * Defines.
 */

/*
 * Macros.
 */
#define     RETURN_STATUS_ON_FAILURE(sTATUS,oK)                             \
    {                                                                       \
        if ( sTATUS != oK )                                                 \
        {                                                                   \
            OSA_STATUS oSAsTATUS ;                                          \
                                                                            \
            if ( Osa_TranslateErrorCode((char *)__FUNCTION__,sTATUS,&oSAsTATUS) )   \
                ASSERT_EXT(0, "Tx Status 0x%x, OSA Status 0x%x", sTATUS, oSAsTATUS) ;         \
                                                                            \
            return oSAsTATUS ;                                              \
        }                                                                   \
    }

#define     RETURN_STATUS(sTATUS,oK)                \
    {                                               \
        RETURN_STATUS_ON_FAILURE(sTATUS,oK)         \
                                                    \
        return OS_SUCCESS ;                         \
    }

/*
 * Functions.
 */
void        Osa_Init( void ) ;
OSA_STATUS  OsaMem_InitPools( void ) ;

#endif
