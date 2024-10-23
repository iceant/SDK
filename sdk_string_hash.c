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

static uint64_t sdk_hash_fmix64(uint64_t u){
    uint64_t v = u * 3935559000370003845LLU + 2691343689449507681LLU;
    
    v ^= v >> 21;
    v ^= v << 37;
    v ^= v >>  4;
    
    v *= 4768777513237032717LLU;
    
    v ^= v << 20;
    v ^= v >> 41;
    v ^= v <<  5;
    
    return v;
}

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

// Return 64-bit FNV-1a hash for key (NUL-terminated). See description:
// https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
static uint64_t sdk_hash_fnv_key(const char* key) {
    uint64_t hash = FNV_OFFSET;
    for (const char* p = key; *p; p++) {
        hash ^= (uint64_t)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }
    return hash;
}

const char* sdk_hash_int_to_str(sdk_int_t n, char* buf, sdk_size_t buf_size){
    char str[43];
    char *s = str + sizeof(str);
    sdk_uint_t m;
    if(n==SDK_INT_MIN){
        m = SDK_INT_MAX + 1UL;
    }else if(n<0){
        m = -n;
    }else{
        m = n;
    }
    
    do{
        *--s = m%10 + '0';
    }while((m/=10)>0);
    if(n<0){
        *--s = '-';
    }
    sdk_size_t size = (str + sizeof(str))-s;
    size = (size > buf_size)?buf_size:size;
    memcpy(buf, s, size);
    buf[size]='\0';
    return buf;
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


uint32_t sdk_int_hash(const void* data, void* arg){
    char str[43];
    const char* s = sdk_hash_int_to_str((sdk_size_t)data, str, sizeof(str));
    return sdk_string_hash(s, arg);
}

void* sdk_int_hash_cp(const void* data, void* arg){
    return data;
}

sdk_bool_t sdk_int_hash_eq(const void* data1, const void* data2, void* arg){
    return ((sdk_size_t)data1==(sdk_size_t)data2)?SDK_TRUE:SDK_FALSE;
}

void sdk_int_hash_free(void* data, void* arg){

}

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

uint32_t sdk_voidp_hash(const void* data, void* arg){
    char str[43];
    const char* s = sdk_hash_int_to_str((sdk_size_t)data, str, sizeof(str));
    return sdk_string_hash(s, arg);
}

void* sdk_voidp_hash_cp(const void* data, void* arg){
    return data;
}

sdk_bool_t sdk_voidp_hash_eq(const void* data1, const void* data2, void* arg){
    return (data1==data2)?SDK_TRUE:SDK_FALSE;
}

void sdk_voidp_hash_free(void* data, void* arg){

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

/* -------------------------------------------------------------------------------------------------------------- */
/*  */


sdk_hashtable_key_ops_t sdk_int_hash_key_ops={
        .hash = sdk_int_hash,
        .cp = sdk_int_hash_cp,
        .free = sdk_int_hash_free,
        .eq = sdk_int_hash_eq,
        .arg = 0
};

sdk_hashtable_val_ops_t sdk_int_hash_val_ops={
        .cp = sdk_int_hash_cp,
        .free = sdk_int_hash_free,
        .eq = sdk_int_hash_eq,
        .arg = 0
};

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

sdk_hashtable_val_ops_t sdk_voidp_hash_val_ops={
        .cp = sdk_voidp_hash_cp,
        .free = sdk_voidp_hash_free,
        .eq = sdk_voidp_hash_eq,
        .arg = 0
};

