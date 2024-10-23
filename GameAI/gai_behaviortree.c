#include <gai_behaviortree.h>
#include <assert.h>
#include "sdk_memory.h"

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

gai_err_t gai_behaviortreenode_init(gai_behaviortreenode_t* self, const char* name, int type){
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
    self->action = 0;
    self->evaluator= 0;
    self->parent = 0;
    self->type = type;
    sdk_err_t err = sdk_vector_init(&self->children, 0);
    if(err!=SDK_ERR_OK) {
        return GAI_ERR_ERROR;
    }

    return GAI_ERR_OK;
}

void gai_behaviortreenode_destroy(gai_behaviortreenode_t * self){
    sdk_size_t i;
    gai_behaviortreenode_t * child;

    SDK_FREE(self->name);
    if(self->action){
        gai_action_destroy(self->action);
    }
    if(self->evaluator){
        gai_evaluator_destroy(self->evaluator);
    }
    for(i=0; i<self->children.size;i++){
        child = sdk_vector_get(&self->children, 0);
        gai_behaviortreenode_destroy(child);
        sdk_vector_resize_remove(&self->children, 0);
    }
    sdk_vector_destroy(&self->children);
}

gai_err_t gai_behaviortreenode_add_child(gai_behaviortreenode_t * self, gai_behaviortreenode_t * child){
    assert(self);
    assert(child);

    sdk_err_t err = sdk_vector_resize_append(&self->children, child);
    if(err!=SDK_ERR_OK){
        return GAI_ERR_ERROR;
    }
    child->parent = self;
    return GAI_ERR_OK;
}

gai_behaviortreenode_t * gai_behaviortreenode_get_child(gai_behaviortreenode_t * self, sdk_size_t idx){
    assert(self);
    assert(idx < self->children.size);
    return sdk_vector_get(&self->children, idx);
}

sdk_size_t gai_behaviortreenode_child_index(gai_behaviortreenode_t * self, gai_behaviortreenode_t * child){
    assert(self);
    assert(child);

    sdk_size_t i;
    gai_behaviortreenode_t * node;
    for(i=0; i<self->children.size;i++){
        node = sdk_vector_get(&self->children, i);
        if(node==child){
            return i;
        }
    }
    return GAI_INVALID_INDEX;
}

void gai_behaviortreenode_set_parent(gai_behaviortreenode_t * self, gai_behaviortreenode_t * parent){
    assert(self);
    assert(parent);
    self->parent = parent;
}

void gai_behaviortreenode_set_action(gai_behaviortreenode_t * self, gai_action_t * action){
    assert(self);
    assert(action);
    self->action = action;
}

void   gai_behaviortreenode_set_evaluator(gai_behaviortreenode_t * self, gai_evaluator_t * evaluator){
    assert(self);
    assert(evaluator);
    self->evaluator = evaluator;
}

void gai_behaviortreenode_set_type(gai_behaviortreenode_t * self, int type){
    assert(self);
    self->type = type;
}

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

gai_behaviortreenode_t * gai_behaviortree_action_node_init(gai_behaviortreenode_t * self, const char* name, gai_action_t * action){
    gai_err_t err = gai_behaviortreenode_init(self, name, GAI_BEHAVIOR_TREE_NODE_TYPE_ACTION);
    if(err!=GAI_ERR_OK){
        return 0;
    }
    gai_behaviortreenode_set_action(self, action);
    return self;
}

gai_behaviortreenode_t * gai_behaviortree_condition_node_init(gai_behaviortreenode_t * self, const char* name, gai_evaluator_t* evaluator){
    gai_err_t err = gai_behaviortreenode_init(self, name, GAI_BEHAVIOR_TREE_NODE_TYPE_CONDITION);
    if(err!=GAI_ERR_OK){
        return 0;
    }
    gai_behaviortreenode_set_evaluator(self, evaluator);
    return self;
}

gai_behaviortreenode_t * gai_behaviortree_selector_node_init(gai_behaviortreenode_t * self, const char* name){
    gai_err_t err = gai_behaviortreenode_init(self, name, GAI_BEHAVIOR_TREE_NODE_TYPE_SELECTOR);
    if(err!=GAI_ERR_OK){
        return 0;
    }
    return self;
}

gai_behaviortreenode_t * gai_behaviortree_sequence_node_init(gai_behaviortreenode_t * self, const char* name){
    gai_err_t err = gai_behaviortreenode_init(self, name, GAI_BEHAVIOR_TREE_NODE_TYPE_SEQUENCE);
    if(err!=GAI_ERR_OK){
        return 0;
    }
    return self;
}


/* -------------------------------------------------------------------------------------------------------------- */
/*  */

gai_err_t gai_behaviortree_init(gai_behaviortree_t * self, void* ud){
    assert(self);
    self->current_node = 0;
    self->root = 0;
    self->userdata = ud;
    return GAI_ERR_OK;
}

void gai_behaviortree_destroy(gai_behaviortree_t * self){
    assert(self);
    if(self->root){
        gai_behaviortreenode_destroy(self->root);
    }
}



/* -------------------------------------------------------------------------------------------------------------- */
/*  */

typedef struct evaluate_result_s{
    gai_behaviortreenode_t * node;
    bool result;
}evaluate_result_t;

static evaluate_result_t* gai_behaviortree_evaluate_sequence(gai_behaviortree_t * self
                                                             , gai_behaviortreenode_t * node
                                                             , void* ud
                                                             , sdk_size_t index
                                                             , evaluate_result_t* result);

