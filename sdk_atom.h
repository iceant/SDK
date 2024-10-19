#ifndef INCLUDED_SDK_ATOM_H
#define INCLUDED_SDK_ATOM_H

#ifndef INCLUDED_SDK_TYPES_H
#include <sdk_types.h>
#endif /*INCLUDED_SDK_TYPES_H*/

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

const char* sdk_atom_str(const char* str);

const char* sdk_atom_int(sdk_int_t n);

const char* sdk_atom_new(const char* str, sdk_size_t str_len);

sdk_size_t sdk_atom_length(const char* str);

#endif /* INCLUDED_SDK_ATOM_H */
