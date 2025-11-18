#ifndef __OTA_VERSION_H__
#define __OTA_VERSION_H__

//各级版本号
#define VERSION_MAJOR 3
#define VERSION_MINOR 0
#define VERSION_PATCH 0

#include "log.h"
#include "ota_http.h"
#include <unistd.h>
#include "cJSON.h"
#include <sys/reboot.h>
#include <openssl/sha.h>

/**
 * @brief 版本检查更新
 * 
 * @return int 
 */
int ota_version_checkUpdate();

/**
 * @brief 每日检查更新
 * 
 */
void ota_version_checkUpdateDaily();


/**
 * @brief 打印版本号
 * 
 */
void ota_version_printVersion();

#endif /* __OTA_VERSION_H__ */
