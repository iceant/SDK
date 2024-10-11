#include <sdk_ringbuffer.h>
#include <stdlib.h>
#include <stdio.h>
#include <sdk_macros.h>

static sdk_ringbuffer_t buf;
static uint8_t block[256];

int main(int argc, char** argv){
    sdk_ringbuffer_init(&buf, block, SDK_ARRAY_SIZE(block));
    
    for(int i=0; i< SDK_ARRAY_SIZE(block); i++){
        sdk_ringbuffer_put(&buf, i&0xFF);
    }
    
    char c;
    for(sdk_size_t i=0; i< SDK_ARRAY_SIZE(block); i++){
        sdk_ringbuffer_peek(&buf, i, &c);
        printf("[%d] %c\n", i, c);
    }
    
    sdk_size_t endptr = 0;
    unsigned long ulVal = sdk_ringbuffer_strtoul(&buf, 53, &endptr, 10);
    printf("sdk_ringbuffer_strtoul(&buf, 48, &endptr, 10) = %u, endptr = %d\n", ulVal, endptr);
    
    int pos = sdk_ringbuffer_find(&buf, 0, "5678", 4);
    printf("sdk_ringbuffer_find(&buf, 0, \"5678\", 4) = %d\n", pos);
    printf("sdk_ringbuffer_find(&buf, 54, \"5678\", 4) = %d\n", sdk_ringbuffer_find(&buf, 54, "5678", 4));
    
    sdk_ringbuffer_advance(&buf, 4);
    printf("write_idx: %d, read_idx:%d\n", buf.write_idx, buf.read_idx);
    sdk_ringbuffer_put(&buf, 0x12);
    sdk_ringbuffer_put(&buf, 0x34);
    sdk_ringbuffer_put(&buf, 0x56);
    sdk_ringbuffer_put(&buf, 0x78);
//    for(sdk_size_t i=0; i< SDK_ARRAY_SIZE(block); i++){
//        sdk_ringbuffer_peek(&buf, i, &c);
//        printf("[%d] %c\n", i, c);
//    }
    
    {
        uint16_t value = 0;
        sdk_size_t used = sdk_ringbuffer_used(&buf);
        sdk_ringbuffer_uint16_le(&buf, 251, &value);
        printf("sdk_ringbuffer_uint16_le(&buf, 251)=0x%08x\n", value);
        sdk_ringbuffer_uint16_be(&buf, 251, &value);
        printf("sdk_ringbuffer_uint16_be(&buf, 251)=0x%08x\n", value);
    }
    
    {
        uint32_t value = 0;
        sdk_ringbuffer_uint32_le(&buf, 251, &value);
        printf("sdk_ringbuffer_uint32_le(&buf, 251)=0x%08x\n", value);
        sdk_ringbuffer_uint32_be(&buf, 251, &value);
        printf("sdk_ringbuffer_uint32_be(&buf, 251)=0x%08x\n", value);
    }
    return 0;
}