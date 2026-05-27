
#include "custom_fota.h"
#include "custom_system.h"
#include "custom_track.h"
#include "custom_bms_ota.h"
#include "custom_global.h"

#define FOTA_OTA_WRITE_CHUNK_SIZE 1024

custom_fota_t	fota;

// 传入HTTP链接，解析出主机地址和路径
// 返回：0:合法;  -1:非法,存在格式错误;  -2:host_size或path_size过小
int custom_fota_check_httpurl(char *http_url, 
									char *http_host, uint32_t host_size, 
									char *http_path, uint32_t path_size)
{
	char http_host_temp[256] = {0};		// 主机地址，http://121.199.36.16
	char http_path_temp[256] = {0};		// 路径地址，/TUpdate/system_patch.bin
	char *offset = NULL;
	int port = 0;
	
	// 完整URL-(格式：http://121.199.36.16/TUpdate/system_patch.bin / https://121.199.36.16/TUpdate/system_patch.bin)
	if((strncmp(http_url, "http://", 7) == 0) || (strncmp(http_url, "https://", 8) == 0))
	{
		uint32_t prefix_len = (strncmp(http_url, "https://", 8) == 0) ? 8 : 7;
		offset = strchr(http_url + prefix_len, '/');
		if(offset == NULL)
		{
			FOTA_printf("%s: format error!", __func__);
			return -1;
		}
		strcpy(http_path_temp, offset);												// "/TUpdate/system_patch.bin"
		strncpy(http_host_temp, http_url, strlen(http_url) - strlen(offset));		// "http://121.199.36.16"
	}
	// 无头URL-(格式：121.199.36.16/TUpdate/system_patch.bin)
	else
	{
		offset = strchr(http_url, '/');
		if(offset == NULL)
		{
			FOTA_printf("%s: format error!", __func__);
			return -1;
		}
		strcpy(http_path_temp, offset);												// "/TUpdate/system_patch.bin"
		strcpy(http_host_temp, "http://");											
		strncpy(http_host_temp + 7, http_url, strlen(http_url) - strlen(offset));	// "http://121.199.36.16"
	}

	// 添加完整端口号
	if(strncmp(http_host_temp, "https://", 8) == 0)
	{
		if(sscanf(http_host_temp, "https://%*[^:]:%d", &port) != 1)					// 无端口号，添加默认443
		{
			strcat(http_host_temp, ":443");
		}
	}
	else
	{
		if(sscanf(http_host_temp, "http://%*[^:]:%d", &port) != 1) 					// 无端口号，添加默认80
		{
			strcat(http_host_temp, ":80");
		}
	}

	// 检测输入缓冲大小
	if((host_size < strlen(http_host_temp)) || (path_size < strlen(http_path_temp)))
	{
		FOTA_printf("%s: size error!", __func__);
		return -2;
	}
	strncpy(http_host, http_host_temp, strlen(http_host_temp));
	strncpy(http_path, http_path_temp, strlen(http_path_temp));
	
	//FOTA_printf("host addr:%s,url:%s", http_host, http_path);

	return 0;
}

// 获取http文件片段
// 传入升级连接,data存放下载数据的buffer,buffer大小,字节开始,字节结束,isfull(是否全量下载 0-非全量下载 1-全量下载),filelen 整个文件的大小,datalen 下载成功的字节数
// 返回：0 下载成功，，-1 URL错误，-2 HTTP请求错误，-3 datasize过小，-4 range_start range_end参数错误
/*  使用HTTP GET方法下载文件，需服务器支持Range分包下载，即断点续传功能，可控制下载文件的始末
 *  三种下载方式：
 *  （1）一次性下载完成，设置isfull为1，range_start和range_end的值会被忽略（需注意datasize大小是否满足）
 *  （2）获取文件包大小，设置isfull为0, range_start和range_end为0
 *  （3）部分下载，设置isfull为0，同时设置range_start和range_end */
