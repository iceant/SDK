#ifndef INCLUDED_SDK_HASHTABLE_H
#define INCLUDED_SDK_HASHTABLE_H

#ifndef INCLUDED_SDK_TYPES_H
#include <sdk_types.h>
#endif /*INCLUDED_SDK_TYPES_H*/

#ifndef INCLUDED_SDK_VECTOR_H
#include <sdk_vector.h>
#endif /*INCLUDED_SDK_VECTOR_H*/

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

typedef struct sdk_hashtable_key_ops_s{
    uint32_t (*hash)(const void* data, void* arg);
    void* (*cp)(const void* data, void* arg);
    void (*free)(void* data, void* arg);
    sdk_bool_t (*eq)(const void* data1, const void* data2, void* arg);
    void* arg;
}sdk_hashtable_key_ops_t;

typedef struct sdk_hashtable_val_ops_s{
    void* (*cp)(const void* data, void* arg);
    void (*free)(void* data, void* arg);
    sdk_bool_t (*eq)(const void* data1, const void* data2, void* arg);
    void* arg;
}sdk_hashtable_val_ops_t;

typedef struct sdk_hashtable_item_s{
    uint32_t hash;
    void* key;
    void* val;
}sdk_hashtable_item_t;

typedef struct sdk_hashtable_s{
    sdk_vector_t ** buckets;
    sdk_size_t capacity;
    sdk_size_t size;
    sdk_hashtable_key_ops_t key_ops;
    sdk_hashtable_val_ops_t val_ops;
}sdk_hashtable_t;

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

sdk_err_t sdk_hashtable_init(sdk_hashtable_t* t, sdk_size_t init_buckets
                             , sdk_hashtable_key_ops_t k_ops, sdk_hashtable_val_ops_t v_ops);

void sdk_hashtable_destroy(sdk_hashtable_t* t);

sdk_err_t sdk_hashtable_resize(sdk_hashtable_t* t, sdk_size_t new_capacity);

sdk_err_t sdk_hashtable_put(sdk_hashtable_t * t, const void* key, const void* val);

void* sdk_hashtable_get(sdk_hashtable_t* t, const void* key);

sdk_err_t sdk_hashtable_remove(sdk_hashtable_t* t, const void* k);

sdk_bool_t sdk_hashtable_contains(sdk_hashtable_t* t, const void* k);

size_t sdk_hashtable_numcol(sdk_hashtable_t* t);

void sdk_hashtable_map(sdk_hashtable_t* t, void (*apply)(sdk_hashtable_item_t* item, void* ud), void* ud);

#endif /* INCLUDED_SDK_HASHTABLE_H */
