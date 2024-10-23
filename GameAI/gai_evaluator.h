#ifndef INCLUDED_GAI_EVALUATOR_H
#define INCLUDED_GAI_EVALUATOR_H

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

#ifndef INCLUDED_GAI_ERRORS_H
#include <gai_errors.h>
#endif /*INCLUDED_GAI_ERRORS_H*/

/* -------------------------------------------------------------------------------------------------------------- */
/*  */
typedef struct gai_evaluator_s gai_evaluator_t;

typedef bool (*gai_evaluator_function_t)(gai_evaluator_t* self, void* ud);

typedef struct gai_evaluator_s{
    char* name;
    gai_evaluator_function_t function;
    void* userdata;
}gai_evaluator_t;

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

gai_err_t gai_evaluator_init(gai_evaluator_t* self, const char* name,
                             gai_evaluator_function_t function,
                             void* ud);

void gai_evaluator_destroy(gai_evaluator_t* self);


#endif /* INCLUDED_GAI_EVALUATOR_H */
