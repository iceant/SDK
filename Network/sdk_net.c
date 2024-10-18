#include <sdk_net.h>
#include <sdk_compiler.h>

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

static sdk_bool_t sdk_net_module__init_flag = SDK_FALSE;

C_CONSTRUCTOR
static void sdk_net_module_init(void){
    if(sdk_net_module__init_flag){
        return;
    }
    sdk_net_module__init_flag = SDK_TRUE;
    sdk_net_init();
}

C_DESTRUCTOR
static void sdk_net_module_destroy(void){
    sdk_net_destroy();
}


/* -------------------------------------------------------------------------------------------------------------- */
/*  */

void sdk_net_init(void){
    sdk_socket_init();
}

void sdk_net_destroy(void){
    sdk_socket_destroy();
}
