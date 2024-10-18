#include "sdk.h"


void sdk_init(void){
    sdk_memory_init();
    sdk_net_init();
}

void sdk_destroy(void){
    sdk_net_destroy();
    sdk_memory_destroy();
}
