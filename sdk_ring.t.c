#include <sdk_ring.h>
#include <stdlib.h>
#include <stdio.h>
#include <sdk_macros.h>

typedef struct object_s{
    char name[32];
    int age;
}object_t;

#define OBJECT_SIZE sizeof(object_t)
#define OBJECT_COUNT 10

#define BUFFER_SIZE (OBJECT_COUNT * OBJECT_SIZE)

static uint8_t buffer[BUFFER_SIZE];
static char name_buf[32];

int main(int argc, char** argv){

    sdk_ring_t ring;
    sdk_ring_init(&ring, buffer, BUFFER_SIZE, OBJECT_SIZE);

    for(int i=0; i<ring.capacity; i++){
        object_t* object = sdk_ring_get_put_slot(&ring);
        if(!object){
            break;
        }
        snprintf(object->name, SDK_ARRAY_SIZE(object->name), "name_%d", i);
        object->age = 30 + i;
    }

    sdk_size_t size = sdk_ring_size(&ring);
    for(int i=0; i<size; i++){
        object_t * p = sdk_ring_pop(&ring);
        if(p){
            printf("object[%d]: (%s) (%d)\n", i, p->name, p->age);
        }
    }

    printf("ring.size=%"SDK_PRId"\n", sdk_ring_size(&ring));

    object_t obj;
    for(int i=0; i<ring.capacity; i++){
        snprintf(obj.name, SDK_ARRAY_SIZE(obj.name), "name_%d", i+10);
        obj.age = 50+i;
        sdk_ring_put(&ring, &obj);
    }
    size = sdk_ring_size(&ring);
    for(int i=0; i<size; i++){
        object_t * p = sdk_ring_peek(&ring, i);
        if(p){
            printf("peek object[%d]: (%s) (%d)\n", i, p->name, p->age);
        }
    }
    printf("ring.size=%"SDK_PRId"\n", sdk_ring_size(&ring));

    return 0;
}