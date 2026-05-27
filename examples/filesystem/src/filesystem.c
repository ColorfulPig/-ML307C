/**
 * @file        network.c
 * @brief       网络状态异步回调通知，示例仅供参考
 * @copyright   Copyright © 2024 China Mobile IOT. All rights reserved.
 * @author      by yangqiang
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stdarg.h"
#include "cm_mem.h"
#include "cm_sys.h"
#include "cm_os.h"
#include "cm_pm.h"
#include "cm_fs.h"

#define test_file1 "test1.txt"
#define test_file2 "test2.txt"

/* 文件系统测试线程 */
void cm_filesystem_task_test(void *p)
{
    char buf[64] = {0};

    /* 1. 获取文件系统大小、剩余空间 */
    cm_fs_system_info_t fs_info = {0};
    cm_fs_getinfo(&fs_info);
    cm_log_printf(0,"[1]file system total:%d,remain:%d", fs_info.total_size, fs_info.free_size);

    /* 2. 获取测试文件的大小 */
    cm_log_printf(0,"[2]<<%s>> file size is:%d", test_file1,cm_fs_filesize(test_file1));

    /* 3. 测试文件系统的正常读写 */
    /* CM_FS_WB:新建测试文件，已存在的文件内容清空，只允许写 */
    int32_t fd = cm_fs_open(test_file1, CM_FS_WB);
    cm_fs_write(fd, "01234", 5);
    cm_fs_write(fd, "56789", 5);    //自动偏移写入
    cm_fs_close(fd);

    /* CM_FS_RB:文件必须存在，只允许读 */
    fd = cm_fs_open(test_file1, CM_FS_RB);
    int32_t f_len = cm_fs_read(fd, buf, 20);
    cm_log_printf(0,"[3]<<%s>> read len:%d, buf:%s\n", test_file1, f_len, buf);
    cm_fs_close(fd);

    /* 获取测试文件的大小 */
    cm_log_printf(0,"[3]<<%s>> file size is:%d", test_file1,cm_fs_filesize(test_file1));

    /* 4. 测试文件系统的追写 */
    /* CM_FS_AB:打开或新建一个二进制文件，只允许在文件末尾追写 */
    fd = cm_fs_open(test_file1, CM_FS_AB);
    cm_fs_write(fd, "abcde", 5);    //在测试文件内容后追写5字节数据
    cm_fs_close(fd);
    fd = cm_fs_open(test_file1, CM_FS_RB);
    memset(buf,0,sizeof(buf));
    f_len = cm_fs_read(fd, buf, 20);
    cm_log_printf(0,"[4]<<%s>> read len:%d, buf:%s\n", test_file1, f_len, buf);
    cm_fs_close(fd);

    /* 5. 测试文件系统的偏移读：到该步骤当前测试文档内容为“0123456789abcde” */
    /* 5.1 从头部开始偏移读  */
    fd = cm_fs_open(test_file1, CM_FS_RB);
    memset(buf,0,sizeof(buf));
    cm_fs_seek(fd,5,CM_FS_SEEK_SET);    //从头部偏移5个字节读取
    f_len = cm_fs_read(fd, buf, 20);
    cm_log_printf(0,"[5.1]<<%s>> read len:%d, buf:%s\n", test_file1, f_len, buf);
    cm_fs_close(fd);

    /* 5.2 从当前位置开始偏移读  */    
    fd = cm_fs_open(test_file1, CM_FS_RB);
    memset(buf,0,sizeof(buf));
    cm_fs_seek(fd,5,CM_FS_SEEK_SET);    //先从头部偏移5个字节
    cm_fs_seek(fd,5,CM_FS_SEEK_CUR);    //再从当前位置偏移5个字节读取
    f_len = cm_fs_read(fd, buf, 20);
    cm_log_printf(0,"[5.2]<<%s>> read len:%d, buf:%s\n", test_file1, f_len, buf);

    /* 5.3 从尾部开始偏移读  */    
    fd = cm_fs_open(test_file1, CM_FS_RB);
    memset(buf,0,sizeof(buf));
    cm_fs_seek(fd,0,CM_FS_SEEK_END);    //从尾部偏移0字节读取
    f_len = cm_fs_read(fd, buf, 20);
    cm_log_printf(0,"[5.3]<<%s>> read len:%d, buf:%s\n", test_file1, f_len, buf);
    cm_fs_close(fd);

    /* 6. 测试文件系统的偏移写（覆盖写）：到该步骤当前测试文档内容为“0123456789abcde” */
    fd = cm_fs_open(test_file1, CM_FS_RBPLUS);
    cm_fs_seek(fd,5,CM_FS_SEEK_SET);    //从头部偏移5个字节写
    cm_fs_write(fd, "abcde", 5);    //写5个字节
    cm_fs_close(fd);

    fd = cm_fs_open(test_file1, CM_FS_RB);
    memset(buf,0,sizeof(buf));
    f_len = cm_fs_read(fd, buf, 20);
    cm_log_printf(0,"[6]<<%s>> read len:%d, buf:%s\n", test_file1, f_len, buf);
    cm_fs_close(fd);

    /* 7. 修改文件名 */
    cm_fs_move(test_file1,test_file2);
    cm_log_printf(0,"[7]<<%s>> file exit state:%d,file size:%d\n", test_file1,cm_fs_exist(test_file1),cm_fs_filesize(test_file1));
    cm_log_printf(0,"[7]<<%s>> file exit state:%d,file size:%d\n", test_file2,cm_fs_exist(test_file2),cm_fs_filesize(test_file2));

    /* 8. 删除文件 */
    cm_fs_delete(test_file2);
    cm_log_printf(0,"[8]<<%s>> file exit state:%d,file size:%d\n", test_file2,cm_fs_exist(test_file2),cm_fs_filesize(test_file2));

    /* 9. 获取文件系统大小、剩余空间 */
    cm_fs_getinfo(&fs_info);
    cm_log_printf(0,"[9]file system total:%d,remain:%d", fs_info.total_size, fs_info.free_size);

    while(1)
    {
        osDelay(2000);
    }
}

/* opencpu程序主入口，禁止任何阻塞操作*/
int cm_opencpu_entry(void *arg)
{
    (void)arg;
    osThreadAttr_t app_task_attr = {0};
    app_task_attr.name  = "test_task";
    app_task_attr.stack_size = 1024 * 2;
    app_task_attr.priority = osPriorityNormal;

    osThreadNew((osThreadFunc_t)cm_filesystem_task_test, 0, &app_task_attr);
    return 0;
}