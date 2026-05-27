# -*- coding: utf-8 -*-
# Copyright © 2023 China Mobile IOT. All rights reserved.
#
# Date:     2023/02/20
# Author:   zhangxw
# Function: 编译环境配置
#
# ===================================================================


from SCons.Script import *


class VersionConfig:
    MODULE_BUILDTIME = time.strftime("%Y%m%d%H%M%S", time.localtime())
    MODULE_GIT_VERSION = ''
    MODULE_RELEASE_VERSION = 'alpha'
    TARGET_NAME = 'ML307C'

    @staticmethod
    def get_module_name(env):
        return env['MODULE_NAME']

    @staticmethod
    def get_build_time():
        return VersionConfig.MODULE_BUILDTIME

    @staticmethod
    def init_module_version(env, ARGUMENTS):
        env['MODULE_NAME'] = ARGUMENTS.get('module', 'ML307C')
        env['MODULE_TARGET'] = env['MODULE_NAME']
        env['TARGET_NAME'] = env['MODULE_TARGET'] + '_APP'
        env['DEMO_SUPPORT'] = ARGUMENTS.get('demo', 'n')
        env['TEST_SUPPORT'] = ARGUMENTS.get('test', 'n')


class BuildConfig:
    root = os.path.join(Dir('#').abspath)
    triple = 'arm-none-eabi-'
    CPU = 'arm'
    CC = triple + 'gcc'
    CXX = triple + 'g++'
    AR = triple + 'ar'
    SZ = triple + 'size'
    COPY = triple + 'objcopy'
    OBJDUMP = triple + 'objdump'
    READELF = triple + 'readelf'
    LIBPATH = [
        # os.path.join(root, 'prebuild', 'libs'),
    ]
    LIBS = [
        # 'core',
    ]
    COMMON_INCS = [
        '#include/cmiot',
        '#include/platform',
        '#include/platform/AudioHAL',
        '#include/platform/lwip',
        '#include/platform/lwip/arch',
        '#include/platform/lwip/ipsec',
        '#include/platform/lwip/ipv4',
        '#include/platform/lwip/ipv6',
        '#include/platform/lwip/lwip',
        '#include/platform/lwip/netif',
    ]
    COMMON_DEFS = [  # 全局宏定义
        '_SYS_SELECT_H'
    ]
    COMMON_LINKFLAGS = [
        '-mcpu=cortex-r5',
        '-mtune=generic-armv7-r',
        '-Wl,--gc-sections',
        '-mno-unaligned-access',
        '-nostartfiles',
        '--specs=nosys.specs'
    ]
    COMMON_CCFLAGS = [  # 编译参数
        '-mthumb-interwork',
        '-std=gnu11',
        '-mcpu=cortex-r5',
        '-mno-unaligned-access',
        '--specs=nosys.specs',
        '-Wall',
        '-ffunction-sections',
        '-fdata-sections',
        '-Os'
    ]


class PathConfig:
    root = os.path.join(Dir('#').abspath)
    prebuild = os.path.join(root, 'prebuild')
    GUN_PATH = os.path.join(root, 'tools', 'toolchain', 'gcc-arm-none-eabi', 'bin')  # 使用join方法屏蔽平台路径差异
    ABOOT_PATH = os.path.join(root, 'tools', 'aboot')
    LINK_DIR = os.path.join(prebuild, 'ld')
    BASELINE_DIR = os.path.join(prebuild, 'baseline')
    BUILD_DIR = os.path.join(root, 'out')
    OBJECT_DIR = os.path.join(BUILD_DIR, 'obj')
    IMAGE_DIR = os.path.join(BUILD_DIR, 'image')
