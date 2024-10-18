#include <sdk_hashtable.h>
#include <sdk_memory.h>

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

#define DEFAULT_CAPACITY 1024
#define GROWTH_FACTOR    1

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

static sdk_hashtable_item_t * sdk_hashtable_item_get(sdk_hashtable_t* t, const void* k){
    sdk_hashtable_item_t* result = 0;
    sdk_hashtable_item_t * node = 0;
    sdk_vector_t * bucket = 0;
    
    uint32_t computed_hash;
    sdk_size_t bucket_idx;
    
    computed_hash = t->key_ops.hash(k, t->key_ops.arg);
    bucket_idx = computed_hash % t->capacity;
    bucket = t->buckets[bucket_idx];
    
    if(!bucket) return 0;
    for(sdk_size_t i=0; i<bucket->size; i++){
        node = sdk_vector_get(bucket, i);
        if(node->hash == computed_hash){
            if(t->key_ops.eq(node->key, k, t->key_ops.arg)){
                result = node;
                break;
            }
        }
    }
    return result;
}

/* -------------------------------------------------------------------------------------------------------------- */
/*  */


sdk_err_t sdk_hashtable_resize(sdk_hashtable_t* t, sdk_size_t new_capacity){
    sdk_vector_t ** new_buckets;
    sdk_vector_t * bucket;
    sdk_hashtable_item_t* node;
    sdk_size_t new_idx;
    sdk_err_t err;
    
    new_buckets = SDK_ALLOC(new_capacity * sizeof(*new_buckets));
    if(!new_buckets){
        return SDK_ERR_NOMEM;
    }
    
    for(int i=0; i<new_capacity; i++){
        new_buckets[i] = 0;
    }
    
    for(sdk_size_t i=0; i<t->capacity; i++){
        bucket = t->buckets[i];
        if(!bucket) continue;
        for(sdk_size_t j=0; j<bucket->size; j++){
            node = sdk_vector_get(bucket, j);
            new_idx = node->hash % new_capacity;
            if(!new_buckets[new_idx]){
                SDK_NEW(new_buckets[new_idx]);
                if(!new_buckets[new_idx]){
                    return SDK_ERR_NOMEM;
                }
                sdk_vector_init(new_buckets[new_idx], 1);
            }
            err = sdk_vector_resize_append(new_buckets[new_idx], node);
            if(err!=SDK_ERR_OK){
                return err;
            }
        }
    }
    
    t->capacity = new_capacity;
    SDK_FREE(t->buckets);
    t->buckets = new_buckets;
    
    return SDK_ERR_OK;
}
/* -------------------------------------------------------------------------------------------------------------- */
/*  */

sdk_err_t sdk_hashtable_init(sdk_hashtable_t* t, sdk_size_t init_buckets
        , sdk_hashtable_key_ops_t k_ops, sdk_hashtable_val_ops_t v_ops)
{
    t->capacity = init_buckets;
    t->buckets = SDK_ALLOC(sizeof(*t->buckets) * init_buckets);
    if(!t->buckets){
        return SDK_ERR_NOMEM;
    }
    t->size = 0;
    t->val_ops = v_ops;
    t->key_ops = k_ops;
    
    for(size_t i=0; i<t->capacity; i++){
        t->buckets[i] = 0;
    }
    
    return SDK_ERR_OK;
}

void sdk_hashtable_destroy(sdk_hashtable_t* t){
    sdk_vector_t *bucket_p;
    sdk_hashtable_item_t * item_p;
    
    for(sdk_size_t i=0; i<t->capacity; i++){
        bucket_p = t->buckets[i];
        if(!bucket_p) continue;
        for(sdk_size_t j=0; j<bucket_p->size; j++){
            item_p = sdk_vector_get(bucket_p, j);
            t->key_ops.free(item_p->key, t->key_ops.arg);
            t->val_ops.free(item_p->val, t->val_ops.arg);
            SDK_FREE(item_p);
        }
        sdk_vector_destroy(bucket_p);
        SDK_FREE(bucket_p);
    }
    SDK_FREE(t->buckets);
}

