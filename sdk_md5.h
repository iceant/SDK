#ifndef INCLUDED_SDK_MD5_H
#define INCLUDED_SDK_MD5_H

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

#ifndef INCLUDED_SDK_TYPES_H
#include <sdk_types.h>
#endif /*INCLUDED_SDK_TYPES_H*/

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

typedef struct sdk_md5_ctx_s{
    uint32_t lo, hi;
    uint32_t a, b, c, d;
    unsigned char buffer[64];
    uint32_t block[16];
} sdk_md5_ctx_t;

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

void sdk_md5_init(sdk_md5_ctx_t *ctx);
void sdk_md5_update(sdk_md5_ctx_t *ctx, const void *data, unsigned long size);
void sdk_md5_final(unsigned char md5[16], sdk_md5_ctx_t *ctx);



#endif /*INCLUDED_SDK_MD5_H*/
