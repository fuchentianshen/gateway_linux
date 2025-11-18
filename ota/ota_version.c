#include "ota_version.h"

/**
 * 获取文件的SHA1哈希值（40位16进制字符串）
 * 相同文件返回相同的哈希值，可用于判断文件是否相同
 * linux命令生成：sha1sum 文件名
 * 此函数可以利用AI工具帮我们生成
 */
static char *get_file_sha(char *filepath)
{
    FILE *file = fopen(filepath, "rb");
    if (!file)
    {
        perror("Failed to open file");
        return NULL;
    }

    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA_CTX sha1;
    SHA1_Init(&sha1);

    const int bufSize = 32768;
    unsigned char *buffer = (unsigned char *)malloc(bufSize);
    if (!buffer)
    {
        perror("Failed to allocate memory");
        fclose(file);
        return NULL;
    }

    int bytesRead;
    while ((bytesRead = fread(buffer, 1, bufSize, file)) > 0)
    {
        SHA1_Update(&sha1, buffer, bytesRead);
    }

    SHA1_Final(hash, &sha1);
    fclose(file);
    free(buffer);

    char *outputBuffer = (char *)malloc(SHA_DIGEST_LENGTH * 2 + 1);
    if (!outputBuffer)
    {
        perror("Failed to allocate memory");
        return NULL;
    }

    for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
    {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }

    return outputBuffer;
}

int ota_version_checkUpdate()
{
    // 获取远程的版本信息json
    char *json = ota_http_getJson(OTA_FILEINFO_URL);

    // 解析版本号
    cJSON *root = cJSON_Parse(json);
    int major = cJSON_GetObjectItem(root, "major")->valueint;
    int minor = cJSON_GetObjectItem(root, "minor")->valueint;
    int patch = cJSON_GetObjectItem(root, "patch")->valueint;
    log_debug("--远程版本：%d.%d.%d--\n", major, minor, patch);
    ota_version_printVersion();

    // 比较版本号，如果当前版本是最新，则不需要更新
    if (VERSION_MAJOR > major || (VERSION_MAJOR == major && VERSION_MINOR > minor) ||
        (VERSION_MAJOR == major && VERSION_MINOR == minor && VERSION_PATCH >= patch))
    {
        log_debug("--当前版本已是最新--\n");
        free(json);
        cJSON_Delete(root);
        return 0;
    }

    // 如果当前版本不是最新的，则下载新版本
    int ret = ota_http_download(OTA_DOWNLOAD_URL, OTA_LOCAL_FILE);
    if (ret == -1)
    {
        log_error("--下载新版本失败--\n");
        free(json);
        cJSON_Delete(root);
        return -1;
    }

    // 校验文件
    char *remote_hash = cJSON_GetObjectItem(root, "sha1")->valuestring;
    char *local_hash = get_file_sha(OTA_LOCAL_FILE);
    if (strcmp(remote_hash, local_hash) != 0)
    {
        log_error("--文件校验失败--\n");
        free(json);
        cJSON_Delete(root);
        free(local_hash);
        unlink(OTA_LOCAL_FILE); // 删除下载的文件
        return -1;
    }

    // 重启系统，运行新版本
    log_debug("重启系统，运行新版本");
    reboot(RB_AUTOBOOT); // 要求必须是root用户

    return 0;
}

void ota_version_checkUpdateDaily()
{
    while (1)
    {
        ota_version_checkUpdate();
        sleep(60 * 60 * 24);
    }
}

void ota_version_printVersion()
{
    log_debug("--当前版本：%d.%d.%d--\n", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
}