int custom_fota_httpfile_get_partial(char *http_url, 
									uint8_t *data, uint32_t datasize, 
									uint32_t range_start, uint32_t range_end, 
									bool isfull, uint32_t *filelen, uint32_t *datalen)
{
	char host_addr[256] = {0}, file_url[256] = {0};

	// 解析URL
	if(custom_fota_check_httpurl(http_url, host_addr, sizeof(host_addr), file_url, sizeof(file_url)) != 0)
	{
		FOTA_printf("%s: error!", __func__);
		return -2;
	}

	// 创建httpclient
	cm_httpclient_handle_t http_client = NULL;
	cm_httpclient_ret_code_e ret = CM_HTTP_RET_CODE_UNKNOWN_ERROR;
	
	ret = cm_httpclient_create((const uint8_t*)host_addr, NULL, &http_client);
	if(CM_HTTP_RET_CODE_OK != ret || NULL == http_client)
	{
		FOTA_printf("cm_httpclient_create error!");
		return -3;
	}

	// 配置httpclient
	cm_httpclient_cfg_t client_cfg;
	client_cfg.ssl_enable = (strncmp(http_url, "https://", 8) == 0) ? 1 : 0;
	client_cfg.ssl_id = client_cfg.ssl_enable ? 1 : 0;
	client_cfg.cid = 0;
	client_cfg.conn_timeout = HTTPCLIENT_CONNECT_TIMEOUT_DEFAULT;
	client_cfg.rsp_timeout = HTTPCLIENT_WAITRSP_TIMEOUT_DEFAULT;
	client_cfg.dns_priority = 1;
	ret = cm_httpclient_set_cfg(http_client, client_cfg);
	if(ret != CM_HTTP_RET_CODE_OK)
	{
		FOTA_printf("cm_httpclient_set_cfg error!");
		cm_httpclient_delete(http_client);
		return -4;
	}

	// http下载
	cm_httpclient_sync_response_t response = {0};
	cm_httpclient_sync_param_t param = {HTTPCLIENT_REQUEST_GET, (const uint8_t*)file_url, 0, NULL};
	int file_size = 0, http_ret = 0;
	char http_header[64] = {0};
	char *file_size_offset = NULL;

	// 全量下载
	if(isfull == 1)
	{
		// 若有报头设置 可使用该接口设置报头（此处为空）
		memset(http_header, 0, sizeof(http_header));
		cm_httpclient_custom_header_set(http_client, (uint8_t *)http_header, strlen(http_header)); 

		// 发送http请求，并返回结果。
		ret = cm_httpclient_sync_request(http_client, param, &response);
		if(CM_HTTP_RET_CODE_OK != ret || (response.response_code != 200 && response.response_code != 206)) // ret不等于OK 或者code不等于200也不等于206
		{
			osDelay(ONE_SECONED);
			
			// 重新下载
			ret = cm_httpclient_sync_request(http_client, param, &response);                                    
			if(CM_HTTP_RET_CODE_OK != ret || (response.response_code != 200 && response.response_code != 206))
			{
				http_ret = -2;
				FOTA_printf("httpfile download full mode error!");
				goto EXIT;
			}
		}

		// 缓冲区大小不够
		if(datasize < response.response_content_len)
		{
			http_ret = -3;
			FOTA_printf("httpfile download full mode datasize not enough! at least:%d", response.response_content_len);
			goto EXIT;
		}

		// 返回数据与长度
		memcpy(data, response.response_content, response.response_content_len); // 拷贝数据
		*datalen = response.response_content_len;                               // 拷贝数据长度
		*filelen = response.response_content_len;                               // 拷贝数据长度

		http_ret = 0;
		FOTA_printf("httpfile download full mode success!");
		FOTA_printf("httpfile download full mode response_code:%d,response_content_len:%d,reponse_header_len:%d",
		         response.response_code, response.response_content_len, response.response_header_len);
		goto EXIT;
	}
	// 只获取文件包大小
	else if(isfull == 0 && range_start == 0 && range_end == 0)
	{
		// 设置报头
		memset(http_header, 0, sizeof(http_header));
		strcpy(http_header, "Range:bytes=0-9\r\n");     						// 使用Range尝试下载10个字节 以获取文件大小
		cm_httpclient_custom_header_set(http_client, (uint8_t *)http_header, strlen(http_header)); 

		// 发送http请求，并返回结果。
		ret = cm_httpclient_sync_request(http_client, param, &response);
		if(CM_HTTP_RET_CODE_OK != ret || (response.response_code != 200 && response.response_code != 206)) // ret不等于OK 或者code不等于200也不等于206
		{
			osDelay(ONE_SECONED);
			
			// 重新下载
			ret = cm_httpclient_sync_request(http_client, param, &response);                                    
			if(CM_HTTP_RET_CODE_OK != ret || (response.response_code != 200 && response.response_code != 206))
			{
				http_ret = -2;
				FOTA_printf("httpfile download full mode error!");
				goto EXIT;
			}
		}
		
		// 取出报头中的文件包大小
file_size_offset = strstr((char *)response.response_header, (char *)"Content-Range");
if(file_size_offset != NULL)
{
    if(sscanf(file_size_offset, "%*[^/]/%d", &file_size) != 1 || file_size <= 0)
    {
        FOTA_printf("http header error:%d,%s", response.response_header_len, response.response_header);
        http_ret = -2;
        goto EXIT;
    }
}
else
{
    file_size_offset = strstr((char *)response.response_header, (char *)"Content-Length");
    if(file_size_offset == NULL || sscanf(file_size_offset, "%*[^:]: %d", &file_size) != 1 || file_size <= 0)
    {
        FOTA_printf("http header error:%d,%s", response.response_header_len, response.response_header);
        http_ret = -2;
        goto EXIT;
    }

    FOTA_printf("httpfile get filesize by Content-Length:%d", file_size);
}

		
		// 返回数据长度
		*filelen = file_size; 													// 拷贝数据长度
		
		http_ret = 0;
		FOTA_printf("httpfile download get filesize:%d", *filelen);
		goto EXIT;
    }
	// 分片段下载（按照range_start和range_end 部分下载）
	else if(isfull == 0 && (range_end - range_start > 0))
	{
		// 设置报头
		memset(http_header, 0, sizeof(http_header));
		sprintf(http_header, "Range:bytes=%d-%d\r\n", (int)range_start, (int)range_end);          // 使用Range配置下载文件始末
		cm_httpclient_custom_header_set(http_client, (uint8_t *)http_header, strlen(http_header)); 

		// 发送http请求，并返回结果。
		ret = cm_httpclient_sync_request(http_client, param, &response);
		if(CM_HTTP_RET_CODE_OK != ret || (response.response_code != 200 && response.response_code != 206)) // ret不等于OK 或者code不等于200也不等于206
		{
			osDelay(ONE_SECONED);
			
			// 重新下载
			ret = cm_httpclient_sync_request(http_client, param, &response);                                    
			if(CM_HTTP_RET_CODE_OK != ret || (response.response_code != 200 && response.response_code != 206))
			{
				http_ret = -2;
				FOTA_printf("httpfile download full mode error!");
				goto EXIT;
			}
		}

		// 取出报头中的文件包大小
		file_size_offset = strstr((char *)response.response_header, (char *)"Content-Range");
		if(file_size_offset == NULL)
		{
			FOTA_printf("http header error:%d,%s", response.response_header_len, response.response_header);
			http_ret = -2;
			goto EXIT;
		}
		
		if(sscanf(file_size_offset, "%*[^/]/%d", &file_size) != 1 || file_size <= 0)			// 正则匹配 提取文件大小
		{
			FOTA_printf("http header error:%d,%s", response.response_header_len, response.response_header);
			http_ret = -2;
			goto EXIT;
		}

		// 返回数据与长度
		memcpy(data, response.response_content, response.response_content_len); // 拷贝数据
		*datalen = response.response_content_len;                               // 拷贝数据长度
		*filelen = file_size;                                                   // 拷贝数据长度
		http_ret = 0;
		
		//FOTA_printf("httpfile download partial mode success!");
		//FOTA_printf("httpfile download partial mode response_code:%d,response_content_len:%d,reponse_header_len:%d",
		//				response.response_code, response.response_content_len, response.response_header_len);
	}
	else
	{
		FOTA_printf("httpfile download param error!");
		http_ret = -4;
		goto EXIT;
	}

EXIT:
	cm_httpclient_custom_header_free(http_client);
	cm_httpclient_sync_free_data(http_client);
	cm_httpclient_terminate(http_client);
	cm_httpclient_delete(http_client);
	
	if(http_ret < 0)
	{
		memset(data, 0, datasize);
		*datalen = 0;
	}
	
	return http_ret;
}