static evaluate_result_t* gai_behaviortree_evaluate_selector(gai_behaviortree_t * self, gai_behaviortreenode_t * node, void* ud, evaluate_result_t* result){
    sdk_size_t i;
    gai_behaviortreenode_t * child;
    for(i=0; i<node->children.size;i++){
        child = sdk_vector_get(&node->children, i);
        if(child->type == GAI_BEHAVIOR_TREE_NODE_TYPE_ACTION){
            result->node = child;
            result->result = true;
            return result;
        }else if(child->type==GAI_BEHAVIOR_TREE_NODE_TYPE_CONDITION){
            result->result = false;
            return result;
        }else if(child->type==GAI_BEHAVIOR_TREE_NODE_TYPE_SELECTOR){
            result = gai_behaviortree_evaluate_selector(self, node, ud, result);
            if(result->result){
                return result;
            }
        }else if(child->type == GAI_BEHAVIOR_TREE_NODE_TYPE_SEQUENCE){
            result = gai_behaviortree_evaluate_sequence(self, child, ud, 0, result);
            if(result->result){
                return result;
            }
        }
    }
    result->result = false;
    return result;
}


static evaluate_result_t* gai_behaviortree_evaluate_sequence(gai_behaviortree_t * self
                                                             , gai_behaviortreenode_t * node
                                                             , void* ud
                                                             , sdk_size_t index
                                                             , evaluate_result_t* result)
{
    gai_behaviortreenode_t * child;
    for(sdk_size_t i = index; i<node->children.size; i++){
        child = sdk_vector_get(&node->children, i);
        if(child->type == GAI_BEHAVIOR_TREE_NODE_TYPE_ACTION){
            result->node = child;
            result->result = true;
            return result;
        }else if(child->type == GAI_BEHAVIOR_TREE_NODE_TYPE_CONDITION){
            if(!gai_evaluator_evaluate(child->evaluator, ud)){
                result->result = false;
                return result;
            }
        }else if(child->type==GAI_BEHAVIOR_TREE_NODE_TYPE_SELECTOR){
            result = gai_behaviortree_evaluate_selector(self, node, ud, result);
            if(!result->result){
                result->result = false;
                return result;
            }else if(result->result && result->node){
                return result;
            }
        }else if(child->type==GAI_BEHAVIOR_TREE_NODE_TYPE_SEQUENCE){
            result = gai_behaviortree_evaluate_sequence(self,node, ud, index, result);
            if(!result->result){
                result->result = false;
                return result;
            }else if(result->result && result->node){
                return result;
            }
        }
    }
    result->result = true;
    return result;
}

static gai_behaviortreenode_t* gai_behaviortree_evaluate_node(gai_behaviortree_t * self, gai_behaviortreenode_t *node
                                                          , void* ud, evaluate_result_t* result){
    if(node->type == GAI_BEHAVIOR_TREE_NODE_TYPE_ACTION){
        return node;
    }else if(node->type == GAI_BEHAVIOR_TREE_NODE_TYPE_CONDITION){
        assert(false);
    }else if(node->type==GAI_BEHAVIOR_TREE_NODE_TYPE_SELECTOR){
        result = gai_behaviortree_evaluate_selector(self, node, ud, result);
        if(result->result){
            return result->node;
        }
    } else if(node->type==GAI_BEHAVIOR_TREE_NODE_TYPE_SEQUENCE){
        result = gai_behaviortree_evaluate_sequence(self, node, ud, 0, result);
        if(result->result){
            return result->node;
        }
    }
    return 0;
}

static evaluate_result_t * gai_behaviortree_continue_evaluate(gai_behaviortree_t * self, gai_behaviortreenode_t * node, void* ud, evaluate_result_t* result){
    gai_behaviortreenode_t * parent_node = node->parent;
    gai_behaviortreenode_t * child_node = node;

    while(parent_node){
        if(parent_node->type == GAI_BEHAVIOR_TREE_NODE_TYPE_SEQUENCE){
            sdk_size_t child_idx = gai_behaviortreenode_child_index(parent_node, child_node);
            if(child_idx < parent_node->children.size){
                return gai_behaviortree_evaluate_sequence(self, parent_node, ud, child_idx+1, result);
            }
        }
        child_node = parent_node;
        parent_node = child_node->parent;
    }

    return result;
}

void gai_behaviortree_update(gai_behaviortree_t* self, void* ud){
    evaluate_result_t result;
    result.result = false;
    result.node = 0;

    if(self->current_node==0){
        self->current_node = gai_behaviortree_evaluate_node(self, self->root, ud, &result);
    }

    if(self->current_node){
        int status = self->current_node->action->status;
        if(status==GAI_ACTION_STATUS_UNINITIALIZED){
            gai_action_initialize(self->current_node->action, ud);
        }else if(status==GAI_ACTION_STATUS_TERMINATED){
            gai_action_cleanup(self->current_node->action, ud);
            self->current_node = gai_behaviortree_continue_evaluate(self, self->current_node, ud, &result)->node;

        }else if(status==GAI_ACTION_STATUS_RUNNING){
            gai_action_update(self->current_node->action, ud);
        }
    }
}

void gai_behaviortree_set_root(gai_behaviortree_t * self, gai_behaviortreenode_t * root){
    self->root = root;
}
