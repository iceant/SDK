#include "sdk_memory.h"
#include <sdk_macros.h>
#include <sdk_printf.h>
#include <sdk_compiler.h>

#if (SDK_USE_TLSF==1)
#include <tlsf.h>

static uint8_t sdk_memory__pool[SDK_TLSF_MEMORY_POOL_SIZE];
static sdk_bool_t sdk_memory__init_flag = SDK_FALSE;
static sdk_bool_t sdk_memory__destroy_flag = SDK_FALSE;

C_CONSTRUCTOR
void sdk_memory_init(void){
    if(sdk_memory__init_flag){
        return;
    }
    sdk_memory__init_flag = SDK_TRUE;
    size_t size = init_memory_pool(SDK_ARRAY_SIZE(sdk_memory__pool), sdk_memory__pool);
    #if defined(SDK_MEMORY_DEBUG)
    sdk_printf("[sdk_memory] %"SDK_PRIu"/%"SDK_PRIu"(used %"SDK_PRIu")\n", size, SDK_ARRAY_SIZE(sdk_memory__pool)
               , SDK_ARRAY_SIZE(sdk_memory__pool)-size);
    #endif
}

C_DESTRUCTOR
void sdk_memory_destroy(void){
    if(sdk_memory__destroy_flag){
        return;
    }
    sdk_memory__destroy_flag = SDK_TRUE;
    #if defined(SDK_MEMORY_DEBUG)
    sdk_printf("[sdk_memory] destroy memory pool!\n");
    #endif
    destroy_memory_pool(sdk_memory__pool);
}

void* sdk_memory_alloc(sdk_size_t nBytes){
    return tlsf_malloc(nBytes);
}

void* sdk_memory_calloc(sdk_size_t nCount, sdk_size_t nBytes){
    return tlsf_calloc(nCount, nBytes);
}

void* sdk_memory_realloc(void* memory, sdk_size_t nBytes){
    return tlsf_realloc(memory, nBytes);
}

void sdk_memory_free(void* memory){
    if(memory){
        tlsf_free(memory);
    }
}

#endif

#if (SDK_USE_TLSF!=1)
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
#endif