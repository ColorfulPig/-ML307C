;------------------------------------------------------------
; (C) Copyright [2006-2008] Marvell International Ltd.
; All Rights Reserved
;------------------------------------------------------------

;/*--------------------------------------------------------------------------------------------------------------------
; INTEL CONFIDENTIAL
; Copyright 2006 Intel Corporation All Rights Reserved.
; The source code contained or described herein and all documents related to the source code ("Material") are owned
; by Intel Corporation or its suppliers or licensors. Title to the Material remains with Intel Corporation or
; its suppliers and licensors. The Material contains trade secrets and proprietary and confidential information of
; Intel or its suppliers and licensors. The Material is protected by worldwide copyright and trade secret laws and
; treaty provisions. No part of the Material may be used, copied, reproduced, modified, published, uploaded, posted,
; transmitted, distributed, or disclosed in any way without Intel's prior express written permission.
;
; No license under any patent, copyright, trade secret or other intellectual property right is granted to or
; conferred upon you by disclosure or delivery of the Materials, either expressly, by implication, inducement,
; estoppel or otherwise. Any license under such intellectual property rights must be express and approved by
; Intel in writing.
; -------------------------------------------------------------------------------------------------------------------*/

;
; MIPS_TEST
; Assembly definitions
;
MIPS_TEST_ADDRESS EQU 0x0C600000

  IF :DEF: ASM_MIPS_TEST

  IF :DEF: DATA_COLLECTOR_IMPL 
	
	INCLUDE ISPT_asm_defs.inc ;ISPT INTC hook interface

  ELSE ; !DATA_COLLECTOR_IMPL
	MACRO
	MIPS_TEST_TRACE $val
	IF :DEF: ASM_MIPS_TEST_RAM
  MOV      r2,#$val
	IMPORT   mipsRamHookAsm
	BL       mipsRamHookAsm
	ELSE
  ;LDR     r1,=MIPS_TEST_ADDRESS
	MOV      r1,#MIPS_TEST_ADDRESS
  MOV      r2,#$val
  STRH     r2,[r1]
	ENDIF

	MEND     ; macro MIPS_TEST_TRACE

	MACRO
	MIPS_TEST_TRACE_R2
	IF :DEF: ASM_MIPS_TEST_RAM
	IMPORT   mipsRamHookAsm
	BL       mipsRamHookAsm
	ELSE
  ;LDR     r1,=MIPS_TEST_ADDRESS
	MOV      r1,#MIPS_TEST_ADDRESS
  STRH     r2,[r1]
	ENDIF

	MEND     ; macro MIPS_TEST_TRACE

  ENDIF ; !DATA_COLLECTOR_IMPL
  ENDIF
	END
