#include <sdk_ring.h>
#include <assert.h>
#include <sdk_macros.h>
#include <sdk_memory.h>
/* -------------------------------------------------------------------------------------------------------------- */
/*  */

sdk_err_t sdk_ring_init(sdk_ring_t * ring, uint8_t * buffer, sdk_size_t buffer_size, sdk_size_t object_size)
{
    assert(ring);
    assert(buffer);
    assert(buffer_size>0);
    assert(object_size>0);
    assert(buffer_size>object_size);

    ring->capacity = (sdk_size_t)(buffer_size/object_size);
    ring->objects = (void**) buffer;
    ring->object_size = object_size;
    ring->read_idx = 0;
    ring->write_idx = 0;

    return SDK_ERR_OK;
}

void sdk_ring_reset(sdk_ring_t * ring){
    ring->read_idx = 0;
    ring->write_idx = 0;
}

sdk_err_t sdk_ring_put(sdk_ring_t * ring, void* object){
    assert(ring);
    assert(object);

    sdk_size_t write_idx = ring->write_idx + 1;
    sdk_size_t read_idx = ring->read_idx;
    if(write_idx>=ring->capacity){
        write_idx = 0;
    }
    if(write_idx==read_idx){
        return SDK_ERR_FULL;
    }

    memcpy(ring->objects + ring->write_idx * ring->object_size, object, ring->object_size);
    ring->write_idx = write_idx;

    return SDK_ERR_OK;
}

void* sdk_ring_get_put_slot(sdk_ring_t * ring){
    assert(ring);

    sdk_size_t write_idx = ring->write_idx + 1;
    sdk_size_t read_idx = ring->read_idx;
    if(write_idx>=ring->capacity){
        write_idx = 0;
    }

    if(write_idx==read_idx){
        return 0;
    }

    void* object = ring->objects + ring->write_idx * ring->object_size;
    ring->write_idx = write_idx;

    return object;
}

sdk_err_t sdk_ring_pop_to(sdk_ring_t * ring, void** object){
    assert(ring);
    sdk_size_t read_idx = ring->read_idx;
    if(read_idx==ring->write_idx){
        return SDK_ERR_EMPTY;
    }
    if(object){
        memcpy(*object, ring->objects + read_idx * ring->object_size, ring->object_size);
    }
    read_idx += 1;
    if(read_idx>=ring->capacity){
        read_idx = 0;
    }
    ring->read_idx = read_idx;
    return SDK_ERR_OK;
}

void* sdk_ring_pop(sdk_ring_t * ring){
    assert(ring);
    sdk_size_t read_idx = ring->read_idx;
    if(read_idx==ring->write_idx){
        /* empty */
        return 0;
    }
    void* object = ring->objects + read_idx * ring->object_size;
    read_idx += 1;
    if(read_idx>=ring->capacity){
        read_idx = 0;
    }
    ring->read_idx = read_idx;
    return object;
}

void* sdk_ring_peek(sdk_ring_t * ring, sdk_size_t idx){
    assert(ring);
    assert(idx <= sdk_ring_size(ring));

    sdk_size_t i;
    sdk_size_t read_idx = ring->read_idx;

    if(read_idx==ring->write_idx){
        /* empty */
        return 0;
    }

    for(i=0; i<idx;i++){
        read_idx+=1;
        if(read_idx >= ring->capacity){
            read_idx = 0;
        }
    }

    void* object = ring->objects + read_idx * ring->object_size;
    return object;
}

sdk_err_t sdk_ring_peek_to(sdk_ring_t * ring, sdk_size_t idx, void** object){
    assert(ring);
    assert(idx <= sdk_ring_size(ring));

    sdk_size_t i;
    sdk_size_t read_idx = ring->read_idx;
    if(read_idx==ring->write_idx){
        /* empty */
        return SDK_ERR_EMPTY;
    }

    for(i=0; i<idx;i++){
        read_idx+=1;
        if(read_idx >= ring->capacity){
            read_idx = 0;
        }
    }

    if(object){
        memcpy(*object, ring->objects + read_idx * ring->object_size, ring->object_size);
    }
    return SDK_ERR_OK;
}

sdk_bool_t sdk_ring_is_full(sdk_ring_t * ring){
    sdk_size_t write_idx = ring->write_idx + 1;
    if(write_idx>=ring->capacity){
        write_idx = 0;
    }
    return (write_idx == ring->read_idx)?(SDK_TRUE):(SDK_FALSE);
}

sdk_bool_t sdk_ring_is_empty(sdk_ring_t * ring){
    return (ring->write_idx == ring->read_idx)?(SDK_TRUE):(SDK_FALSE);
}

sdk_size_t sdk_ring_size(sdk_ring_t * ring){
    assert(ring);
    sdk_size_t size = ring->capacity - 1;
    if(!sdk_ring_is_full(ring)){
        if(ring->write_idx >= ring->read_idx){
            size = (ring->write_idx - ring->read_idx);
        }else{
            size = (ring->capacity + ring->write_idx - ring->read_idx);
        }
    }
    return size;
}