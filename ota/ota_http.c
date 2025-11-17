#include "ota_http.h"

/**
 * @brief 接收回调函数
 *
 * @param ptr 响应数据指针
 * @param size 每个小块的数据长度
 * @param nmemb 块数量
 * @param steam 用户传入的容器指针
 * @return size_t
 */
static size_t receive_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
    char *res_data = (char *)ptr;
    char *json_buff = (char *)stream;
    int len = size * nmemb;
    memcpy(json_buff, res_data, size * nmemb);
    json_buff[len] = '\0';
    return len;
}
char *ota_http_getJson(char *url)
{
    // 创建一个CURL句柄
    CURL *curl = curl_easy_init();
    // 配置1：设置URL
    curl_easy_setopt(curl, CURLOPT_URL, url);
    // 配置2：设置接收响应的回调函数
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, receive_callback);
    // 配置3：设置接收响应的回调函数的参数
    char *json_buff = (char *)malloc(100);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, json_buff);

    // 执行请求
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        log_error("请求失败(%s),原因: %s", url, curl_easy_strerror(res));
        // 释放资源
        curl_easy_cleanup(curl);
        free(json_buff);
        return NULL;
    }
    // 释放资源
    curl_easy_cleanup(curl);

    return json_buff;
}

int ota_http_download(char *url, char *filepath)
{
    // 创建一个CURL句柄
    CURL *curl = curl_easy_init();
    // 配置1：设置URL
    curl_easy_setopt(curl, CURLOPT_URL, url);
    // 配置2：设置接收响应的回调函数
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fwrite);
    // 配置3：设置接收响应的回调函数的参数
    FILE *fp = fopen(filepath, "wb");
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

    // 执行请求
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        log_error("请求失败(%s),原因: %s", url, curl_easy_strerror(res));
        // 释放资源
        curl_easy_cleanup(curl);
        fclose(fp);
        return -1;
    }
    // 释放资源
    curl_easy_cleanup(curl);
    fclose(fp);
    return 0;
}
