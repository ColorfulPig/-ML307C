# -*- coding: utf-8 -*-
# ===================================================================
#
# Copyright © 2023 China Mobile IOT. All rights reserved.
#
# Date:     2023/02/17
# Author:   zhangxw
# Function: scons模块构建类
#
# ===================================================================


from EnvironConfig import *


class ModuleBuild:
    """
    模块构建类，除env/name外，其他参数必须以list类型传入
    构建时区分XIP/RAM模式，将ram_srcs/flash_srcs归类，生成的obj分别以.ram.o/.flash.o命名
    构建后返回object列表或library
    """
    def __init__(self, env, name, ram_srcs=None, flash_srcs=None, pub_incs=None, pri_incs=None, cpp_define=None):
        self.pub_incs = pub_incs
        self.pri_incs = pri_incs
        self.cpp_define = cpp_define
        self.obj = []
        self.lib = []
        self.ram_srcs = None
        self.flash_srcs = None
        env.PrependUnique(CPPPATH=pub_incs)  # 公共头文件更新至工程环境中
        self.name = name
        self.env = env.Clone()  # 克隆环境，防止模块串改工程环境
        self.env.PrependUnique(CPPPATH=pri_incs)
        self.env.AppendUnique(CPPDEFINES=cpp_define)

        if ram_srcs is not None and len(ram_srcs) > 0:
            self.ram_srcs = ram_srcs
        if flash_srcs is not None and len(flash_srcs) > 0:
            self.flash_srcs = flash_srcs

    def build_object(self):
        """
        编译生成.o列表
        :return: object集合
        """
        if self.ram_srcs is not None:
            obj_env = self.env.Clone(OBJSUFFIX='.ram.o')  # 拷贝环境变量并设置obj后缀为.ram.o
            obj_list = obj_env.Object(self.ram_srcs)  # 编译生成obj列表
            self.obj.append(obj_list)

        if self.flash_srcs is not None:
            obj_env = self.env.Clone(OBJSUFFIX='.flash.o')  # 拷贝环境变量并设置obj后缀为.flash.o
            obj_list = obj_env.Object(self.flash_srcs)  # 编译生成obj列表
            self.obj.append(obj_list)

        return self.obj

    def build_library(self):
        """
        编译生成.a静态库
        :return: 静态库文件
        """
        self.obj = self.build_object()  # 编译为object
        self.lib = self.env.Library(target=self.name, source=self.obj)  # 打包为静态库
        return self.lib

    def add_private_incs(self, pri_incs):
        """
        增加私有头文件，若使用则须在构建前，仅作用于本模块
        :param pri_incs: 头文件列表
        :return: None
        """
        self.pri_incs.extend(pri_incs)
        self.env.PrependUnique(CPPPATH=pri_incs)

    def add_cpp_defines(self, defines):
        """
        增加模块宏定义，若使用则须在构建前，仅作用于本模块
        :param defines: 宏定义列表
        :return: None
        """
        self.cpp_define.extend(defines)
        self.env.AppendUnique(CPPDEFINES=defines)

    def get_name(self):
        """
        :return: 模块名
        """
        return self.name

    def get_sources(self):
        """
        :return: 源文件列表
        """
        return self.ram_srcs.extend(self.flash_srcs)
