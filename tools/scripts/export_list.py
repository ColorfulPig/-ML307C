# -*- coding: utf-8 -*-
# ===================================================================
#
# Copyright © 2022 China Mobile IOT. All rights reserved.
#
# Date: 2022/09/07
# Author: zhangxw
# Function: 强制生成export_list的接口调用源文件及接口列表
#
# ===================================================================


import re
import os
import sys


# 检索头文件
def handle_header(file_in, file_out):
    with open(file_in, "r") as export_list, \
            open(file_out, "w") as target:
        target.truncate(0)  # 清空文件
        for line in export_list:
            line = line.strip()
            # 行检测，#include
            if re.match("^#include[\s]*[\w.<>\"]+$", line) is None:  # 此行不符合头文件引用规范
                continue
            target.write(line)
            target.write("\n")

        target.write("\n\n")


# 检索API接口
def handle_line(file_in, file_out_c, file_out_func):
    with open(file_in, "r") as export_list, \
            open(file_out_c, "a") as target_c, \
            open(file_out_func, "w") as target_func:
        func_start = "int cm_core_stub_init(void)\n{\n    int test = 0;\n"
        target_c.write(func_start)  # 写入函数头部信息
        target_func.truncate(0)  # 清空文件

        for line in export_list:
            line = line.strip()
            # 行检测，数字/字母/下划线
            if re.match("^[\w]+$", line) is None:  # 此行不符合API接口命名规范
                continue

            func_str = "    test += (int)" + line + ";\n"
            target_c.write(func_str)
            target_func.write(line + "\n")

        func_end = "    return test;\n}"
        target_c.write(func_end)


# export_list清单文件
if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("参数错误:", len(sys.argv))
        sys.exit()

    input_file = str(sys.argv[1])
    func_file = str(sys.argv[2])
    if os.path.isfile(input_file) is not True:  # 判断bin文件是否存在
        print("bin文件不存在", input_file)
        sys.exit()

    export_dir = os.path.dirname(os.path.realpath(input_file))
    export_file = export_dir + "\\cm_export_func.c"
    handle_header(input_file, export_file)
    handle_line(input_file, export_file, func_file)
