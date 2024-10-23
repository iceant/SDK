#include <gai_knowledge.h>
#include <assert.h>
#include <sdk_memory.h>
#include <sdk_datetime.h>
/* -------------------------------------------------------------------------------------------------------------- */
/*  */

gai_err_t gai_knowledgesource_init(gai_knowledgesource_t * self, gai_knowledge_evaluator_t evaluator, long update_frequency, void* ud){
    assert(self);
    assert(evaluator);

    self->userdata = ud;
    self->confidence = 0;
    self->evaluation = 0;
    self->evaluator = evaluator;
    self->last_update_time = 0;
    self->update_frequency = update_frequency;
    return GAI_ERR_OK;
}

void gai_knowledgesource_destroy(gai_knowledgesource_t * self){
}


gai_knowledge_evaluate_result_t*
gai_knowledgesource_evaluate(gai_knowledgesource_t * self, void* ud, gai_knowledge_evaluate_result_t* result){
    time_t time = sdk_datetime_current_ms();
    time_t next_update_time = self->last_update_time + self->update_frequency;
    if(next_update_time < time){
        self->last_update_time = time;
        result = self->evaluator(self, ud, result);
        self->confidence = result->confidence;
        self->evaluation= result->evaluation;
    }
    result->evaluation = self->evaluation;
    result->confidence = self->confidence;
    return result;
}



