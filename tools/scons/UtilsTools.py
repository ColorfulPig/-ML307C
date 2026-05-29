# -*- coding: utf-8 -*-
# ===================================================================
#
# Copyright (C) 2023 China Mobile IOT. All rights reserved.
#
# Date:
# Author:
# Modify:
# Function: 工具集合
#
# ===================================================================


import os
import stat
import shutil
import glob
import hashlib
import zipfile
import re
import subprocess
from SCons.Script import *
from EnvironConfig import *


def get_app_version():
    """
    Parse APP version from custom_global.h and return x.y.z.
    """
    version_file = os.path.join(PathConfig.root, 'custom', 'custom_main', 'inc', 'custom_global.h')
    version_map = {
        'APP_VERSION_HIGH': '0',
        'APP_VERSION_MID': '0',
        'APP_VERSION_LOW': '0',
    }

    if not os.path.exists(version_file):
        return '0.0.0'

    with open(version_file, 'r', encoding='utf-8', errors='ignore') as fp:
        for line in fp:
            match = re.match(r'\s*#define\s+(APP_VERSION_HIGH|APP_VERSION_MID|APP_VERSION_LOW)\s+(\d+)', line)
            if match:
                version_map[match.group(1)] = match.group(2)

    return '%s.%s.%s' % (
        version_map['APP_VERSION_HIGH'],
        version_map['APP_VERSION_MID'],
        version_map['APP_VERSION_LOW'],
    )


def get_test_app_version():
    """
    Parse test APP version from custom_global.h and return x.y.z.
    This mirrors the runtime g_testAPPVER format.
    """
    version_file = os.path.join(PathConfig.root, 'custom', 'custom_main', 'inc', 'custom_global.h')
    version_map = {
        'testAPP_VERSION_HIGH': '0',
        'testAPP_VERSION_MID': '0',
        'testAPP_VERSION_LOW': '0',
    }

    if not os.path.exists(version_file):
        return '0.0.0'

    with open(version_file, 'r', encoding='utf-8', errors='ignore') as fp:
        for line in fp:
            match = re.match(r'\s*#define\s+(testAPP_VERSION_HIGH|testAPP_VERSION_MID|testAPP_VERSION_LOW)\s+(\d+)', line)
            if match:
                version_map[match.group(1)] = match.group(2)

    return '%s.%s.%s' % (
        version_map['testAPP_VERSION_HIGH'],
        version_map['testAPP_VERSION_MID'],
        version_map['testAPP_VERSION_LOW'],
    )


def get_pkg_base_name():
    """
    Return package base name like ZY_1.0.0.
    """
    return 'ZY_%s' % get_app_version()


def calc_file_md5(file_path):
    md5 = hashlib.md5()
    with open(file_path, 'rb') as fp:
        for chunk in iter(lambda: fp.read(4096), b''):
            md5.update(chunk)
    return md5.hexdigest()


def archive_pkg_outputs(pkg_nm, env):
    """
    Archive generated package by version.
    """
    if not os.path.exists(pkg_nm):
        return

    version = get_app_version()
    test_version = get_test_app_version()
    archive_root = os.path.join(env['IMAGE_DIR'], 'archive')
    app_dir_name = 'App_%s' % version
    test_dir_name = 'Test_%s' % test_version
    archive_dir = os.path.join(archive_root, app_dir_name, test_dir_name)
    archive_zip = os.path.join(archive_dir, os.path.basename(pkg_nm))
    extract_dir = os.path.join(archive_dir, 'package')

    os.makedirs(archive_dir, exist_ok=True)
    shutil.copy2(pkg_nm, archive_zip)

    if os.path.exists(extract_dir):
        shutil.rmtree(extract_dir)
    os.makedirs(extract_dir, exist_ok=True)

    with zipfile.ZipFile(pkg_nm, 'r') as zip_fp:
        zip_fp.extractall(extract_dir)

    for image_name in ('system.img', 'user_app.bin'):
        image_matches = glob.glob(os.path.join(extract_dir, '**', image_name), recursive=True)
        if image_matches:
            shutil.copy2(image_matches[0], os.path.join(archive_dir, image_name))

    print('archive package to: %s' % archive_dir)


