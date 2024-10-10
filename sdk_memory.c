#include "sdk_memory.h"
#include <stdlib.h>


void* sdk_memory_alloc(sdk_size_t nBytes){
    return malloc(nBytes);
}

void* sdk_memory_calloc(sdk_size_t nCount, sdk_size_t nBytes){
    return calloc(nCount, nBytes);
}

void* sdk_memory_realloc(void* memory, sdk_size_t nBytes){
    return realloc(memory, nBytes);
}

void sdk_memory_free(void* memory){
    if(memory){
        free(memory);
    }
}