sdk_err_t sdk_hashtable_put(sdk_hashtable_t * t, const void* key, const void* val){
    sdk_hashtable_item_t* node;
    sdk_size_t bucket_idx;
    sdk_err_t err;
    
    node = sdk_hashtable_item_get(t, key);
    if(node){
       t->val_ops.free(node->val, t->val_ops.arg);
       node->val= val?t->val_ops.cp(val, t->val_ops.arg):0;
    }else{
        SDK_NEW(node);
        if(!node){
            return SDK_ERR_NOMEM;
        }
        node->hash = t->key_ops.hash(key, t->key_ops.arg);
        node->key = t->key_ops.cp(key, t->key_ops.arg);
        node->val = t->val_ops.cp(val, t->val_ops.arg);
        
        bucket_idx = node->hash % t->capacity;
        if(!t->buckets[bucket_idx]){
            SDK_NEW(t->buckets[bucket_idx]);
            if(!t->buckets[bucket_idx]){
                return SDK_ERR_NOMEM;
            }
            sdk_vector_init(t->buckets[bucket_idx], 1);
        }
        err = sdk_vector_resize_append(t->buckets[bucket_idx], node);
        if(err!=SDK_ERR_OK){
            return err;
        }
        t->size++;
        if(t->size > t->capacity * GROWTH_FACTOR){
            return sdk_hashtable_resize(t, t->capacity * GROWTH_FACTOR);
        }
    }
    return SDK_ERR_OK;
}

void* sdk_hashtable_get(sdk_hashtable_t* t, const void* key){
    sdk_hashtable_item_t * item = sdk_hashtable_item_get(t, key);
    if(!item) return 0;
    return item->val;
}

sdk_err_t sdk_hashtable_remove(sdk_hashtable_t* t, const void* k){
    sdk_hashtable_item_t* result = 0;
    sdk_hashtable_item_t * node = 0;
    sdk_vector_t * bucket = 0;
    
    uint32_t computed_hash;
    sdk_size_t bucket_idx;
    sdk_err_t err;
    
    computed_hash = t->key_ops.hash(k, t->key_ops.arg);
    bucket_idx = computed_hash % t->capacity;
    bucket = t->buckets[bucket_idx];
    
    if(!bucket) return SDK_ERR_INVAL;
    
    for(sdk_size_t i=0; i<bucket->size; i++){
        node = sdk_vector_get(bucket, i);
        if(node->hash == computed_hash){
            if(t->key_ops.eq(node->key, k, t->key_ops.arg)){
                result = node;
                err = sdk_vector_resize_remove(bucket, i);
                if(err!=SDK_ERR_OK){
                    return err;
                }
                break;
            }
        }
    }
    
    if(result){
        t->key_ops.free(result->key, t->key_ops.arg);
        t->val_ops.free(result->val, t->val_ops.arg);
        SDK_FREE(result);
        t->size--;
        if(t->size < t->capacity/4){
            err = sdk_hashtable_resize(t, t->capacity / 2);
            if(err!=SDK_ERR_OK){
                return err;
            }
        }
        return SDK_ERR_OK;
    }
    
    return SDK_ERR_INVAL;
}

sdk_bool_t sdk_hashtable_contains(sdk_hashtable_t* t, const void* k){
    return sdk_hashtable_item_get(t, k)!=0?SDK_TRUE:SDK_FALSE;
}

static sdk_size_t item_numcol(sdk_vector_t* bucket){
    if(!bucket) return 0;
    return (bucket->size==0)?0:(bucket->size-1);
}

size_t sdk_hashtable_numcol(sdk_hashtable_t* t){
    sdk_size_t result = 0;
    for(sdk_size_t i=0; i<t->capacity; i++){
        result+= item_numcol(t->buckets[i]);
    }
    return result;
}

void sdk_hashtable_map(sdk_hashtable_t* t, void (*apply)(sdk_hashtable_item_t* item, void* ud), void* ud){
    sdk_vector_t * bucket;
    sdk_hashtable_item_t* node;
    
    for(sdk_size_t i=0; i<t->capacity; i++){
        bucket = t->buckets[i];
        if(!bucket) continue;
        for(sdk_size_t j=0; j<bucket->size; j++){
            node = sdk_vector_get(bucket, j);
            apply(node, ud);
        }
    }
}

