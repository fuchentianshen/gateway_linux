#include "daemon_runner.h"
#include "app_runner.h"
#include "ota_version.h"

int main(int argc, char const *argv[])
{
    // 如果参数小于2，提示必须输入参数
    if (argc < 2)
    {
        log_error("必须携带参数：app|ota|daemon|version");
        return -1;
    }
    else if (strcmp(argv[1], "app") == 0)
    {
        app_runner_run();
    }
    else if (strcmp(argv[1], "ota") == 0)
    {
        ota_version_checkUpdateDaily();
    }
    else if (strcmp(argv[1], "daemon") == 0)
    {
        daemon_runner_run();
    }
    else if (strcmp(argv[1], "version") == 0)
    {
        ota_version_printVersion();
    }
    else
    {
        log_error("参数错误");
        return -1;
    }

    return 0;
}
