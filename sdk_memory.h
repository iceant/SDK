#ifndef INCLUDED_SDK_MEMORY_H
#define INCLUDED_SDK_MEMORY_H

#ifndef INCLUDED_SDK_TYPES_H
#include <sdk_types.h>
#endif // INCLUDED_SDK_TYPES_H

#ifndef INCLUDED_STRING_H
#define INCLUDED_STRING_H
#include <string.h>
#endif // INCLUDED_STRING_H

////////////////////////////////////////////////////////////////////////////////
////

void* sdk_memory_alloc(sdk_size_t nBytes);

void* sdk_memory_calloc(sdk_size_t nCount, sdk_size_t nBytes);

void* sdk_memory_realloc(void* memory, sdk_size_t nBytes);

void sdk_memory_free(void* memory);

////////////////////////////////////////////////////////////////////////////////
////
#define SDK_ALLOC(x) sdk_memory_alloc((x))
#define SDK_CALLOC(n, x) sdk_memory_calloc((n), (x))
#define SDK_REALLOC(p, x) sdk_memory_realloc((p), (x))
#define SDK_RESIZE(p, x) ((p)=SDK_REALLOC(p, x))
#define SDK_FREE(p) (sdk_memory_free((p)), (p)=0)

#define SDK_NEW(p) ((p)=SDK_ALLOC(sizeof(*(p))))
#define SDK_NEW0(p) ((p)=SDK_CALLOC(1, sizeof(*(p))))

#endif /*INCLUDED_SDK_MEMORY_H*/
