#*********************************************************
#  @file    Makefile
#  @brief   ML307C OpenCPU Makefile
#  Copyright (c) 2024 China Mobile IOT.
#  All rights reserved.
#  created by zxw@20240409
#*******************************************************


.PHONY: all clean

# 脚本参数
PRJ_DIR := $(PRJ_DIR)
TARGET_NAME := $(TARGET_NAME)
AUDIO := PWM


# 库文件及用户代码
SRC_FILES :=
INCLUDES :=
CDEFINES :=
PREDEFINE :=
include kernel/kernel.mk
include kernel/common/default_dflags.mk


# 版本控制及目标文件
ifeq ($(strip $(TARGET_NAME)),ML307C)
SCT_FILE := kernel/common/mem/CraneLR_DS_4M_Ram_4M_Flash_XIP_CIPSRAM_Common.sct
FLASH_TYPE := 4M
RN_GENERAL_INFO := CRANELRH_DS_TX_Z2A0_LTEONLY_DKB_XIP_LWIP_MODULE_CIRAM_LTEONLY_THIN_SINGLE_SIM_4M_SMS
EXPORT_LIST := kernel/common/$(TARGET_NAME)/export/core_export.list
BSLINE_DIR := prebuild/baseline
CORE_STUB_DIR := prebuild/libs
CORE_STUB_FILE := cm_stub.o
endif

# 
LIBNAME := lib

EXPORT_C_FILE := kernel/common/$(TARGET_NAME)/export/cm_export_func.c

# GCC配置
CC := armcc
CXX := armcc
AR := armar
LD := armlink
FROMELF := fromelf
MAKE := gnumake
PPC := PPC
PPL := PPL
PPU := PPU
PPMAN := PPManCmd

# 参数配置

KERNEL_TARGET := $(TARGET_NAME)
OBJ_DIR := $(PRJ_DIR)/kernel/obj
IMG_DIR := $(PRJ_DIR)/kernel/img
ASR_TOOL := $(PRJ_DIR)/tools/win/asrtool
SRC_FILES += $(EXPORT_C_FILE)
SRC_DIRS := $(sort $(dir $(SRC_FILES)))
OBJS := $(addprefix $(OBJ_DIR)/, $(subst .c,.o,$(subst .cpp,.o,$(SRC_FILES))))
PKG_LIB := $(IMG_DIR)/kernel.lib
COMPILED_LIB_LIST := $(IMG_DIR)/compiled_liblist.txt
COMPILED_PP_LIST := $(IMG_DIR)/compiled_pplist.txt
PREBUILT_LIB_LIST := $(PRJ_DIR)/kernel/common/$(TARGET_NAME)/$(LIBNAME)/prebuilt_liblist.txt
PREBUILT_PP_LIST := $(PRJ_DIR)/kernel/common/$(TARGET_NAME)/$(LIBNAME)/prebuilt_pplist.txt
FEEDBACK_LIST := $(IMG_DIR)/feedbacklinkopt.txt
TXT_MDB_FILE := $(KERNEL_TARGET)_MDB_FILE_NEW.txt
DIAGDB_C_FILE := diagDB.c
DEPS := $(subst .o,.d, $(OBJS))


# 编译参数
PREDEFINE += -DLTEONLY_THIN_SINGLE_SIM
INCLUDES += -I$(PRJ_DIR)/kernel/common/inc
INCLUDES += -Iinclude\platform\lwip \
			-Iinclude\platform\lwip\arch \
			-Iinclude\platform\lwip\ipsec \
			-Iinclude\platform\lwip\ipv4 \
			-Iinclude\platform\lwip\ipv6 \
			-Iinclude\platform\lwip\lwip \
			-Iinclude\platform\lwip\netif \
			-Iinclude\cmiot \
			-Iinclude\platform\AuC
