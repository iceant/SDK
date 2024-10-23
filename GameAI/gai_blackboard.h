#ifndef INCLUDED_GAI_BLACKBOARD_H
#define INCLUDED_GAI_BLACKBOARD_H

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

#ifndef INCLUDED_GAI_ERRORS_H
#include <gai_errors.h>
#endif /*INCLUDED_GAI_ERRORS_H*/

#ifndef INCLUDED_SDK_HASHTABLE_H
#include <sdk_hashtable.h>
#endif /*INCLUDED_SDK_HASHTABLE_H*/

#ifndef INCLUDED_SDK_VECTOR_H
#include <sdk_vector.h>
#endif /*INCLUDED_SDK_VECTOR_H*/

#ifndef INCLUDED_GAI_KNOWLEDGE_H
#include <gai_knowledge.h>
#endif /*INCLUDED_GAI_KNOWLEDGE_H*/


/* -------------------------------------------------------------------------------------------------------------- */
/*  */

typedef struct gai_blackboard_s{
    sdk_hashtable_t attributes;
    sdk_hashtable_t sources;
    void* userdata;
}gai_blackboard_t;


/* -------------------------------------------------------------------------------------------------------------- */
/*  */

gai_err_t gai_blackboard_init(gai_blackboard_t * self, void* userdata);

void gai_blackboard_destroy(gai_blackboard_t * self);

void* gai_blackboard_get(gai_blackboard_t *self, const char* attribute, void* ud);

gai_err_t gai_blackboard_set(gai_blackboard_t * self, const char* attribute, void* value);

gai_err_t gai_blackboard_remove_source(gai_blackboard_t *self, const char* attribute, gai_knowledgesource_t* source);

gai_err_t gai_blackboard_add_source(gai_blackboard_t * self, const char* attribute, gai_knowledgesource_t* source);


#endif /*INCLUDED_GAI_BLACKBOARD_H*/
