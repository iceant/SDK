#ifndef INCLUDED_SDK_STRING_HASH_H
#define INCLUDED_SDK_STRING_HASH_H

#ifndef INCLUDED_SDK_TYPES_H
#include <sdk_types.h>
#endif /*INCLUDED_SDK_TYPES_H*/

#ifndef INCLUDED_SDK_HASHTABLE_H
#include <sdk_hashtable.h>
#endif /*INCLUDED_SDK_HASHTABLE_H*/


/* -------------------------------------------------------------------------------------------------------------- */
/*  */

uint32_t sdk_string_hash(const void* data, void* arg);
void* sdk_string_hash_cp(const void* data, void* arg);
sdk_bool_t sdk_string_hash_eq(const void* data1, const void* data2, void* arg);
void sdk_string_hash_free(void* data, void* arg);

uint32_t sdk_int_hash(const void* data, void* arg);
void* sdk_int_hash_cp(const void* data, void* arg);
sdk_bool_t sdk_int_hash_eq(const void* data1, const void* data2, void* arg);
void sdk_int_hash_free(void* data, void* arg);

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

extern sdk_hashtable_key_ops_t sdk_string_hash_key_ops;
extern sdk_hashtable_val_ops_t sdk_string_hash_val_ops;

extern sdk_hashtable_key_ops_t sdk_int_hash_key_ops;
extern sdk_hashtable_val_ops_t sdk_int_hash_val_ops;

#endif /* INCLUDED_SDK_STRING_HASH_H */