def generate_pkg(target, source, env):
    """
    生成固件包
    """
    bs_img_dir = os.path.join(env['IMAGE_DIR'], 'DBG')
    rd_img = os.path.join(bs_img_dir, 'images', 'ReliableData.bin')
    rf_img = os.path.join(bs_img_dir, 'images', 'rf.bin')
    cp_img = os.path.join(bs_img_dir, 'images', 'cp.bin')
    app_img = os.path.join(env['IMAGE_DIR'], env['OUTPUT_BASE'] + '.bin')
    dsp_img = os.path.join(bs_img_dir, 'images', 'dsp.bin')
    upd_img = os.path.join(PathConfig.ABOOT_PATH, 'images', 'updater.bin')
    boot33_img = os.path.join(PathConfig.ABOOT_PATH, 'images', 'boot33_reduce.bin')
    img_info = "cp=%s,rd=%s,rfbin=%s,dsp=%s,boot33_bin=%s,updater=%s,user_app=%s" % (
        cp_img,
        rd_img,
        rf_img,
        dsp_img,
        boot33_img,
        upd_img,
        app_img,
    )
    pkg_nm = os.path.join(env['IMAGE_DIR'], get_pkg_base_name() + '.zip')

    env_tmp = env.Clone()
    env_tmp['aboot_path'] = PathConfig.ABOOT_PATH
    env_tmp['aboot_evb'] = 'ASR1605_EVB'
    env_tmp['aboot_template'] = 'ASR1605_SINGLE_SIM_SMS_GPS_04MB'
    env_tmp['aboot_img_info'] = img_info
    env_tmp['aboot_pkg_name'] = pkg_nm
    aboot_template = 'ASR1605_SINGLE_SIM_SMS_GPS_04MB'

    unzip_cmd_str = env['PKG_UNZIP'] + ' ' + env['BASELINE_DIR'] + ' ' + bs_img_dir
    os.system(unzip_cmd_str)

    if os.path.exists(env['IMAGE_DIR']):
        arerease_bin = '%s/arelease' % PathConfig.ABOOT_PATH
        if os.name == 'nt':
            arerease_bin += '.exe'
        elif not os.access(arerease_bin, os.X_OK):
            os.chmod(arerease_bin, stat.S_IXGRP + stat.S_IRGRP + stat.S_IXUSR + stat.S_IRUSR)

        status = subprocess.run(
            [arerease_bin, '-c', PathConfig.ABOOT_PATH, '-g', '-p',
             'ASR1605_EVB', '-v', aboot_template, '-i', img_info, pkg_nm],
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT
        )

        if status.returncode != 0:
            print('generate package Failed')
            print(status.stdout.decode('utf-8', errors='ignore'))
        else:
            print('generate package Done')
            archive_pkg_outputs(pkg_nm, env)


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
    env['CCCOMSTR'] = 'Compiling $SOURCE'
    env['LINKCOMSTR'] = 'Linking $TARGET'
    env['LSTCOMSTR'] = 'Generating $TARGET'
    env['SECTCOMSTR'] = 'Generating $TARGET'
    env['PACKCOMSTR'] = 'Generating $TARGET'
    env['UTILSCOMSTR'] = 'Generating $TARGET'

    env['PKG_UNZIP'] = 'python ' + File(os.path.join('tools', 'scripts', 'unzip.py')).srcnode().abspath
    env.Append(BUILDERS={'Binary': binary_builder})
    env.Append(BUILDERS={'GenLst': listing_builder})
    env.Append(BUILDERS={'GenLds': lds_builder})
    env.Append(BUILDERS={'GenPkg': pkg_builder})


# Convert from ELF to binary
binary_builder = Builder(
    action=Action('$OBJCOPY -O binary -R .note -R .commnet -S $SOURCE $TARGET', '$UTILSCOMSTR'),
    suffix='.bin',
    src_suffix='.elf'
)

# Generate listing file
listing_builder = Builder(
    action=Action('$OBJDUMP $SOURCE -x -S > $TARGET', '$UTILSCOMSTR'),
    suffix='.lst',
    src_suffix='.elf'
)

# Generate ld script
lds_builder = Builder(
    action=Action('$CC -E -P -w - <$SOURCE -o $TARGET', '$UTILSCOMSTR'),
    suffix='.lds',
    src_suffix='.ld'
)

# Generate release package
pkg_builder = Builder(
    action=Action(generate_pkg, '$UTILSCOMSTR'),
    suffix='.zip',
    src_suffix='.bin'
)
