#include <sdk_bitset.h>
#include <stdio.h>
#include <stdlib.h>
#include <sdk_types.h>
#include <sdk_memory.h>
#include <sdk_fmt.h>
#include <assert.h>

int main(int argc, char** argv){
    sdk_memory_init();
    sdk_bitset_t* set;
    printf("sdk_bitset_new(1024)\n");
    set = sdk_bitset_new(1024);
    
    printf("sdk_bitset_length(set)=%"SDK_PRId"\n", sdk_bitset_length(set));
    printf("sdk_bitset_count(set)=%"SDK_PRId"\n", sdk_bitset_count(set));
    
    printf("sdk_bitset_put(set, 2, 1)\n");
    sdk_bitset_put(set, 2, 1);
    
    printf("sdk_bitset_count(set)=%"SDK_PRId"\n", sdk_bitset_count(set));
    
    printf("sdk_bitset_get(set, 2)=%"SDK_PRId"\n", sdk_bitset_get(set, 2));
    printf("sdk_bitset_get(set, 4)=%"SDK_PRId"\n", sdk_bitset_get(set, 4));
    printf("sdk_bitset_not(set, 2, 5)\n");
    sdk_bitset_not(set, 2, 5);
    printf("sdk_bitset_get(set, 2)=%"SDK_PRId"\n", sdk_bitset_get(set, 2));
    printf("sdk_bitset_get(set, 4)=%"SDK_PRId"\n", sdk_bitset_get(set, 4));
    
    sdk_fmt_register('B', sdk_bitset_fmt);
    
    sdk_fmt_print("%B", set);
    
    sdk_bitset_free(&set);
    sdk_memory_destroy();
    
    return 0;
}