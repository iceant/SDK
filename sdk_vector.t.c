#include <sdk_vector.h>
#include <stdlib.h>
#include <stdio.h>

void vector_print(sdk_vector_t* v){
    for(sdk_size_t i=0; i<v->size; i++){
        printf("[%d] %d\n", i, (int)sdk_vector_get(v, i));
    }
}

int main(int argc, char** argv)
{
    sdk_vector_t v;
    sdk_vector_init(&v, 0);

    for(int i=0; i<256; i++){
        sdk_vector_resize_append(&v, (void*)(i+1));
    }
    vector_print(&v);

    for(int i=0; i<v.size; ){
        sdk_vector_resize_remove(&v, 0);
        printf("capacity:%d size:%d after remove[%d]\n"
               , v.capacity
               , v.size
               , i);
    }

    sdk_vector_destroy(&v);

    return 0;
}