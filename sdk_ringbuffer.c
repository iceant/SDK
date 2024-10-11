#include <sdk_ringbuffer.h>
#include <assert.h>
#include <sdk_types.h>

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

#ifndef ULONG_MAX
#define	ULONG_MAX	((unsigned long)(~0L))		/* 0xFFFFFFFF */
#endif

static sdk_size_t sdk_ringbuffer__get_idx(sdk_ringbuffer_t * buf, sdk_size_t  idx){
#if 0
    sdk_size_t read_idx = buf->read_idx ;
    for(sdk_size_t i=0; i<idx; i++){
        read_idx+=1;
        if(read_idx >= buf->buffer_size){
            read_idx = 0;
        }
    }
    return read_idx;
#else
    return ( buf->read_idx + idx ) % buf->buffer_size;
#endif
}

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

sdk_err_t sdk_ringbuffer_init(sdk_ringbuffer_t* buf, char* block, sdk_size_t block_size){
    assert(buf);
    assert(block);
    assert(block_size>0);
    
    buf->buffer = block;
    buf->buffer_size = block_size;
    buf->write_idx = 0;
    buf->read_idx= 0;
    
    return SDK_ERR_OK;
}

void sdk_ringbuffer_reset(sdk_ringbuffer_t * buf){
    assert(buf);
    buf->write_idx = 0;
    buf->read_idx = 0;
}

sdk_err_t sdk_ringbuffer_put(sdk_ringbuffer_t* buf, char value){
    sdk_size_t write_idx = buf->write_idx + 1;
    if(write_idx >= buf->buffer_size){
        write_idx = 0;
    }
    if(write_idx == buf->read_idx){
        return SDK_ERR_FULL;
    }
    buf->buffer[buf->write_idx] = value;
    buf->write_idx = write_idx;
    return SDK_ERR_OK;
}

sdk_err_t sdk_ringbuffer_pop(sdk_ringbuffer_t* buf, char* value){
    if(buf->write_idx == buf->read_idx){
        return SDK_ERR_EMPTY;
    }
    if(value){
        *value = buf->buffer[buf->read_idx];
    }
    sdk_size_t read_idx = buf->read_idx + 1;
    if(read_idx >= buf->buffer_size){
        read_idx = 0;
    }
    buf->read_idx = read_idx;
    return SDK_ERR_OK;
}

sdk_err_t sdk_ringbuffer_write(sdk_ringbuffer_t* buf, char* data, sdk_size_t data_size){
    sdk_err_t err = SDK_ERR_OK;
    for(sdk_size_t i=0; i<data_size; i++){
        err = sdk_ringbuffer_put(buf, *data++);
        if(err!=SDK_ERR_OK){
            return err;
        }
    }
    return err;
}

sdk_err_t sdk_ringbuffer_read(sdk_ringbuffer_t *buf, char* data, sdk_size_t data_size){
    sdk_err_t  err;
    sdk_size_t i;
    
    if(sdk_ringbuffer_is_empty(buf)){
        return SDK_ERR_EMPTY;
    }
    
    for(i=0; i<data_size; i++){
        err = sdk_ringbuffer_pop(buf, data++);
        if(err!=SDK_ERR_OK){
            break;
        }
    }
    
    return SDK_ERR_OK;
}

sdk_bool_t sdk_ringbuffer_is_empty(sdk_ringbuffer_t* buf){
    return (buf->write_idx==buf->read_idx)?SDK_TRUE:SDK_FALSE;
}

sdk_bool_t sdk_ringbuffer_is_full(sdk_ringbuffer_t* buf){
    sdk_size_t write_idx = buf->write_idx + 1;
    if(write_idx >= buf->buffer_size){
        write_idx = 0;
    }
    return (write_idx == buf->read_idx)?SDK_TRUE:SDK_FALSE;
}

/*
 0 1 2 3 4 5 6
   |     | - w
   |- r
   
   
 0 1 2 3 4 5 6
   |     | - r
   |- w
 */
sdk_size_t sdk_ringbuffer_used(sdk_ringbuffer_t* buf){
    sdk_size_t write_idx = buf->write_idx;
    sdk_size_t read_idx = buf->read_idx;
    if(write_idx >= read_idx){
        return (write_idx - read_idx);
    }else{
        return (buf->buffer_size - read_idx + write_idx);
    }
}

