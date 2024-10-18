#ifndef INCLUDED_BASE64_H
#define INCLUDED_BASE64_H

#ifndef INCLUDED_STDDEF_H
#define INCLUDED_STDDEF_H
#include <stddef.h>
#endif /*INCLUDED_STDDEF_H*/

#ifndef INCLUDED_STDINT_H
#define INCLUDED_STDINT_H
#include <stdint.h>
#endif /*INCLUDED_STDINT_H*/



/* -------------------------------------------------------------------------------------------------------------- */
/*  */

size_t base64_encode(const uint8_t in[], uint8_t out[], size_t len, int newline_flag);
size_t base64_decode(const uint8_t in[], uint8_t out[], size_t len);


#endif /*INCLUDED_BASE64_H*/
