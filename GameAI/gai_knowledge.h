#ifndef INCLUDED_GAI_KNOWLEDGE_H
#define INCLUDED_GAI_KNOWLEDGE_H

#ifndef INCLUDED_GAI_ERRORS_H
#include <gai_errors.h>
#endif /*INCLUDED_GAI_ERRORS_H*/

#ifndef INCLUDED_TIME_H
#define INCLUDED_TIME_H
#include <time.h>
#endif /*INCLUDED_TIME_H*/



/* -------------------------------------------------------------------------------------------------------------- */
/*  */

typedef struct gai_knowledgesource_s gai_knowledgesource_t;

typedef struct gai_knowledge_evaluate_result_s{
    int confidence;
    void* evaluation;
}gai_knowledge_evaluate_result_t;

typedef gai_knowledge_evaluate_result_t* (*gai_knowledge_evaluator_t)(gai_knowledgesource_t* self, void* ud, gai_knowledge_evaluate_result_t* result);

struct gai_knowledgesource_s{
    int confidence;
    void* evaluation;
    gai_knowledge_evaluator_t evaluator;
    time_t last_update_time;
    int update_frequency;
    void* userdata;
};



/* -------------------------------------------------------------------------------------------------------------- */
/*  */

gai_err_t gai_knowledgesource_init(gai_knowledgesource_t * self, gai_knowledge_evaluator_t evaluator
                                   , long update_frequency
                                   , void* ud);

void gai_knowledgesource_destroy(gai_knowledgesource_t * self);

gai_knowledge_evaluate_result_t*
gai_knowledgesource_evaluate(gai_knowledgesource_t * self, void* ud, gai_knowledge_evaluate_result_t* result);




#endif /*INCLUDED_GAI_KNOWLEDGE_H*/
