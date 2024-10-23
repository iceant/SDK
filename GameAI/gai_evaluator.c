#include <gai_evaluator.h>
#include <assert.h>
#include <sdk_memory.h>

/* -------------------------------------------------------------------------------------------------------------- */
/*  */


gai_err_t gai_evaluator_init(gai_evaluator_t* self, const char* name,
                             gai_evaluator_function_t function,
                             void* ud){
    assert(self);
    assert(function);
    
    if(name){
        size_t size = strlen(name);
        self->name = SDK_ALLOC(size+1);
        if(!self->name){
            return GAI_ERR_NOMEM;
        }
        memcpy(self->name, name, size);
        self->name[size]='\0';
    }else{
        self->name = 0;
    }
    self->function = function;
    self->userdata = ud;
    return GAI_ERR_OK;
}

void gai_evaluator_destroy(gai_evaluator_t* self){
    assert(self);
    SDK_FREE(self->name);
}

bool gai_evaluator_evaluate(gai_evaluator_t* self, void* ud){
    assert(self);
    return self->function(self, ud);
}