CFLAGS :=  -c --cpu Cortex-R4 --no_unaligned_access -g -O2 --apcs /inter --diag_suppress 2084,1,2,177,550,1296,2795,6319,9931,9933 --diag_error=warning --gnu --thumb --loose_implicit_cast
LDFLAGS := --via $(COMPILED_LIB_LIST) \
		 --via $(PREBUILT_LIB_LIST) \
	     --elf --scatter $(SCT_FILE)  \
	     --predefine="$(PREDEFINE)" \
	     --predefine="-DL1_WIFI_LOCATION" \
	     --predefine="-DREDUCE_SULOG_IRBUF_DSP" \
         --predefine="-DCRANELRH_SUPPORT" \
	     --map --symbols --info sizes,totals  \
	     --keep init.o(Header) \
	     --keep init.o(Vectors) \
	     --diag_suppress 6312,6314,6319,6329 \
	     --feedback $(FEEDBACK_LIST)
ARFLAG := -v --create -r

# 生成IMG文件
all:  export check $(KERNEL_TARGET) copy_file_to_sdk


$(KERNEL_TARGET): $(PKG_LIB) $(IMG_DIR)/$(TXT_MDB_FILE)
	@echo 'create [cp.bin]'
	@if exist $(IMG_DIR)/$@_NO_TIM.bin (@del /s/q $(subst $(strip /),\,$(IMG_DIR)/$@_NO_TIM.bin))
	@if exist $(IMG_DIR)/cp.bin (@del /s/q $(subst $(strip /),\,$(IMG_DIR)/cp.bin))
	$(LD)  $(LDFLAGS) -o $(IMG_DIR)/cp.axf --list $(IMG_DIR)/cp.map
	@cd $(IMG_DIR) &( \
	@$(FROMELF) --bin --output $@.bin cp.axf &&\
	@ren $@.bin $@_NO_TIM.bin &&\
	platform -p DKB_SS -t EVB_3  -i NONE  -b $@_NO_TIM.bin -o cp.bin &&\
	perl external_rw_region_lzma.pl cp.bin $(RN_GENERAL_INFO) &&\
	perl external_loadtable_update.pl cp.bin $(RN_GENERAL_INFO))
	@echo "Done"

export: check
	python tools\scripts\export_list.py $(EXPORT_LIST) $(IMG_DIR)\export_list.list
#	$(CC) $(CFLAGS) $(EXPORT_INCLUDES) $(CDEFINES)  -o $(patsubst %.c,%.o,$(EXPORT_C_FILE)) $(EXPORT_C_FILE)


check:
	@if not exist $(IMG_DIR) ( \
		md $(subst $(strip /),\,$(IMG_DIR)) &&\
		xcopy /E/Y/I $(subst $(strip /),\,$(ASR_TOOL)) $(subst $(strip /),\,$(IMG_DIR)\) \
	)
	@if not exist $(OBJ_DIR) (md $(subst $(strip /),\,$(OBJ_DIR)))
	@for /d %%y in ($(subst $(strip /),\,$(SRC_DIRS))) do ( \
		@if not exist $(OBJ_DIR)\%%y (md $(subst $(strip /),\,$(OBJ_DIR)\%%y)) \
	)


# 拷贝文件至SDK中
# core_stub.o
# cp.bin
# MDB.txt
copy_file_to_sdk: $(KERNEL_TARGET)
	@echo 'copy files[...]'
	@($(FROMELF) -s $(IMG_DIR)\cp.axf > $(IMG_DIR)\elf_info.txt) &\
	(python tools\scripts\generate_asm.py $(IMG_DIR)\elf_info.txt $(IMG_DIR)\export_list.list) &\
	(arm-none-eabi-gcc -mthumb-interwork -std=gnu11 -mcpu=cortex-r5 -mno-unaligned-access -g -w -ffunction-sections -fdata-sections -c $(IMG_DIR)\cm_core_stub.s -o $(IMG_DIR)\$(CORE_STUB_FILE))
	@cd $(IMG_DIR) &\
	(copy /y cp.bin $(subst $(strip /),\,$(PRJ_DIR)\$(BSLINE_DIR))) &\
	(copy /y cp.axf $(subst $(strip /),\,$(PRJ_DIR)\$(BSLINE_DIR))) &\
	(copy /y cp.map $(subst $(strip /),\,$(PRJ_DIR)\$(BSLINE_DIR))) &\
	(copy /y $(TXT_MDB_FILE) $(subst $(strip /),\,$(PRJ_DIR)\$(BSLINE_DIR))) &\
	(copy /y $(CORE_STUB_FILE) $(subst $(strip /),\,$(PRJ_DIR)\$(CORE_STUB_DIR)))


