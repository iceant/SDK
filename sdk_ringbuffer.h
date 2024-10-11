#ifndef INCLUDED_SDK_RINGBUFFER_H
#define INCLUDED_SDK_RINGBUFFER_H

#ifndef INCLUDED_SDK_TYPES_H
#include <sdk_types.h>
#endif /*INCLUDED_SDK_TYPES_H*/

#ifndef INCLUDED_SDK_ERRORS_H
#include <sdk_errors.h>
#endif /*INCLUDED_SDK_ERRORS_H*/

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

#define SDK_RINGBUFFER_FIND_INVALID (-1)

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

typedef struct sdk_ringbuffer_s{
    char* buffer;
    sdk_size_t buffer_size;
    sdk_size_t write_idx;
    sdk_size_t read_idx;
}sdk_ringbuffer_t;

/* -------------------------------------------------------------------------------------------------------------- */
/* METHODS */

sdk_err_t sdk_ringbuffer_init(sdk_ringbuffer_t* buf, char* block, sdk_size_t block_size);

void sdk_ringbuffer_reset(sdk_ringbuffer_t * buf);

sdk_bool_t sdk_ringbuffer_is_empty(sdk_ringbuffer_t* buf);

sdk_bool_t sdk_ringbuffer_is_full(sdk_ringbuffer_t* buf);

sdk_size_t sdk_ringbuffer_used(sdk_ringbuffer_t* buf);

sdk_size_t sdk_ringbuffer_unused(sdk_ringbuffer_t* buf);

sdk_err_t sdk_ringbuffer_read(sdk_ringbuffer_t *buf, char* data, sdk_size_t data_size);

sdk_err_t sdk_ringbuffer_write(sdk_ringbuffer_t* buf, char* data, sdk_size_t data_size);

sdk_err_t sdk_ringbuffer_pop(sdk_ringbuffer_t* buf, char* value);

sdk_err_t sdk_ringbuffer_put(sdk_ringbuffer_t* buf, char value);

sdk_err_t sdk_ringbuffer_peek(sdk_ringbuffer_t* buf, sdk_size_t idx, char* value);

sdk_bool_t sdk_ringbuffer_is(sdk_ringbuffer_t* buf, sdk_size_t idx, char value);

sdk_err_t sdk_ringbuffer_advance(sdk_ringbuffer_t * buf, sdk_size_t idx);

unsigned long sdk_ringbuffer_strtoul(sdk_ringbuffer_t* buf, sdk_size_t idx, sdk_size_t* endptr, int base);

int sdk_ringbuffer_find(sdk_ringbuffer_t* buf, sdk_size_t idx, char* data, sdk_size_t data_size);

/* peek */
sdk_err_t sdk_ringbuffer_uint16_le(sdk_ringbuffer_t* buf, sdk_size_t idx, uint16_t* value);
sdk_err_t sdk_ringbuffer_uint16_be(sdk_ringbuffer_t* buf, sdk_size_t idx, uint16_t* value);
sdk_err_t sdk_ringbuffer_uint32_le(sdk_ringbuffer_t* buf, sdk_size_t idx, uint32_t* value);
sdk_err_t sdk_ringbuffer_uint32_be(sdk_ringbuffer_t* buf, sdk_size_t idx, uint32_t* value);

#endif /* INCLUDED_SDK_RINGBUFFER_H */
