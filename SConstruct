# -*- coding: utf-8 -*-
# ===================================================================
#
# Copyright © 2023 China Mobile IOT. All rights reserved.
#
# Date:
# Author:
# Modify:
# Function: scons工程文件
#
# ===================================================================


import os
import sys
import subprocess

# scons脚本路径添加至系统
sys.path.append(os.path.realpath(os.path.join('tools', 'scons')))
from EnvironConfig import *
from UtilsTools import *

# 系统环境配置
op_sys = Platform()
print("Operation system: %s" % op_sys)

env = Environment(
    ENV={'PATH': os.environ['PATH']},
    tools=['default', 'gcc', 'g++', 'gas', 'ar', 'gnulink'],
    toolpath=['tools/scons'],
    PROGSUFFIX='.elf',
    BUILD_DIR=PathConfig.BUILD_DIR,
)
env.PrependENVPath('PATH', PathConfig.GUN_PATH)  # GUN工具路径
env.PrependENVPath('PATH', PathConfig.ABOOT_PATH)  # aboot工具路径
# print(env['ENV']['PATH'])

# 读取命令行参数
# debug = ARGUMENTS.get('debug', 0)

def install_spawn(env):
    """
    命令行太长时(通常是链接命令)，绕过shell，使用subprocess执行
    """
    # This code is from the SCons wiki: https://github.com/SCons/scons/wiki/LongCmdLinesOnWin32
    if env['PLATFORM'] == 'win32':
        old_spawn = env['SPAWN']

        def my_spawn(sh, escape, cmd, args, spawnenv):
            if ">" in args or "<" in args:  # 忽略重定向命令
                return old_spawn(sh, escape, cmd, args, spawnenv)

            if "arm-none-eabi" in cmd:  # 子进程PATH环境无法传入
                cmd = PathConfig.GUN_PATH + os.path.sep + cmd

            newargs = ' '.join(args[1:])
            cmdline = cmd + " " + newargs
            startupinfo = subprocess.STARTUPINFO()
            startupinfo.dwFlags |= subprocess.STARTF_USESHOWWINDOW
            proc = subprocess.Popen(cmdline, startupinfo = startupinfo, shell = False, env = spawnenv)
            # data, err = proc.communicate()
            rv = proc.wait()
            return rv

        env['SPAWN'] = my_spawn


# 生成工程环境
root_dir = os.path.join(Dir('#').abspath)
VersionConfig.init_module_version(env, ARGUMENTS)
install_tools(env)  # 安装工具
install_spawn(env)
env['IMAGE_DIR'] = os.path.join(PathConfig.IMAGE_DIR)
env['OBJECT_DIR'] = os.path.join(PathConfig.OBJECT_DIR)
env['BASELINE_DIR'] = os.path.join(PathConfig.BASELINE_DIR)
env['LDS_SRC'] = os.path.join(PathConfig.LINK_DIR, 'app.ld')
env['OUTPUT_BASE'] = get_pkg_base_name()


# 链接配置
# ①链接器文件生成
# ②链接器，map文件配置
target_map = env['OUTPUT_BASE'] + '.map'
env.AppendUnique(LINKFLAGS=['-L%s' % env['IMAGE_DIR']])
env.AppendUnique(LINKFLAGS=['-T%s' % 'linker.lds'])
env.AppendUnique(LINKFLAGS=['-Wl,-Map=' + os.path.join(env['IMAGE_DIR'], target_map)])


Export('env', 'root_dir', 'target_map')
SConscript('SConscript', variant_dir=env['OBJECT_DIR'], duplicate=0)
