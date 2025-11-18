#ifndef __OTA_HTTP_H__
#define __OTA_HTTP_H__

#include <curl/curl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "log.h"

#define OTA_FILEINFO_URL "http://192.168.42.38:8000/fileinfo.json"
#define OTA_DOWNLOAD_URL "http://192.168.42.38:8000/download/gateway"
#define OTA_LOCAL_FILE "/home/wuhan/gateway.update"

/**
 * @brief 请求获取json数据
 * 
 * @param url 请求地址
 * @return char* json数据
 */
char *ota_http_getJson(char *url);

/**
 * @brief 请求下载文件
 * 
 * @param url 请求地址
 * @param filepath 文件保存路径
 * @return int 0:成功 -1:失败
 */
int ota_http_download(char *url, char *filepath);

#endif /* __OTA_HTTP_H__ */