int custom_fota_httpfile_download(void)
{
	uint32_t datasize = 0;
	
	// 获取升级文件大小
	if(0 != custom_fota_httpfile_get_partial(fota.url, NULL, 0, 0, 0, 0, &fota.filesize, &datasize)) 
	{
		FOTA_printf("%s: getfile length error!", __func__);
		return -1;
	}
	FOTA_printf("%s: filesize=%d",__func__ , fota.filesize);

	// 获取文件系统与heap空间大小
	cm_fs_getinfo(&fs_system_info);
	cm_mem_get_heap_stats(&heap_stats);
	FOTA_printf("%s: heapsize=%d,fssize=%d",__func__ , heap_stats.free, fs_system_info.free_size);

	// 下载到内存
	if(fota.save_mode == FOTA_SAVE_MEM)
	{
		if((fota.filesize + FOTA_RESERVED_SPACE_SIZE) < heap_stats.free)			// 存放：内存，并预留部分空间
		{
			// 获取内存
			fota.file = cm_malloc(fota.filesize);									// 申请内存：存放文件
			if(fota.file == NULL)
			{
				FOTA_printf("%s: cm_malloc failed!",__func__);
				return -2;
			}

			int ret = custom_fota_httpfile_get_partial(fota.url, (uint8_t *)fota.file, fota.filesize, 0, 0, 1, &fota.filesize, &datasize);	// 下载整个文件到内存
			if(ret != 0)
			{
				FOTA_printf("%s: custom_fota_httpfile_get_partial() error!", __func__);
				cm_free(fota.file);
				return -3;
			}
			FOTA_printf("%s: finish! datasize=%d,filesize=%d", __func__, datasize, fota.filesize);	
			//cm_free(fota.file);													// 内存不释放
		}
		else
		{
			FOTA_printf("%s: heap space not enough!",__func__);
			return -4;
		}
	}
	// 下载到文件系统
	else if(fota.save_mode == FOTA_SAVE_FS)
	{
		if((fota.filesize + FOTA_RESERVED_SPACE_SIZE) < fs_system_info.free_size)	// 存放：文件系统，并预留部分空间
		{
			// 获取内存
			if(fota.modules == FOTA_MODULE_LTE)
			{
				fota.file = cm_malloc(strlen(LTE_FOTA_FILE_SAVE_DIR));				// 申请内存：存放文件名
				if(fota.file == NULL)
				{
					FOTA_printf("%s: cm_malloc failed!",__func__);
					return -5;
				}
				strcpy(fota.file, LTE_FOTA_FILE_SAVE_DIR);
			}
			else
			{
				fota.file = cm_malloc(strlen(BMS_FOTA_FILE_SAVE_DIR));	
				if(fota.file == NULL)
				{
					FOTA_printf("%s: cm_malloc failed!",__func__);
					return -6;
				}
				strcpy(fota.file, BMS_FOTA_FILE_SAVE_DIR);
			}

			// 当前服务端仅支持整包下载，文件系统模式也改为先整包下载到内存再落盘。
			if((fota.filesize + FOTA_RESERVED_SPACE_SIZE) >= heap_stats.free)
			{
				FOTA_printf("%s: heap space not enough for full download!",__func__);
				return -7;
			}

			uint8_t *data = cm_malloc(fota.filesize);
			if(data == NULL)
			{
				FOTA_printf("%s: cm_malloc failed!",__func__);
				return -7;
			}

			// 打开文件
			if(cm_fs_exist(fota.file) == true)										// 存在，则删除。
			{
				cm_fs_delete(fota.file);
			}

			// 写入文件
			int32_t fd = cm_fs_open(fota.file, CM_FS_WB);							// 只写方式打开文件
			if(fd < 0)
			{
				cm_free(data);
				FOTA_printf("%s:cm_fs_open() error,fd=%d",__func__, fd);
				return -8;
			}  

			int ret = custom_fota_httpfile_get_partial(fota.url, data, fota.filesize, 0, 0, 1, &fota.filesize, &datasize);
			if(ret != 0)
			{
				cm_free(data);
				cm_fs_close(fd);
				FOTA_printf("%s: custom_fota_httpfile_get_partial() error!", __func__);
				return -9;
			}

			// 下载成功,写入文件
			int32_t f_wlen = cm_fs_write(fd, data, datasize);
			if(f_wlen != datasize)													// 写入失败
			{
				cm_free(data);
				cm_fs_close(fd);
				cJSON_printf("%s:cm_fs_write() error,f_wlen=%d,datasize=%d",__func__, f_wlen, datasize);
				return -10;
			}

			cm_free(data);
			cm_fs_close(fd);
			FOTA_printf("%s: finish! datasize=%d,filesize=%d", __func__, datasize, fota.filesize);
			return 0;
		}
		else
		{
			FOTA_printf("%s: filesystem space not enough!",__func__);
			return -11;
		}
	}
	else
	{
		FOTA_printf("%s: error!", __func__);
		return -12;
	}

	return 0;
}

