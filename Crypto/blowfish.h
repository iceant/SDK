#ifndef INCLUDED_BLOWFISH_H
#define INCLUDED_BLOWFISH_H

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

#ifndef INCLUDED_STDINT_H
#define INCLUDED_STDINT_H
#include <stdint.h>
#endif /*INCLUDED_STDINT_H*/

#ifndef INCLUDED_STDDEF_H
#define INCLUDED_STDDEF_H
#include <stddef.h>
#endif /*INCLUDED_STDDEF_H*/

#ifndef INCLUDED_STRING_H
#define INCLUDED_STRING_H
#include <string.h>
#endif /*INCLUDED_STRING_H*/


/* -------------------------------------------------------------------------------------------------------------- */
/*  */

#define BLOWFISH_BLOCK_SIZE 8           // Blowfish operates on 8 bytes at a time

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

typedef struct {
    uint32_t p[18];
    uint32_t s[4][256];
} BLOWFISH_KEY;

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

void blowfish_key_setup(const uint8_t user_key[], BLOWFISH_KEY *keystruct, size_t len);
void blowfish_encrypt(const uint8_t in[], uint8_t out[], const BLOWFISH_KEY *keystruct);
void blowfish_decrypt(const uint8_t in[], uint8_t out[], const BLOWFISH_KEY *keystruct);








#endif /*INCLUDED_BLOWFISH_H*/
