# -*- coding: utf-8 -*-
# ===================================================================
#
# Copyright © 2023 China Mobile IOT. All rights reserved.
#
# Date:     2023/02/24
# Author:   zhangxw
# Modify:   by zhangxw@2023/6/16，适配ML307R平台
# Function: scons工程文件
#
# ===================================================================


import os,stat
import sys
import glob
from EnvironConfig import *
from TestDefine import *
Import('env', 'root_dir', 'target_map')


# 加载编译模块，实现源码与编译文件分离
compnent_list = [
    'src',
    'third-party',
]

if env['DEMO_SUPPORT'].lower() != 'n':  # 编译demo
    compnent_list.append('examples')
    demo_support = env['DEMO_SUPPORT'].lower()
    Export('demo_support')
elif env['TEST_SUPPORT'].lower() == 'y':  # 编译test程序
    compnent_list.append('test')
    test_support_def = []
    for k, v in TestSelect.TestSupport.items():  # 检索test宏定义
        if v.lower() == 'y':
            test_support_def.append(k)
    env.PrependUnique(CPPDEFINES=test_support_def)
else:
    compnent_list.append('custom')

objects = []
for compnent in compnent_list:
    object = SConscript(os.path.join(compnent, 'SConscript'))
    objects.extend(object)

# 检索lib库中的.a文件和.o文件
lib_path = os.path.join(Dir('#').abspath, 'prebuild', 'libs')
lib_files = os.listdir(lib_path)
libs = []
for lib in lib_files:
    if lib.endswith('.a'):
        libs.append(lib)
    elif lib.endswith('.o'):
        objects.append(os.path.join(lib_path, lib))


# 生成可执行文件
elf = env.Program(os.path.join(env['IMAGE_DIR'], env['OUTPUT_BASE']), objects, LIBS = libs)
map = os.path.join(env['IMAGE_DIR'], target_map)
bin = env.Binary(elf)
lds = env.GenLds(os.path.join(env['IMAGE_DIR'], 'linker'), env['LDS_SRC'])
lst = env.GenLst(elf)
pkg = env.GenPkg(bin)

env.AlwaysBuild(pkg)
env.Clean(elf, map)  # 执行scons -c时删除与elf相关的附加文件
env.Clean(elf, os.path.join(env['IMAGE_DIR'], 'DBG'))
env.Requires(elf, lds)
