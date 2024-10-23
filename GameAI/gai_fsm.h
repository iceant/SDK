#ifndef INCLUDED_GAI_FSM_H
#define INCLUDED_GAI_FSM_H

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

typedef struct gai_fsm_state_s{
    char* name;
    gai_action_t* action;
    sdk_vector_t transitions;
}gai_fsm_state_t;

typedef struct gai_fsm_transition_s{
    char* to_state_name;
    gai_evaluator_t * evaluator;
}gai_fsm_transition_t;

typedef struct gai_fsm_s{
    gai_fsm_state_t * current_state;
    sdk_vector_t states;
    void* userdata;
}gai_fsm_t;
/* -------------------------------------------------------------------------------------------------------------- */
/* State */

gai_err_t gai_fsm_state_init(gai_fsm_state_t * self, const char* name, gai_action_t * action);

void gai_fsm_state_destroy(gai_fsm_state_t * self);

/* -------------------------------------------------------------------------------------------------------------- */
/* Transition */

gai_err_t gai_fsm_transition_init(gai_fsm_transition_t * self, const char* to_state_name, gai_evaluator_t * evaluator);

void gai_fsm_transition_destroy(gai_fsm_transition_t * self);

/* -------------------------------------------------------------------------------------------------------------- */
/* FiniteStateMachine */

gai_err_t gai_fsm_init(gai_fsm_t * self, void* userdata);

void gai_fsm_destroy(gai_fsm_t * self);

gai_fsm_state_t * gai_fsm_find_state(gai_fsm_t * self, const char* state_name);

gai_fsm_transition_t * gai_fsm_find_transition(gai_fsm_t* self, const char* from_state_name, const char* to_state_name);

bool gai_fsm_contains_transition(gai_fsm_t* self, const char* from_state_name, const char* to_state_name);

bool gai_fsm_contains_state(gai_fsm_t* self, const char* state_name);

void gai_fsm_set_state(gai_fsm_t* self, const char* state_name, void* ud);

gai_err_t gai_fsm_add_state(gai_fsm_t * self, gai_fsm_state_t* state);

gai_err_t gai_fsm_add_transition(gai_fsm_t* self, const char* from_state_name, gai_fsm_transition_t * transition);

void gai_fsm_update(gai_fsm_t *self, void* ud);



#endif /*INCLUDED_GAI_FSM_H*/
