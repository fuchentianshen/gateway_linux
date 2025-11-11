#include "cJSON.h"
#include "log.h"


int main(int argc, char const *argv[])
{
    // 生成json
    cJSON *rootObject = cJSON_CreateObject();
 
    cJSON_AddStringToObject(rootObject, "name", "fanyingjie");
    cJSON_AddNumberToObject(rootObject, "age", 18);

    char *json = cJSON_PrintUnformatted(rootObject);
    log_debug("生成json:%s", json);

    //解析json
    cJSON *rootObject2 = cJSON_Parse(json);
    char *name = cJSON_GetObjectItem(rootObject, "name")->valuestring;
    int age = cJSON_GetObjectItem(rootObject, "age")->valueint;
    log_debug("解析json: name:%s, age:%d", name, age);

    //释放json
    cJSON_Delete(rootObject);
    cJSON_Delete(rootObject2);
    cJSON_free(json);
    return 0;
}
