#ifndef INCLUDED_SHA256_H
#define INCLUDED_SHA256_H

#ifndef INCLUDED_STDINT_H
#define INCLUDED_STDINT_H
#include <stdint.h>
#endif /*INCLUDED_STDINT_H*/

#ifndef INCLUDED_STDDEF_H
#define INCLUDED_STDDEF_H
#include <stddef.h>
#endif /*INCLUDED_STDDEF_H*/

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

#define SHA256_BLOCK_SIZE 32            // SHA256 outputs a 32 byte digest

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

typedef struct {
    uint8_t data[64];
    uint32_t datalen;
    unsigned long long bitlen;
    uint32_t state[8];
} SHA256_CTX;

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

void sha256_init(SHA256_CTX *ctx);
void sha256_update(SHA256_CTX *ctx, const uint8_t data[], size_t len);
void sha256_final(SHA256_CTX *ctx, uint8_t hash[]);




#endif /*INCLUDED_SHA256_H*/
