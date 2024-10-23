#include <gai_blackboard.h>
#include <assert.h>
#include <sdk_memory.h>
#include <sdk_string_hash.h>
#include <gai_knowledge.h>
/* -------------------------------------------------------------------------------------------------------------- */
/*  */

static uint32_t sdk_vector_hash(const void* data, void* arg){
    char str[43];
    const char* s = sdk_hash_int_to_str((sdk_size_t)data, str, sizeof(str));
    return sdk_string_hash(s, arg);
}

static void* sdk_vector_hash_cp(const void* data, void* arg){
    return (void*)data;
}

static sdk_bool_t sdk_vector_hash_eq(const void* data1, const void* data2, void* arg){
    return (data1==data2)?SDK_TRUE:SDK_FALSE;
}

static void sdk_vector_hash_free(void* data, void* arg){
    sdk_vector_t* vector = (sdk_vector_t*)data;
    sdk_size_t i;
    void* source;
    for(i=0; i<vector->size; i++){
        source = sdk_vector_get(vector, 0);
        sdk_vector_resize_remove(vector, 0);
    }
    sdk_vector_destroy(vector);
}

static sdk_hashtable_val_ops_t sdk_vector_hash_val_ops={
        .cp = sdk_vector_hash_cp,
        .free = sdk_vector_hash_free,
        .eq = sdk_vector_hash_eq,
        .arg = 0
};
/* -------------------------------------------------------------------------------------------------------------- */
/*  */

gai_err_t gai_blackboard_init(gai_blackboard_t * self, void* userdata){
    assert(self);
    self->userdata =userdata;
    sdk_hashtable_init(&self->attributes, 1024, sdk_string_hash_key_ops, sdk_voidp_hash_val_ops);
    sdk_hashtable_init(&self->sources, 1024, sdk_string_hash_key_ops, sdk_vector_hash_val_ops);

    return GAI_ERR_OK;
}

void gai_blackboard_destroy(gai_blackboard_t * self){
    sdk_hashtable_destroy(&self->attributes);
    sdk_hashtable_destroy(&self->sources);
}

gai_err_t gai_blackboard_add_source(gai_blackboard_t * self, const char* attribute, gai_knowledgesource_t* source){
    assert(self);
    assert(attribute);
    assert(source);

    sdk_err_t err;

    sdk_vector_t* vector = sdk_hashtable_get(&self->sources, attribute);
    if(!vector){
        vector = SDK_ALLOC(sizeof(*vector));
        if(!vector){
            return GAI_ERR_ERROR;
        }
        err = sdk_hashtable_put(&self->sources, attribute, vector);
        if(err!=SDK_ERR_OK){
            return GAI_ERR_ERROR;
        }
    }

    err = sdk_vector_resize_append(vector, source);
    if(err!=SDK_ERR_OK){
        return GAI_ERR_ERROR;
    }

    return GAI_ERR_OK;
}

gai_err_t gai_blackboard_remove_source(gai_blackboard_t *self, const char* attribute, gai_knowledgesource_t* source){

    assert(self);
    assert(attribute);
    assert(source);

    sdk_vector_t* vector = sdk_hashtable_get(&self->sources, attribute);
    if(!vector){
        return GAI_ERR_OK;
    }

    sdk_size_t idx;
    sdk_err_t err;
    void* p;
    for(idx = 0; idx < vector->size; idx++){
        p = sdk_vector_get(vector, idx);
        if(p==source){
            break;
        }
    }

    if(idx!=vector->size){
        err = sdk_vector_resize_remove(vector, idx);
        if(err!=SDK_ERR_OK){
            return GAI_ERR_ERROR;
        }
    }

    return GAI_ERR_OK;
}

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

static void* gai_blackboard_evaluate_source(gai_blackboard_t * self, sdk_vector_t* sources, void* ud){
    int best_confidence = 0;

    gai_knowledge_evaluate_result_t result;
    result.confidence=0;
    result.evaluation = 0;

    void* best_result = 0;

    sdk_size_t i;
    gai_knowledgesource_t * source;
    for(i=0; i<sources->size; i++){
        source = sdk_vector_get(sources, i);
        gai_knowledge_evaluate_result_t* eval = gai_knowledgesource_evaluate(source, ud, &result);
        if(eval->confidence > best_confidence){
            best_confidence = eval->confidence;
            best_result = eval->evaluation;
        }
    }
    return best_result;
}

void* gai_blackboard_get(gai_blackboard_t *self, const char* attribute, void* ud){
    assert(self);
    assert(attribute);

    sdk_vector_t* sources = sdk_hashtable_get(&self->sources, attribute);
    if(!sources){
        return sdk_hashtable_get(&self->attributes, attribute);
    }
    return gai_blackboard_evaluate_source(self, sources, ud);
}

gai_err_t gai_blackboard_set(gai_blackboard_t * self, const char* attribute, void* value){
    assert(self);
    assert(attribute);
    assert(value);

    sdk_err_t err = sdk_hashtable_put(&self->attributes, attribute, value);
    if(err!=SDK_ERR_OK){
        return GAI_ERR_ERROR;
    }

    return GAI_ERR_OK;
}

