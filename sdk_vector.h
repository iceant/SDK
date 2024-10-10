#ifndef INCLUDED_SDK_VECTOR_H
#define INCLUDED_SDK_VECTOR_H

#ifndef INCLUDED_SDK_TYPES_H
#include <sdk_types.h>
#endif // INCLUDED_SDK_TYPES_H

#ifndef INCLUDED_SDK_ERRORS_H
#include <sdk_errors.h>
#endif // INCLUDED_SDK_ERRORS_H

////////////////////////////////////////////////////////////////////////////////
////

typedef struct sdk_vector_s{
    void** items;
    sdk_size_t capacity;
    sdk_size_t size;
}sdk_vector_t;

////////////////////////////////////////////////////////////////////////////////
////

sdk_err_t sdk_vector_init(sdk_vector_t* vector, sdk_size_t capacity);

void sdk_vector_destroy(sdk_vector_t* vector);

sdk_err_t sdk_vector_resize(sdk_vector_t* vector, sdk_size_t capacity);

sdk_err_t sdk_vector_append(sdk_vector_t* vector, void* item);

sdk_err_t sdk_vector_resize_append(sdk_vector_t* vector, void* item);

void* sdk_vector_get(sdk_vector_t* vector, sdk_size_t idx);

sdk_err_t sdk_vector_set(sdk_vector_t* vector, sdk_size_t idx, void* item);

sdk_err_t sdk_vector_resize_remove(sdk_vector_t* vector, sdk_size_t idx);

sdk_err_t sdk_vector_remove(sdk_vector_t* vector, sdk_size_t idx);



#endif /*INCLUDED_SDK_VECTOR_H*/
