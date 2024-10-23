#ifndef INCLUDED_GAI_BEHAVIORTREE_H
#define INCLUDED_GAI_BEHAVIORTREE_H

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

#ifndef INCLUDED_GAI_ERRORS_H
#include <gai_errors.h>
#endif /*INCLUDED_GAI_ERRORS_H*/

#ifndef INCLUDED_GAI_ACTION_H
#include <gai_action.h>
#endif /*INCLUDED_GAI_ACTION_H*/

#ifndef INCLUDED_GAI_EVALUATOR_H
#include <gai_evaluator.h>
#endif /*INCLUDED_GAI_EVALUATOR_H*/

#ifndef INCLUDED_SDK_VECTOR_H
#include <sdk_vector.h>
#endif /*INCLUDED_SDK_VECTOR_H*/


/* -------------------------------------------------------------------------------------------------------------- */
/*  */

#define GAI_BEHAVIOR_TREE_NODE_TYPE_ACTION      1
#define GAI_BEHAVIOR_TREE_NODE_TYPE_CONDITION   2
#define GAI_BEHAVIOR_TREE_NODE_TYPE_SELECTOR    3
#define GAI_BEHAVIOR_TREE_NODE_TYPE_SEQUENCE    4

#define GAI_INVALID_INDEX                       (-1)

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

typedef struct gai_behaviortreenode_s gai_behaviortreenode_t;

struct gai_behaviortreenode_s{
    int type;
    char* name;
    gai_action_t* action;
    gai_evaluator_t * evaluator;
    gai_behaviortreenode_t * parent;
    sdk_vector_t children;
};

typedef struct gai_behaviortree_s{
    gai_behaviortreenode_t * current_node;
    gai_behaviortreenode_t * root;
    void* userdata;
}gai_behaviortree_t;

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

gai_err_t gai_behaviortreenode_init(gai_behaviortreenode_t* self, const char* name, int type);

void gai_behaviortreenode_destroy(gai_behaviortreenode_t * self);

gai_err_t gai_behaviortreenode_add_child(gai_behaviortreenode_t * self, gai_behaviortreenode_t * child);

gai_behaviortreenode_t * gai_behaviortreenode_get_child(gai_behaviortreenode_t * self, sdk_size_t idx);

sdk_size_t gai_behaviortreenode_child_index(gai_behaviortreenode_t * self, gai_behaviortreenode_t * child);

void gai_behaviortreenode_set_parent(gai_behaviortreenode_t * self, gai_behaviortreenode_t * parent);

void gai_behaviortreenode_set_action(gai_behaviortreenode_t * self, gai_action_t * action);

void   gai_behaviortreenode_set_evaluator(gai_behaviortreenode_t * self, gai_evaluator_t * evaluator);

void gai_behaviortreenode_set_type(gai_behaviortreenode_t * self, int type);

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

gai_behaviortreenode_t * gai_behaviortree_action_node_init(gai_behaviortreenode_t * self, const char* name, gai_action_t * action);

gai_behaviortreenode_t * gai_behaviortree_condition_node_init(gai_behaviortreenode_t * self, const char* name, gai_evaluator_t* evaluator);

gai_behaviortreenode_t * gai_behaviortree_selector_node_init(gai_behaviortreenode_t * self, const char* name);

gai_behaviortreenode_t * gai_behaviortree_sequence_node_init(gai_behaviortreenode_t * self, const char* name);

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

gai_err_t gai_behaviortree_init(gai_behaviortree_t * self, void* ud);

void gai_behaviortree_destroy(gai_behaviortree_t * self);

void gai_behaviortree_set_root(gai_behaviortree_t * self, gai_behaviortreenode_t * root);

void gai_behaviortree_update(gai_behaviortree_t* self, void* ud);






#endif /*INCLUDED_GAI_BEHAVIORTREE_H*/
