#include <sdk_string_hash.h>
#include <string.h>
#include <sdk_memory.h>

/* -------------------------------------------------------------------------------------------------------------- */
/*  */


static uint32_t sdk_hash_fmix32(uint32_t h){
    h ^= h >> 16;
    h *= 0x3243f6a9U;
    h ^= h >> 16;
    return h;
}

/* -------------------------------------------------------------------------------------------------------------- */
/*  */


uint32_t sdk_string_hash(const void* data, void* arg){
    uint32_t hash = (const uint32_t) 5381;
    const char* str = (const char*)data;
    char c;
    while((c=*str++)){
        hash = ((hash << 5) + hash) + c;
    }
    return sdk_hash_fmix32(hash);
}

void* sdk_string_hash_cp(const void* data, void* arg){
    const char* input = (const char* )data;
    sdk_size_t input_size = strlen(input) + 1;
    char* result;
    
    result = SDK_ALLOC(sizeof(*result) * input_size);
    if(!result){
        return 0;
    }
    strncpy(result, input, input_size);
    result[input_size]='\0';
    return result;
}

sdk_bool_t sdk_string_hash_eq(const void* data1, const void* data2, void* arg){
    const char* str1 = (const char*)data1;
    const char* str2 = (const char*)data2;
    return !(strncmp(str1, str2, strlen(str1)))?SDK_TRUE:SDK_FALSE;
}

void sdk_string_hash_free(void* data, void* arg){
    SDK_FREE(data);
}

/* -------------------------------------------------------------------------------------------------------------- */
/*  */


sdk_hashtable_key_ops_t sdk_string_hash_key_ops={
        .hash = sdk_string_hash,
        .cp = sdk_string_hash_cp,
        .free = sdk_string_hash_free,
        .eq = sdk_string_hash_eq,
        .arg = 0
};

sdk_hashtable_val_ops_t sdk_string_hash_val_ops={
    .cp = sdk_string_hash_cp,
    .free = sdk_string_hash_free,
    .eq = sdk_string_hash_eq,
    .arg = 0
};

