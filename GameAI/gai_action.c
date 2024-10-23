#include <gai_action.h>
#include <assert.h>
#include <sdk_memory.h>

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

gai_err_t gai_action_init(gai_action_t* action, const char* name
        , gai_action_function_t initializeFunction
        , gai_action_function_t updateFunction
        , gai_action_function_t cleanupFunction
        , void* ud
) {
    assert(action);
    if(name){
        size_t name_size = strlen(name);
        action->name = SDK_ALLOC(name_size+1);
        if(!action->name){
            return GAI_ERR_NOMEM;
        }
        memcpy(action->name, name, name_size);
        action->name[name_size]='\0';
    }else{
        action->name = 0;
    }
    
    action->initialize = initializeFunction;
    action->update = updateFunction;
    action->cleanup = cleanupFunction;
    action->userdata = ud;
    action->status = GAI_ACTION_STATUS_UNINITIALIZED;
    return GAI_ERR_OK;
}

void gai_action_destroy(gai_action_t* action){
    SDK_FREE(action->name);
}

void gai_action_initialize(gai_action_t* self, void* ud){
    if(self->status == GAI_ACTION_STATUS_UNINITIALIZED){
        if(self->initialize){
            self->initialize(self, ud);
        }
    }
    self->status = GAI_ACTION_STATUS_RUNNING;
}

int gai_action_update(gai_action_t* self, void* ud){
    if(self->status == GAI_ACTION_STATUS_TERMINATED){
        return GAI_ACTION_STATUS_TERMINATED;
    }else if(self->status==GAI_ACTION_STATUS_RUNNING){
        if(self->update){
            self->update(self, ud);
        }else{
            self->status = GAI_ACTION_STATUS_TERMINATED;
        }
    }
    return self->status;
}

void gai_action_cleanup(gai_action_t* self, void* ud){
    if(self->status == GAI_ACTION_STATUS_TERMINATED){
        if(self->cleanup){
            self->cleanup(self, ud);
        }
    }
    self->status = GAI_ACTION_STATUS_UNINITIALIZED;
}

