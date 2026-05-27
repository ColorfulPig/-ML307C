# -*- coding: utf-8 -*-
# ===================================================================
#
# Copyright © 2023 China Mobile IOT. All rights reserved.
#
# Date:
# Author:
# Modify:
# Function: 工具集合
#
# ===================================================================


import shutil, glob, hashlib
from SCons.Script import *
from EnvironConfig import *
import subprocess

def generate_pkg(target, source, env):
    """
    生成固件包
    """
    bs_img_dir = os.path.join(env['IMAGE_DIR'], 'DBG')
    # image fill
    rd_img = os.path.join(bs_img_dir, 'images', 'ReliableData.bin')
    rf_img = os.path.join(bs_img_dir, 'images', 'rf.bin')
    cp_img = os.path.join(bs_img_dir, 'images', 'cp.bin')
    app_img = os.path.join(env['IMAGE_DIR'], env['TARGET_NAME'] + '.bin')
    dsp_img = os.path.join(bs_img_dir, 'images', 'dsp.bin')
    upd_img = os.path.join(PathConfig.ABOOT_PATH, 'images', 'updater.bin')
    boot33_img = os.path.join(PathConfig.ABOOT_PATH, 'images', 'boot33_reduce.bin')
    img_info = "cp=%s,rd=%s,rfbin=%s,dsp=%s,boot33_bin=%s,updater=%s,user_app=%s" % (cp_img, rd_img, rf_img, dsp_img, boot33_img, upd_img, app_img)
    pkg_nm = os.path.join(env['IMAGE_DIR'], env['TARGET_NAME'] + '.zip')

    # unzip base package, and generate_pkg
    env_tmp = env.Clone()
    # env_tmp['aboot_tool'] = os.path.join(PathConfig.ABOOT_PATH, 'arelease')
    env_tmp['aboot_path'] = PathConfig.ABOOT_PATH
    env_tmp['aboot_evb'] = 'ASR1605_EVB'
    env_tmp['aboot_template'] = 'ASR1605_SINGLE_SIM_SMS_GPS_04MB'
    env_tmp['aboot_img_info'] = img_info
    env_tmp['aboot_pkg_name'] = pkg_nm
    aboot_template = 'ASR1605_SINGLE_SIM_SMS_GPS_04MB'
    #aboot_template = 'ASR1605_SINGLE_SIM_SMS_04MB' if env['MODULE_SUB_NAME'] == 'DC-CN' else 'ASR1605_SINGLE_SIM_02MB'
    #cc = env_tmp.Command(bs_img_dir, env['BASELINE_DIR'], '$PKG_UNZIP $SOURCE $TARGET')
    #dd = env_tmp.Command('high', cc, 'arelease -c $aboot_path -g -p $aboot_evb -v $aboot_template -i $aboot_img_info $aboot_pkg_name')
    #env_tmp.Command([bs_img_dir, 'hhh'], env['BASELINE_DIR'],
    #                [
    #                '$PKG_UNZIP $SOURCE $TARGET',
    #                'arelease -c $aboot_path -g -p $aboot_evb -v $aboot_template -i $aboot_img_info $aboot_pkg_name'
    #                ])
    unzip_cmd_str = env['PKG_UNZIP'] + ' ' + env['BASELINE_DIR'] + ' ' + bs_img_dir
    os.system(unzip_cmd_str)

    # gen_cmd_str = 'tools\\aboot\\arelease -c %s -g -p ASR1602_EVB -v ASR1602_SINGLE_SIM_SMS_04MB -i %s %s > nul' % (PathConfig.ABOOT_PATH, img_info, pkg_nm)
    # os.system(gen_cmd_str)
    if os.path.exists(env['IMAGE_DIR']):
        arerease_bin = '%s/arelease' % PathConfig.ABOOT_PATH
        if os.name =='nt':
            arerease_bin += '.exe'
        elif not os.access(arerease_bin, os.X_OK):
            os.chmod(arerease_bin, stat.S_IXGRP + stat.S_IRGRP + stat.S_IXUSR + stat.S_IRUSR)
        status = subprocess.run([arerease_bin, '-c', PathConfig.ABOOT_PATH, '-g', '-p', 
            'ASR1605_EVB', '-v', aboot_template, '-i', img_info, pkg_nm ], 
            stdout = subprocess.PIPE, stderr = subprocess.STDOUT)
        if status.returncode != 0:
            print('generate package Failed')
            print(status.stdout.decode('utf-8'))  # 解码并打印错误信息
        else:
            print('generate package Done')


