#include <sdk_memory.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){
    
    printf("sdk_memory_init()\n");
    sdk_memory_init();
    
    printf("sdk_memory_alloc(100)\n");
    void* m100 = sdk_memory_alloc(100);
    
    printf("sdk_memory_free(100)\n");
    sdk_memory_free(m100);
    
    printf("sdk_memory_destroy()\n");
    sdk_memory_destroy();
    return 0;
}