/**
 *  @brief FOTA-全系统差分升级/APP整包升级
 *  @param [in] ota_url 传入升级连接
 *  @return
 *   0 下载成功 触发升级成功
 *  -1 OTA初始化失败
 *  -2 获取升级包大小失败 \n
 *  -3 动态内存不足 申请内存失败 \n
 *  -4 下载失败 并清除已写入升级空间的数据 \n
 *  -5 写入数据失败                      \n
 *  -6 触发升级失败，可能升级包有误       \n
 *
 *  @details 直接传入升级链接,若打开APP_FOTA_CHECK会对链接的格式做部分校验，且会先尝试下载10个字节，看升级包是否存在且可下载
 *  若确认每次传入链接是无误的，可无需检查，注意host_addr与file_url大小为256,可根据实际情况调整
 */
int custom_fota_lte_ota_start(uint8_t save_mode, char *file, uint32_t file_size)
{
	int ret = -1;	
	
	// OTA初始化
	ret = cm_ota_init(); 
	if(ret < 0)
	{
		FOTA_printf("cm_ota_init fail: %d!", ret);
		return -1;
	}
	FOTA_printf("cm_ota_init() ok!");

	// 擦除固件区
	ret = cm_ota_firmware_erase(); 
	if(ret < 0)
	{
		FOTA_printf("cm_ota_firmware_erase fail: %d!", ret);
		return -2;
	}
	FOTA_printf("cm_ota_firmware_erase() ok!");

	// 设置升级包大小
	cm_ota_set_otasize(file_size);
	FOTA_printf("cm_ota_set_otasize() ok!");

	// 写入升级数据
	if(save_mode == FOTA_SAVE_MEM)
	{
		uint32_t write_offset = 0;
		uint32_t write_len = 0;

		FOTA_printf("%s: save_mode=mem,file_size=%d", __func__, file_size);

		while(write_offset < file_size)
		{
			write_len = ((file_size - write_offset) > FOTA_OTA_WRITE_CHUNK_SIZE) ? FOTA_OTA_WRITE_CHUNK_SIZE : (file_size - write_offset);

			ret = cm_ota_firmware_write(file + write_offset, write_len);
			if(ret < 0)
			{
				FOTA_printf("cm_ota_firmware_write fail: %d! offset=%d,len=%d", ret, write_offset, write_len);
				ret = cm_ota_firmware_write(file + write_offset, write_len);
				if(ret < 0)
				{
					FOTA_printf("cm_ota_firmware_write retry fail: %d! offset=%d,len=%d", ret, write_offset, write_len);
					return -3;
				}
			}

			write_offset += write_len;
			FOTA_printf("cm_ota_firmware_write ok: offset=%d/%d", write_offset, file_size);
		}

		FOTA_printf("cm_ota_firmware_write() ok!");
	}
	else if(save_mode == FOTA_SAVE_FS)
	{
		FOTA_printf("%s: save_mode=fs,file_name=%s,file_size=%d", __func__, file, file_size);
		
		//-xxx
		//读文件&写入升级区		
	}
	
	// 启动升级
	ret = cm_ota_upgrade();
	if(ret < 0)
	{
		FOTA_printf("cm_ota_upgrade failed: %d!", ret);
		return -4;
	}
	FOTA_printf("cm_ota_upgrade() ok!");
	
	return 0;
}

