#include <gai_decisiontree.h>
#include <assert.h>
#include <sdk_memory.h>
#include <stdio.h>

/* -------------------------------------------------------------------------------------------------------------- */
/* DecisionTreeNode */

gai_err_t gai_decisiontreenode_init(gai_decisiontreenode_t* self, const char* name){
    assert(self);
    
    sdk_err_t err = sdk_vector_init(&self->children, 0);
    if(err!=SDK_ERR_OK){
        return GAI_ERR_ERROR;
    }
    
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
    
    self->evaluator = 0;
    self->action = 0;
    
    return GAI_ERR_OK;
}

void gai_decisiontreenode_destroy(gai_decisiontreenode_t * self){
    SDK_FREE(self->name);
    if(self->action){
        gai_action_destroy(self->action);
    }
//    gai_decisiontreenode_t * child;
//    for(sdk_size_t i=0; i<self->children.size; i++){
//        child = sdk_vector_get(&self->children, 0);
//        if(child->action){
//            gai_action_destroy(child->action);
//        }
//        gai_decisiontreenode_destroy(self);
//        sdk_vector_resize_remove(&self->children, 0);
//    }
    sdk_vector_destroy(&self->children);
}

gai_err_t gai_decisiontreenode_addchild(gai_decisiontreenode_t* self, gai_decisiontreenode_t* child){
    assert(self);
    assert(child);
    sdk_err_t  err = sdk_vector_resize_append(&self->children, child);
    if(err!=SDK_ERR_OK){
        return GAI_ERR_ERROR;
    }
    return GAI_ERR_OK;
}


void gai_decisiontreenode_set_evaluator(gai_decisiontreenode_t * self, gai_decisiontree_evaluator_t evaluator){
    assert(self);
    assert(evaluator);
    self->evaluator = evaluator;
}

gai_action_t * gai_decisiontreenode_evaluate(gai_decisiontreenode_t* self, void* ud){
    assert(self);
    assert(self->evaluator);
    
    int eval = self->evaluator(self, ud);
    gai_decisiontreenode_t * choice = sdk_vector_get(&self->children, eval);
    if(choice->children.size==0){
        /* leaf action */
        return choice->action;
    }else{
        /* branch node, recursively evaluate children */
        return gai_decisiontreenode_evaluate(choice, ud);
    }
}

/* -------------------------------------------------------------------------------------------------------------- */
/*  */


gai_err_t gai_decisiontree_init(gai_decisiontree_t * self){
    assert(self);
    self->branch = 0;
    self->current_action = 0;
    
    return GAI_ERR_OK;
}

void gai_decisiontree_destroy(gai_decisiontree_t * self){
    assert(self);
    self->branch = 0;
    self->current_action = 0;
}

void gai_decisiontree_update(gai_decisiontree_t* self, void* ud){
    if(self->branch==0){
        return;
    }
    /* Search the tree for an Action to run if not currently executing an Action */
    if(self->current_action==0){
        self->current_action = gai_decisiontreenode_evaluate(self->branch, ud);
        gai_action_initialize(self->current_action, ud);
    }
    
    gai_action_update(self->current_action, ud);
    
    /* Cleanup the action once it has terminated */
    if(self->current_action->status==GAI_ACTION_STATUS_TERMINATED){
        gai_action_cleanup(self->current_action, ud);
        self->current_action = 0;
    }
}

void gai_decisiontree_set_branch(gai_decisiontree_t* self, gai_decisiontreenode_t* branch){
    assert(self);
    assert(branch);
    self->branch = branch;
}

