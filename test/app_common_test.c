#include "app_common.h"
#include "log.h"

int main(int argc, char const *argv[])
{
    long time = app_common_getCurruntTime();
    log_info("time:%ld", time);

    return 0;
}