# 生成txt文件
$(IMG_DIR)/$(TXT_MDB_FILE): $(COMPILED_PP_LIST) $(PKG_LIB)
	@echo 'create mdb txt [$@]'
	@cd $(IMG_DIR) &\
	SETLOCAL ENABLEDELAYEDEXPANSION &\
	(for /f "delims=" %%i in ('type $(subst $(strip /),\, $(PREBUILT_PP_LIST))') do (set prebuilt_pp_ret=!prebuilt_pp_ret!%%i )) & \
	(for /f "delims=" %%i in ('type $(subst $(strip /),\, $(COMPILED_PP_LIST))') do (set compiled_pp_ret=!compiled_pp_ret!%%i )) & \
	($(PPL) $(TXT_MDB_FILE) $(DIAGDB_C_FILE) !prebuilt_pp_ret! !compiled_pp_ret! -f ..\common\$(TARGET_NAME)\mdb\3G_PS_Filter_file.txt -f ..\common\$(TARGET_NAME)\mdb\APLP_Filter_file.txt -f ..\common\$(TARGET_NAME)\mdb\diag_list.txt 1>nul) &\
	ENDLOCAL &\
	($(PPMAN) -S..\common\$(TARGET_NAME)\mdb\hsiupdlibdev.i -N$(TXT_MDB_FILE)) &\
	($(PPU) $@ $(DIAGDB_C_FILE) ..\common\$(TARGET_NAME)\mdb\diagDB.txt) &\
	($(CC) $(CFLAGS) $(CDEFINES) -I..\common\inc -o $(patsubst %.c,%.o,$(DIAGDB_C_FILE)) $(DIAGDB_C_FILE)) &\
	(copy /y $(patsubst %.c,%.o,$(DIAGDB_C_FILE)) ..\common\$(TARGET_NAME)\$(LIBNAME)) &\
	@cd ..


# 生成库文件
$(PKG_LIB): $(OBJS)
	@echo 'create lib [$@]'
	$(AR) $(ARFLAG) $@ $^ 
	@type $(patsubst %.o,%.pp,$(subst $(strip /),\,$^)) > $(patsubst %.lib,%.pp,$(subst $(strip /),\,$@))
	@echo $(PKG_LIB) > $(COMPILED_LIB_LIST)
	@echo $(patsubst %.lib,%.pp,$(subst $(strip /),\,$@))  > $(COMPILED_PP_LIST)


# C编译
$(OBJ_DIR)/%.o: %.c
	@echo 'compile $<...'
	$(CC) -E -C -D__PPC_ $(subst $(strip /),\,$<) $(CFLAGS) $(INCLUDES) $(CDEFINES) -o $(patsubst %.o,%.ppp,$(subst $(strip /),\,$@)) && $(PPC) $(patsubst %.o,%.ppp,$(subst $(strip /),\,$@)) $(patsubst %.o,%.pp,$(subst $(strip /),\,$@))
	$(CC) $(CFLAGS) $(INCLUDES) $(CDEFINES)  -o $@ $< --depend=$(patsubst %.c,$(subst $(strip \),/,$(OBJ_DIR))/%.d,$<)

# CPP编译
$(OBJ_DIR)/%.o: %.cpp
	@echo 'compile $<...'
	$(CXX) -E -C -D_PPC_ $(subst $(strip /),\,$<) $(CFLAGS) $(INCLUDES) $(CDEFINES) -o $(patsubst %.o,%.ppp,$(subst $(strip /),\,$@)) && $(PPC) $(patsubst %.o,%.ppp,$(subst $(strip /),\,$@)) $(patsubst %.o,%.pp,$(subst $(strip /),\,$@))
	$(CXX) $(CFLAGS) $(INCLUDES) $(CDEFINES) -o $@ $< --depend=$(patsubst %.c,$(subst $(strip \),/,$(OBJ_DIR))/%.d,$<)


# 清除编译文件	
clean:
	@echo 'clear compiled files'
	@if exist $(IMG_DIR) (@echo clean $(IMG_DIR) & rd /s/q $(subst $(strip /),\,$(IMG_DIR)))
	@if exist $(OBJ_DIR) (@echo clean $(OBJ_DIR) & rd /s/q $(subst $(strip /),\,$(OBJ_DIR)))

-include $(DEPS)