sdk_size_t sdk_ringbuffer_unused(sdk_ringbuffer_t* buf){
    return buf->buffer_size - (sdk_ringbuffer_used(buf)) - 1;
}


sdk_err_t sdk_ringbuffer_peek(sdk_ringbuffer_t* buf, sdk_size_t idx, char* value){
    assert(buf);
    if(sdk_ringbuffer_is_empty(buf)){
        return SDK_ERR_EMPTY;
    }
    sdk_size_t used = sdk_ringbuffer_used(buf);
    if(idx >= used){
        return SDK_ERR_INVAL;
    }
    
    if(value){
        *value = buf->buffer[sdk_ringbuffer__get_idx(buf, idx)];
    }
    
    return SDK_ERR_OK;
}

sdk_bool_t sdk_ringbuffer_is(sdk_ringbuffer_t* buf, sdk_size_t idx, char value){
    assert(buf);
    if(sdk_ringbuffer_is_empty(buf)){
        return SDK_ERR_EMPTY;
    }
    sdk_size_t used = sdk_ringbuffer_used(buf);
    if(idx >= used){
        return SDK_ERR_INVAL;
    }
    
    return (value==buf->buffer[sdk_ringbuffer__get_idx(buf, idx)])?SDK_TRUE:SDK_FALSE;
}

/* move read_idx advance */
sdk_err_t sdk_ringbuffer_advance(sdk_ringbuffer_t * buf, sdk_size_t idx){
    assert(buf);
    if(sdk_ringbuffer_is_empty(buf)){
        return SDK_ERR_EMPTY;
    }
    sdk_size_t used = sdk_ringbuffer_used(buf);
    if(idx >= used){
        return SDK_ERR_INVAL;
    }
    buf->read_idx = sdk_ringbuffer__get_idx(buf, idx);
    return SDK_ERR_OK;
}


unsigned long sdk_ringbuffer_strtoul(sdk_ringbuffer_t* buf, sdk_size_t idx, sdk_size_t* endptr, register int base){
    assert(buf);
    
    if(sdk_ringbuffer_is_empty(buf)){
        return 0;
    }
    
    int c=0;
    register sdk_size_t s = idx;
    register unsigned long acc;
    register unsigned long cutoff;
    register int neg = 0, any, cutlim;
    
    /*
     * See strtol for comments as to the logic used.
     */
    do {
        sdk_ringbuffer_peek(buf, s++, (char*)(&c));
    } while (isspace(c));
    if (c == '-') {
        neg = 1;
        sdk_ringbuffer_peek(buf, s++, (char*)(&c));
    } else if (c == '+'){
        sdk_ringbuffer_peek(buf, s++, (char*)(&c));
    }
    if ((base == 0 || base == 16) &&
        c == '0' && (sdk_ringbuffer_is(buf, s, 'x') || sdk_ringbuffer_is(buf, s, 'X'))) {
        sdk_ringbuffer_peek(buf, s+1, (char*)(&c));
        s += 2;
        base = 16;
    }
    if (base == 0)
        base = c == '0' ? 8 : 10;
    cutoff = (unsigned long)ULONG_MAX / (unsigned long)base;
    cutlim = (unsigned long)ULONG_MAX % (unsigned long)base;
    for (acc = 0, any = 0;; sdk_ringbuffer_peek(buf, s, (char*)(&c)), s++) {
        if (isdigit(c))
            c -= '0';
        else if (isalpha(c))
            c -= isupper(c) ? 'A' - 10 : 'a' - 10;
        else
            break;
        if (c >= base)
            break;
        if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
            any = -1;
        else {
            any = 1;
            acc *= base;
            acc += c;
        }
    }
    if (any < 0) {
        acc = ULONG_MAX;
    } else if (neg)
        acc = -acc;
    if (endptr != 0)
        *endptr = (sdk_size_t) (any ? s - 1 : idx);
    return (acc);
}

