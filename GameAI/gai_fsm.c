#include <gai_fsm.h>
#include <assert.h>
#include <sdk_memory.h>
#include <stdio.h>

/* -------------------------------------------------------------------------------------------------------------- */
/*  */


gai_err_t gai_fsm_state_init(gai_fsm_state_t * self, const char* name, gai_action_t * action)
{
    assert(self);
    if(name){
        size_t name_size = strlen(name);
        self->name = SDK_ALLOC(name_size+1);
        if(!self->name){
            return GAI_ERR_NOMEM;
        }
        memcpy(self->name, name, name_size);
        self->name[name_size]='\0';
    }else{
        self->name = 0;
    }

    self->action = action;
    sdk_err_t  err = sdk_vector_init(&self->transitions, 0);
    if(err!=SDK_ERR_OK){
        return GAI_ERR_ERROR;
    }
    return GAI_ERR_OK;
}


void gai_fsm_state_destroy(gai_fsm_state_t * self){
    sdk_size_t i;
    gai_fsm_transition_t * transition;

    assert(self);
    SDK_FREE(self->name);
    gai_action_destroy(self->action);

    for(i=0; i<self->transitions.size; i++){
        transition = sdk_vector_get(&self->transitions, 0);
        gai_fsm_transition_destroy(transition);
        sdk_vector_resize_remove(&self->transitions, 0);
    }
    sdk_vector_destroy(&self->transitions);
}

gai_err_t gai_fsm_transition_init(gai_fsm_transition_t * self, const char* to_state_name, gai_evaluator_t * evaluator)
{
    assert(self);
    if(to_state_name){
        size_t to_state_name_size = strlen(to_state_name);
        self->to_state_name = SDK_ALLOC(to_state_name_size+1);
        if(!self->to_state_name){
            return GAI_ERR_NOMEM;
        }
        memcpy(self->to_state_name, to_state_name, to_state_name_size);
        self->to_state_name[to_state_name_size]='\0';
    }else{
        self->to_state_name = 0;
    }

    self->evaluator = evaluator;
    return GAI_ERR_OK;
}

void gai_fsm_transition_destroy(gai_fsm_transition_t * self){
    assert(self);
    SDK_FREE(self->to_state_name);
    gai_evaluator_destroy(self->evaluator);
}


/* -------------------------------------------------------------------------------------------------------------- */
/*  */

gai_err_t gai_fsm_init(gai_fsm_t * self, void* userdata)
{
    assert(self);
    self->userdata = userdata;
    self->current_state = 0;

    sdk_err_t err = sdk_vector_init(&self->states, 0);
    if(err!=SDK_ERR_OK){
        return GAI_ERR_ERROR;
    }

    return GAI_ERR_OK;
}

void gai_fsm_destroy(gai_fsm_t * self){
    assert(self);
    sdk_size_t  i;
    gai_fsm_state_t * state;
    gai_fsm_transition_t * transition;

    for(i=0; i<self->states.size; i++){
        state = sdk_vector_get(&self->states, 0);
        gai_fsm_state_destroy(state);
        sdk_vector_resize_remove(&self->states, 0);
    }
    sdk_vector_destroy(&self->states);
}

bool gai_fsm_contains_state(gai_fsm_t* self, const char* state_name){
    assert(self);
    assert(state_name);

    return (gai_fsm_find_state(self, state_name)==0)?false:true;
}

gai_fsm_state_t * gai_fsm_find_state(gai_fsm_t * self, const char* state_name){
    sdk_size_t i;
    gai_fsm_state_t * state;
    assert(self);
    assert(state_name);
    for(i =0 ;i<self->states.size; i++){
        state = sdk_vector_get(&self->states, i);
        if(strncmp(state->name, state_name, strlen(state_name))==0){
            return state;
        }
    }
    return 0;
}

gai_fsm_transition_t * gai_fsm_find_transition(gai_fsm_t* self, const char* from_state_name, const char* to_state_name){
    assert(self);
    assert(from_state_name);
    assert(to_state_name);

    gai_fsm_state_t * state = gai_fsm_find_state(self, from_state_name);
    if(state==0){
        return 0;
    }

    sdk_size_t i;
    gai_fsm_transition_t * transition;
    for(i=0; i<state->transitions.size; i++){
        transition = sdk_vector_get(&state->transitions, i);
        if(strncmp(transition->to_state_name, to_state_name, strlen(to_state_name))==0){
            return transition;
        }
    }

    return 0;
}

bool gai_fsm_contains_transition(gai_fsm_t* self, const char* from_state_name, const char* to_state_name){
    return (gai_fsm_find_transition(self, from_state_name, to_state_name)!=0)?true:false;
}

void gai_fsm_set_state(gai_fsm_t* self, const char* state_name, void* ud){
    gai_fsm_state_t * state = gai_fsm_find_state(self, state_name);
    if(!state){
        return;
    }
    if(self->current_state){
        gai_action_cleanup(self->current_state->action, ud);
    }
    self->current_state = state;
    gai_action_initialize(self->current_state->action, ud);
}

gai_err_t gai_fsm_add_state(gai_fsm_t * self, gai_fsm_state_t* state){
    assert(self);
    assert(state);

    sdk_err_t err = sdk_vector_resize_append(&self->states, state);
    if(err!=SDK_ERR_OK){
        return GAI_ERR_ERROR;
    }

    return GAI_ERR_OK;

}

gai_err_t gai_fsm_add_transition(gai_fsm_t* self, const char* from_state_name, gai_fsm_transition_t * transition){
    assert(self);
    assert(from_state_name);
    assert(transition);

    gai_fsm_state_t * state = gai_fsm_find_state(self, from_state_name);
    if(!state){
        return GAI_ERR_ERROR;
    }

    sdk_err_t  err = sdk_vector_resize_append(&state->transitions, transition);
    if(err!=SDK_ERR_OK){
        return GAI_ERR_ERROR;
    }

    return GAI_ERR_OK;
}

const char* gai_fsm_evaluate_transitions(gai_fsm_t* self, sdk_vector_t * transitions, void* ud){
    sdk_size_t i;
    gai_fsm_transition_t * transition;
    for(i=0; i<transitions->size; i++){
        transition = sdk_vector_get(transitions, i);
        if(transition->evaluator->function(transition->evaluator, ud)){
            return transition->to_state_name;
        }
    }
    return 0;
}

void gai_fsm_update(gai_fsm_t *self, void* ud){
    if(self->current_state){
        int status = self->current_state->action->status;
        if(status == GAI_ACTION_STATUS_RUNNING){
            gai_action_update(self->current_state->action, ud);
        }else if(status==GAI_ACTION_STATUS_TERMINATED){
            gai_action_cleanup(self->current_state->action, ud);
            gai_fsm_state_t * state = 0;
            const char* to_state_name = gai_fsm_evaluate_transitions(self, &self->current_state->transitions, ud);
            if(to_state_name){
                state = gai_fsm_find_state(self,to_state_name);
            }
            if(state){
                self->current_state = state;
                gai_action_initialize(self->current_state->action, ud);
            }
        }else if(status==GAI_ACTION_STATUS_UNINITIALIZED){
            gai_fsm_state_t * state = 0;
            const char* to_state_name = gai_fsm_evaluate_transitions(self, &self->current_state->transitions, ud);
            if(to_state_name){
                state = gai_fsm_find_state(self,to_state_name);
            }
            if(state){
                self->current_state = state;
                gai_action_initialize(self->current_state->action, ud);
            }
        }
    }
}



