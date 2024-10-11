#include <sdk_printf.h>
#include <sdk_types.h>
#include <stdio.h>

static char sdk_printf__buffer[SDK_PRINTF_BUFFER_SIZE];

int sdk_printf(const char* fmt, ...){
    va_list arg;
    va_start(arg, fmt);
    int size = vsnprintf(sdk_printf__buffer, SDK_TLSF_MEMORY_POOL_SIZE, fmt, arg);
    va_end(arg);
    
    if(size > SDK_PRINTF_BUFFER_SIZE){
        return 0;
    }
    printf("%s", sdk_printf__buffer);
    return size;
}

