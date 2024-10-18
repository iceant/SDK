#include <sdk_socket.h>

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

#if defined(WIN32)

sdk_err_t sdk_socket_init(void){
    WSADATA WsaData;
    if(WSAStartup(MAKEWORD(2,2), &WsaData)){
        return SDK_ERR_ERROR;
    }
    return SDK_ERR_OK;
}

void sdk_socket_destroy(void){
    WSACleanup();
}
#else
sdk_err_t sdk_socket_init(void){
    return SDK_ERR_OK;
}

void sdk_socket_destroy(void){

}
#endif