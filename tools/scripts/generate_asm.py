# -*- coding: utf-8 -*-
# ===================================================================
#
# Copyright © 2022 China Mobile IOT. All rights reserved.
#
# Date: 2022/09/08
# Author: zhangxw
# Function: 读入elf明细文件，生成.s汇编文件
#
# Step1:读取elf明细，并将符号表相关的信息存入dict字典内
# Step2:读取export_list信息，通过字典返回地址信息
# Step3:通过asm模板信息，代入func-name、func-addr生成汇编代码
#
# ===================================================================


import os
import sys
import re


# 读取elf明细信息，并生成text段的字典
def handle_elf_file(file):
    with open(file, "r") as target:  # 只读模式读入elf信息

        dict_info = dict()
        line_before = None
        line_undone = False

        for line in target:
            if line_undone is True:  # 上一段落未处理
                line = line_before + line
                line_undone = False
                line_before = None

            line = line.strip()

            # 行检测，数字/字母/下划线
            if re.match("^[\d]+[\s]+[\S]+[\s]+[\w]+", line) is None:  # 此行不符合text段信息
                if re.match("^[\d]+[\s]+[\S]+$", line) is not None:  # 被分行
                    line_undone = True
                    line_before = line

                continue

            list_tmp = line.split()
            dict_info[list_tmp[1]] = str(list_tmp[2])

        return dict_info


# 根据函数名和地址组成汇编代码
def handle_asm(func, addr):
#    asm_str = "/*def %s*/\n" \
#              "    .type %s, %%function\n" \
#              "    .func\n" \
#              "    .global %s\n" \
#              "%s:\n" \
#              "    ldr r7 ,=%s\n" \
#              "    mov pc, r7\n" \
#              "    BX LR\n" \
#              "    .endfunc\n" % (func, func, func, func, addr)

    asm_str = "/*def %s*/\n" \
              "    .type %s, %%function\n" \
              "    .func\n" \
              "    .global %s\n" \
              "    .syntax unified\n" \
              "    .thumb\n" \
              "    .section .text\n" \
              "%s:\n" \
              "    ldr.w pc ,[pc]\n" \
              "    .word %s\n" \
              "    .endfunc\n" % (func, func, func, func, addr)
    return asm_str


# 遍历export_list接口，在字典中查找接口的地址
# 根据函数名和地址生成.s的汇编文件
def generate_asm(dict_info, file_in, file_out):
    with open(file_in, "r") as export_list, \
            open(file_out, "w", encoding='utf-8') as target:
        target.truncate(0)  # 清空文件
        target.write(".arm\n")
        for line in export_list:
            func = line.strip()
            addr = "null"

            if func in dict_info:
                addr = dict_info[func]

            asm_info = handle_asm(func, addr)
            target.write(asm_info)


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("参数错误:", len(sys.argv))
        sys.exit()

    elf_file = str(sys.argv[1])
    func_file = str(sys.argv[2])

    if os.path.isfile(elf_file) is not True:  # 判断文件是否存在
        print("elf明细文件不存在", elf_file)
        sys.exit()

    if os.path.isfile(func_file) is not True:  # 判断文件是否存在
        print("function列表不存在", func_file)
        sys.exit()

    export_dir = os.path.dirname(os.path.realpath(elf_file))
    asm_file = export_dir + "\\cm_core_stub.s"  # 汇编文件名
    dict_info = handle_elf_file(elf_file)  # 生成字典信息
    generate_asm(dict_info, func_file, asm_file)  # 生成汇编文件
