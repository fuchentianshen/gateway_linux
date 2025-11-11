#include "app_common.h"
#include "sys/time.h"
#include "stdlib.h"
long app_common_getCurruntTime(void)
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    
    return tv.tv_sec*1000+tv.tv_usec/1000;
}