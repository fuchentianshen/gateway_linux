#include "log.h"

int main(int argc, char const *argv[])
{
    // 设置日志级别
    // log_set_level(LOG_DEBUG);

    // 输出日志
    log_trace("trace log...");
    log_debug("debug log...");
    log_info("info log...");
    log_warn("warn log...");
    log_error("error log...");
    log_fatal("fatal log...");

    return 0;
}
