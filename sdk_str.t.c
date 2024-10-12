#include <sdk_str.h>
#include <stdio.h>
#include <stdlib.h>
#include <sdk_memory.h>

#define ARRAY_SIZE(A) (sizeof(A)/sizeof(A[0]))

static char string[257]={0};

int main(int argc, char** argv){
//    sdk_memory_init();
    
    memset(string, 0, ARRAY_SIZE(string));
    
    for(int i=0; i< ('~' - '!' + 1); i++){
        string[i] = (char)(i + 33);
    }
    
    for(int i=0; i< ARRAY_SIZE(string); i++){
        printf("[%d] %c ", i, string[i]);
    }
    printf("\n");
    
    char* sub = sdk_str_sub(string, 0, -5);
    printf("sdk_str_sub(string, 0, -5) => %s\n", sub);
    sdk_str_free(sub);
    
    char* dup = sdk_str_dup(string, 12, 33, 2);
    printf("sdk_str_dup(string, 12, 33, 2) => %s\n", dup);
    sdk_str_free(dup);

    char* str1 = "This is Str1";
    char* str2 = "This is Str2";

    char* cat = sdk_str_cat(str1, 1, 0, str2, 0, -5);
    printf("sdk_str_cat(str1, 1, 0, str2, 0, -5): %s\n", cat);
    sdk_str_free(cat);
    
    char* catv = sdk_str_catv("This is", 1,0
                              , " str1", 1, 0
                              , " str2", 1, 0
                              , " str3", 1, 0
                              , NULL);
    printf("sdk_str_catv(\"This is\", \" str1\", \" str2\", \" str3\", NULL) => %s\n", catv);
    sdk_str_free(catv);
    
    char* Reversed = sdk_str_reverse("Reversed", 1, 0);
    printf("sdk_str_reverse(\"Reversed\", 1, 0) = %s\n", Reversed);
    sdk_str_free(Reversed);
    
    char* map = sdk_str_map("I'm Your Father!", 1, 0, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", "abcdefghijklmnopqrstuvwxyz");
    printf("sdk_str_map(\"I'm Your Father!\", 1, 0, \"ABCDEFGHIJKLMNOPQRSTUVWXYZ\", \"abcdefghijklmnopqrstuvwxyz\") = %s\n", map);
    sdk_str_free(map);
    
    
//    sdk_memory_destroy();
    
    return 0;
}