#include <sdk_string_hash.h>
#include <stdio.h>
#include <stdlib.h>
#include <sdk_macros.h>
#include <time.h>

static void print_node(sdk_hashtable_item_t* node, void* arg){
    printf("%s(%08x)=%s\n", (char*)node->key, node->hash, (char*)node->val);
}

static void print_int_node(sdk_hashtable_item_t* node, void* arg){
    printf("%s(%08x)=%"SDK_PRIu"\n", (char*)node->key, node->hash, (sdk_uint_t)node->val);
}

static void test_string_htable(void){
    sdk_hashtable_t t;
    sdk_hashtable_init(&t, 1024, sdk_string_hash_key_ops, sdk_string_hash_val_ops);
    
    char key[32];
    char val[32];
    size_t sz;
    for(int i=0; i<10; i++){
        sz = snprintf(key, SDK_ARRAY_SIZE(key), "name-%d", i);
        key[sz] = '\0';
        sz = snprintf(val, SDK_ARRAY_SIZE(val), "val-%d", i);
        val[sz] = '\0';
        
        sdk_hashtable_put(&t, key, val);
    }
    
    printf("sdk_hashtable_contains(&t, \"name-3\")? %s\n", sdk_hashtable_contains(&t, "name-3")?"TRUE":"FALSE");
    printf("sdk_hashtable_contains(&t, \"name-99\")? %s\n", sdk_hashtable_contains(&t, "name-99")?"TRUE":"FALSE");
    
    printf("remove name-3\n");
    sdk_hashtable_remove(&t, "name-3");
    printf("sdk_hashtable_contains(&t, \"name-3\")? %s\n", sdk_hashtable_contains(&t, "name-3")?"TRUE":"FALSE");
    
    sdk_hashtable_map(&t, print_node, 0);
    
    sdk_hashtable_destroy(&t);
    
}

static void test_int_htable(){
    sdk_hashtable_t t;
    sdk_hashtable_init(&t, 1024, sdk_string_hash_key_ops, sdk_int_hash_val_ops);
    srand(time(NULL));
    
    char key[32];
    size_t sz;
    for(int i=0; i<10; i++){
        sz = snprintf(key, SDK_ARRAY_SIZE(key), "name-%d", i);
        key[sz] = '\0';
        
//        sdk_hashtable_put(&t, key, (void*)(i+rand()));
        sdk_hashtable_put(&t, key, (void*)(i));
    }
    
    printf("sdk_hashtable_contains(&t, \"name-3\")? %s\n", sdk_hashtable_contains(&t, "name-3")?"TRUE":"FALSE");
    printf("sdk_hashtable_contains(&t, \"name-99\")? %s\n", sdk_hashtable_contains(&t, "name-99")?"TRUE":"FALSE");
    
    printf("remove name-3\n");
    sdk_hashtable_remove(&t, "name-3");
    printf("sdk_hashtable_contains(&t, \"name-3\")? %s\n", sdk_hashtable_contains(&t, "name-3")?"TRUE":"FALSE");
    
    
    sdk_hashtable_map(&t, print_int_node, 0);
    
    sdk_hashtable_destroy(&t);
}

int main(int argc, char** argv){
//    test_string_htable();
    test_int_htable();
    
    return 0;
}