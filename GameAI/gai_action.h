#ifndef INCLUDED_GAI_ACTION_H
#define INCLUDED_GAI_ACTION_H

#ifndef INCLUDED_GAI_ERRORS_H
#include <gai_errors.h>
#endif /*INCLUDED_GAI_ERRORS_H*/


/* -------------------------------------------------------------------------------------------------------------- */
/*  */

#define GAI_ACTION_STATUS_UNINITIALIZED     0
#define GAI_ACTION_STATUS_RUNNING           1
#define GAI_ACTION_STATUS_TERMINATED        (-1)

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

typedef struct gai_action_s gai_action_t;

typedef int (*gai_action_function_t)(gai_action_t * action, void* userdata);

struct gai_action_s{
    char* name;
    int status;
    gai_action_function_t cleanup;
    gai_action_function_t initialize;
    gai_action_function_t update;
    void* userdata;
};

/* -------------------------------------------------------------------------------------------------------------- */
/*  */
gai_err_t gai_action_init(gai_action_t* action, const char* name
                    , gai_action_function_t initializeFunction
                    , gai_action_function_t updateFunction
                    , gai_action_function_t cleanupFunction
                    , void* ud
);

void gai_action_destroy(gai_action_t* action);

void gai_action_initialize(gai_action_t* self, void* ud);

int gai_action_update(gai_action_t* self, void* ud);

void gai_action_cleanup(gai_action_t* self, void* ud);


#endif /* INCLUDED_GAI_ACTION_H */
