#ifndef INCLUDED_SHA1_H
#define INCLUDED_SHA1_H

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
#define SHA1_BLOCK_SIZE 20              // SHA1 outputs a 20 byte digest

/* -------------------------------------------------------------------------------------------------------------- */
/*  */
typedef struct {
    uint8_t data[64];
    uint32_t datalen;
    unsigned long long bitlen;
    uint32_t state[5];
    uint32_t k[4];
} SHA1_CTX;

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

void sha1_init(SHA1_CTX *ctx);
void sha1_update(SHA1_CTX *ctx, const uint8_t data[], size_t len);
void sha1_final(SHA1_CTX *ctx, uint8_t hash[]);




#endif /*INCLUDED_SHA1_H*/
