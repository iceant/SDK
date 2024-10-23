#ifndef INCLUDED_GAI_DECISIONTREE_H
#define INCLUDED_GAI_DECISIONTREE_H

#ifndef INCLUDED_GAI_ERRORS_H
#include <gai_errors.h>
#endif /*INCLUDED_GAI_ERRORS_H*/

#ifndef INCLUDED_SDK_VECTOR_H
#include <sdk_vector.h>
#endif /*INCLUDED_SDK_VECTOR_H*/

#ifndef INCLUDED_GAI_ACTION_H
#include <gai_action.h>
#endif /*INCLUDED_GAI_ACTION_H*/


/* -------------------------------------------------------------------------------------------------------------- */
/*  */


typedef struct gai_decisiontreenode_s gai_decisiontreenode_t;
typedef struct gai_decisiontree_s gai_decisiontree_t;

typedef int (*gai_decisiontree_evaluator_t)(gai_decisiontreenode_t * self, void* ud);

struct gai_decisiontreenode_s{
    char* name;
    sdk_vector_t children;
    gai_decisiontree_evaluator_t evaluator;
    gai_action_t* action;
};

struct gai_decisiontree_s{
    gai_decisiontreenode_t * branch;
    gai_action_t* current_action;
};

/* -------------------------------------------------------------------------------------------------------------- */
/*  */
gai_err_t gai_decisiontreenode_init(gai_decisiontreenode_t* self, const char* name);

void gai_decisiontreenode_destroy(gai_decisiontreenode_t * self);

gai_err_t gai_decisiontreenode_addchild(gai_decisiontreenode_t * self, gai_decisiontreenode_t * child);

gai_action_t* gai_decisiontreenode_evaluate(gai_decisiontreenode_t* self, void* ud);

void gai_decisiontreenode_set_evaluator(gai_decisiontreenode_t * self, gai_decisiontree_evaluator_t evaluator);

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

gai_err_t gai_decisiontree_init(gai_decisiontree_t * self);

void gai_decisiontree_destroy(gai_decisiontree_t * self);

void gai_decisiontree_update(gai_decisiontree_t* self, void* ud);

void gai_decisiontree_set_branch(gai_decisiontree_t* self, gai_decisiontreenode_t* branch);

#endif /* INCLUDED_GAI_DECISIONTREE_H */