int sdk_ringbuffer_find(sdk_ringbuffer_t* buf, sdk_size_t idx, char* data, sdk_size_t data_size){
    assert(buf);
    
    if(sdk_ringbuffer_is_empty(buf)){
        return SDK_RINGBUFFER_FIND_INVALID;
    }
    sdk_size_t used = sdk_ringbuffer_used(buf);
    if(idx >= used){
        return SDK_RINGBUFFER_FIND_INVALID;
    }
    
    if(data_size > (used - idx)){
        return SDK_RINGBUFFER_FIND_INVALID;
    }
    sdk_size_t j;
    for(sdk_size_t i=idx; i<used; i++){
        for(j=0; j<data_size && ((i+j) < used); j++){
            if(!sdk_ringbuffer_is(buf, i+j, data[j])){
                break;
            }
        }
        if(j==data_size){
            return (int)i;
        }
    }
    return SDK_RINGBUFFER_FIND_INVALID;
}

sdk_err_t sdk_ringbuffer_uint16_le(sdk_ringbuffer_t* buf, sdk_size_t idx, uint16_t* value){
    assert(buf);
    uint8_t v[2];
    
    if(sdk_ringbuffer_is_empty(buf)){
        return SDK_ERR_EMPTY;
    }
    sdk_size_t used = sdk_ringbuffer_used(buf);
    if(idx >= used){
        return SDK_ERR_INVAL;
    }
    v[0] = buf->buffer[sdk_ringbuffer__get_idx(buf, idx)];
    v[1] = buf->buffer[sdk_ringbuffer__get_idx(buf, idx+1)];
    if(value){
        *value= ((v[1] << 8) | (v[0] & 0xFF));
    }
    
    return SDK_ERR_OK;
}

sdk_err_t sdk_ringbuffer_uint16_be(sdk_ringbuffer_t* buf, sdk_size_t idx, uint16_t* value){
    assert(buf);
    uint8_t v[2];
    
    if(sdk_ringbuffer_is_empty(buf)){
        return SDK_ERR_EMPTY;
    }
    sdk_size_t used = sdk_ringbuffer_used(buf);
    if(idx >= used){
        return SDK_ERR_INVAL;
    }
    v[0] = buf->buffer[sdk_ringbuffer__get_idx(buf, idx)];
    v[1] = buf->buffer[sdk_ringbuffer__get_idx(buf, idx+1)];
    if(value){
        *value= ((v[0] << 8) | (v[1] & 0xFF));
    }
    
    return SDK_ERR_OK;
}

sdk_err_t sdk_ringbuffer_uint32_le(sdk_ringbuffer_t* buf, sdk_size_t idx, uint32_t* value){
    assert(buf);
    uint8_t v[4];
    
    if(sdk_ringbuffer_is_empty(buf)){
        return SDK_ERR_EMPTY;
    }
    sdk_size_t used = sdk_ringbuffer_used(buf);
    if(idx >= used){
        return SDK_ERR_INVAL;
    }
    v[0] = buf->buffer[sdk_ringbuffer__get_idx(buf, idx++)];
    v[1] = buf->buffer[sdk_ringbuffer__get_idx(buf, idx++)];
    v[2] = buf->buffer[sdk_ringbuffer__get_idx(buf, idx++)];
    v[3] = buf->buffer[sdk_ringbuffer__get_idx(buf, idx++)];
    
    if(value){
        *value= ((v[3] << 24) | (v[2] << 16) | (v[1] << 8) | (v[0] & 0xFF));
    }
    
    return SDK_ERR_OK;
}

sdk_err_t sdk_ringbuffer_uint32_be(sdk_ringbuffer_t* buf, sdk_size_t idx, uint32_t* value){
    assert(buf);
    uint8_t v[4];
    
    if(sdk_ringbuffer_is_empty(buf)){
        return SDK_ERR_EMPTY;
    }
    sdk_size_t used = sdk_ringbuffer_used(buf);
    if(idx >= used){
        return SDK_ERR_INVAL;
    }
    v[0] = buf->buffer[sdk_ringbuffer__get_idx(buf, idx++)];
    v[1] = buf->buffer[sdk_ringbuffer__get_idx(buf, idx++)];
    v[2] = buf->buffer[sdk_ringbuffer__get_idx(buf, idx++)];
    v[3] = buf->buffer[sdk_ringbuffer__get_idx(buf, idx++)];
    
    if(value){
        *value= ((v[0] << 24) | (v[1] << 16) | (v[2] << 8) | (v[3] & 0xFF));
    }
    
    return SDK_ERR_OK;
}