// 启动FOTA升级线程(升级指定模块，升级文件URL，升级文件保存位置，是否立即升级)
int custom_fota_start(uint8_t fota_module, char *fota_url, uint8_t save_mode, uint8_t is_update)
{
	if((fota_module < FOTA_MODULE_MAX) && (strlen(fota_url) > 0))
	{
		memset(&fota, 0, sizeof(fota));
		fota.modules = fota_module;
		fota.save_mode = save_mode;
		fota.is_update = is_update;
		fota.url = cm_malloc(strlen(fota_url)+1);
		
		if(fota.url != NULL)
		{
			strcpy(fota.url, fota_url);
			fota.state = FOTA_STATE_UPGRADE;
			FOTA_printf("%s: module=%d,url=%s", __func__, fota.modules, fota.url);
			return 0;
		}
	}	
	FOTA_printf("%s: error!", __func__);
	
	return -1;
}

int custom_fota_finish(void)
{
	if(fota.url != NULL)
	{
		cm_free(fota.url);
	}	
	if(fota.file != NULL)
	{
		cm_free(fota.file);
	}
	FOTA_printf("%s!", __func__);

	return 0;
}

static void custom_fota_task(void *argument)
{
	memset(&fota, 0, sizeof(fota));

	while (1)
	{
		osDelay(ONE_SECONED / 10);

		// 检测是否有升级任务
		if(fota.state == FOTA_STATE_UPGRADE)
		{
			// 下载升级文件
			if(custom_fota_httpfile_download() == 0)
			{
				FOTA_printf("custom_fota_httpfile_download() success.");

				// 启动升级流程
				if(fota.modules == FOTA_MODULE_LTE) 			// LTE模块
				{
					FOTA_printf("FOTA_MODULE_LTE: UPDATE-%s", (fota.is_update == FOTA_UPDATE_YES)? "YES":"NO");	

					if(fota.is_update == FOTA_UPDATE_YES)
					{
						int ret = -1;

						ret = custom_fota_lte_ota_start(fota.save_mode, fota.file, fota.filesize);
						if(ret == 0)
						{
							FOTA_printf("custom_fota_lte_ota_start() success, waiting reboot...");
							osDelay(2000 / 5);
							FOTA_printf("cm_pm_reboot().");
							cm_pm_reboot();
						}
						else
						{
							FOTA_printf("custom_fota_lte_ota_start() fail: %d", ret);
						}
					}
				}
				else if(fota.modules == FOTA_MODULE_BMS)		// BMS模块
				{
					
					FOTA_printf("FOTA_MODULE_BMS: UPDATE-%s", (fota.is_update == FOTA_UPDATE_YES)? "YES":"NO");

					if(fota.is_update == FOTA_UPDATE_YES)
					{
						custom_bms_ota_start(fota.save_mode, fota.file, fota.filesize);
					}
				}
			}
			else
			{
				FOTA_printf("custom_fota_httpfile_download() error.");				
			}

			// 升级任务结束
			custom_fota_finish();
			fota.state = FOTA_STATE_IDLE;
		}
	}
}

int custom_fota_init(void)
{
	osThreadAttr_t fota_task_attr = {0};

	//FOTA_printf("LTE_DEFAULT_FOTA_URL: %s", LTE_DEFAULT_FOTA_URL);
	
	fota_task_attr.name = "fota_task";
	fota_task_attr.stack_size = 8 * 1024;
	fota_task_attr.priority = osPriorityNormal;
	osThreadNew((osThreadFunc_t)custom_fota_task, 0, &fota_task_attr);

	return 0;
}