def install_tools(env):
    """
    基于工程环境安装个性化工具
    :param env: 工程环境变量
    :return: None
    """
    env['CC'] = BuildConfig.CC
    env['CXX'] = BuildConfig.CXX
    env['AR'] = BuildConfig.AR
    env['LINK'] = BuildConfig.CC
    env['SZ'] = BuildConfig.SZ
    env['OBJCOPY'] = BuildConfig.COPY
    env['OBJDUMP'] = BuildConfig.OBJDUMP
    env['READELF'] = BuildConfig.READELF
    env['ABOOT'] = 'arelease'
    env['CCFLAGS'] = BuildConfig.COMMON_CCFLAGS
    env['LINKFLAGS'] = BuildConfig.COMMON_LINKFLAGS
    env['CPPDEFINES'] = BuildConfig.COMMON_DEFS
    env['CPPPATH'] = BuildConfig.COMMON_INCS
    env['LIBS'] = BuildConfig.LIBS
    env['LIBPATH'] = [
        os.path.join(Dir('#').abspath, 'prebuild', 'libs'),
        ] + BuildConfig.LIBPATH 
    env['LNPATH'] = [os.path.join(PathConfig.LINK_DIR)]
    env['LIB_GROUP_PREFIX'] = ['-Wl,--start-group']
    env['LIB_GROUP_SUFFIX'] = ['-Wl,--end-group']
    env['LINKCOM'] = '$LINK -o $TARGET $LINKFLAGS $__RPATH $SOURCES $_LIBDIRFLAGS ' \
                     '$LIB_GROUP_PREFIX $_LIBFLAGS -lm -lgcc $LIB_GROUP_SUFFIX'
    env['CCCOMSTR'] = 'Compiling $SOURCE'  # 自定义编译输出
    env['LINKCOMSTR'] = 'Linking $TARGET'  # 自定义链接输出
    env['LSTCOMSTR'] = 'Generating $TARGET'
    env['SECTCOMSTR'] = 'Generating $TARGET'
    env['PACKCOMSTR'] = 'Generating $TARGET'
    env['UTILSCOMSTR'] = 'Generating $TARGET'

    env['PKG_UNZIP'] = 'python ' + File(os.path.join('tools', 'scripts', 'unzip.py')).srcnode().abspath
    env.Append(BUILDERS = {'Binary': binary_builder}) # 增加自定义命令
    env.Append(BUILDERS = {'GenLst': listing_builder})
    env.Append(BUILDERS = {'GenLds': lds_builder})
    env.Append(BUILDERS = {'GenPkg': pkg_builder})


# Convert from ELF to binary
binary_builder = Builder(
    action = Action('$OBJCOPY -O binary -R .note -R .commnet -S $SOURCE $TARGET', '$UTILSCOMSTR'),
    suffix ='.bin',
    src_suffix ='.elf'
)

# Generate listing file
listing_builder = Builder(
    action =  Action('$OBJDUMP $SOURCE -x -S > $TARGET', '$UTILSCOMSTR'),
    suffix ='.lst',
    src_suffix ='.elf'
)

# Generate ld script
lds_builder = Builder(
    action =  Action('$CC -E -P -w - <$SOURCE -o $TARGET', '$UTILSCOMSTR'),
    suffix ='.lds',
    src_suffix ='.ld'
)

# Generate release package
pkg_builder = Builder(
    action =  Action(generate_pkg, '$UTILSCOMSTR'),
    suffix ='.zip',
    src_suffix ='.bin'
)
