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
