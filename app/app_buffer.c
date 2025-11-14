#include "app_buffer.h"

// 初始化一个sub_buffer
static SubBuffer *init_sub_buffer(int total_size)
{
    // 申请空间
    SubBuffer *sub_buffer = (SubBuffer *)malloc(sizeof(SubBuffer));
    sub_buffer->ptr = (char *)malloc(total_size);
    sub_buffer->total_size = total_size;
    sub_buffer->len = 0;
    return sub_buffer;
}

Buffer *app_buffer_init(int total_size)
{
    // 申请buffer的内存
    Buffer *buffer = (Buffer *)malloc(sizeof(Buffer));

    // 申请sub_buffer的内存,并初始化
    buffer->sub_buffer[0] = init_sub_buffer(total_size);
    buffer->sub_buffer[1] = init_sub_buffer(total_size);
    buffer->read_index = 0;
    buffer->write_index = 1;
    return buffer;
}

void app_buffer_destroy(Buffer *buffer)
{
    free(buffer->sub_buffer[0]->ptr);
    free(buffer->sub_buffer[1]->ptr);
    free(buffer->sub_buffer[0]);
    free(buffer->sub_buffer[1]);
    free(buffer);
}

int app_buffer_write(Buffer *buffer, char *data, int len)
{
    // len的长度不能大于255，如果大于255，就写不到一个字节中
    if (len > 255)
    {
        log_error("数据长度大于255,无法写入");
        return -1;
    }

    // 找到写缓冲区
    SubBuffer *w_buffer = buffer->sub_buffer[buffer->write_index];
    // 写入一个字节的数据长度，再写入数据本身
    w_buffer->ptr[w_buffer->len++] = len;
    // 判断缓冲区剩余空间是否足够
    if (w_buffer->len + len > w_buffer->total_size)
    {
        log_error("缓冲区剩余空间不足,写入失败");
        return -1;
    }
    memcpy(w_buffer->ptr + w_buffer->len, data, len);

    // 更新len
    w_buffer->len += len;
    return 0;
}

int app_buffer_read(Buffer *buffer, char *data_buff, int buff_size)
{
    // 找到读缓冲区
    SubBuffer *r_buffer = buffer->sub_buffer[buffer->read_index];

    // 如果缓冲区为空，切换缓冲区
    if (r_buffer->len == 0)
    {
        buffer->read_index = (buffer->read_index + 1) % 2;
        buffer->write_index = (buffer->write_index + 1) % 2;
        r_buffer = buffer->sub_buffer[buffer->read_index];
        // 如果缓冲区再为空，则返回-1
        if (r_buffer->len == 0)
        {
            log_error("缓冲区为空，读取失败");
            return -1;
        }
    }

    // 读取一个字节数据：要读取的数据的长度,然后读取数据本身
    int len = r_buffer->ptr[0];

    // 判断data_buff的长度是否足够
    if (len > buff_size)
    {
        log_error("数据长度超出data_buff大小，读取失败");
        return -1;
    }

    memcpy(data_buff, r_buffer->ptr + 1, len);

    // 将后面的数据左移到起始位置
    memmove(r_buffer->ptr, r_buffer->ptr + len + 1, r_buffer->len - len - 1);
    // 更新len
    r_buffer->len -= len + 1;
    return 0;
}
