#include "app_buffer.h"

int main(int argc, char const *argv[])
{
    // 初始化缓冲区
    Buffer *buffer = app_buffer_init(15);
    // 向缓冲区写入数据
    app_buffer_write(buffer, "hello", 5);
    app_buffer_write(buffer, "world", 5);

    // 从缓冲区读取数据
    char buff[10];
    app_buffer_read(buffer, buff, 10);
    log_debug("读取到的数据%s", buff);
    char buff2[10];
    app_buffer_read(buffer, buff2, 10);
    log_debug("读取到的数据%s", buff2);

    // 销毁缓冲区
    app_buffer_destroy(buffer);
    return 0;
}
