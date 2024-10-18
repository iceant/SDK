#ifndef INCLUDED_SDK_SOCKET_H
#define INCLUDED_SDK_SOCKET_H

#if defined(_WIN32)
    #ifndef _WIN32_WINNT
    #define _WIN32_WINNT 0x0600
    #endif

    #ifndef INCLUDED_WINSOCK2_H
    #define INCLUDED_WINSOCK2_H
    #include <winsock2.h>
    #endif /*INCLUDED_WINSOCK2_H*/

    #ifndef INCLUDED_WS2TCPIP_H
    #define INCLUDED_WS2TCPIP_H
    #include <ws2tcpip.h>
    #endif /*INCLUDED_WS2TCPIP_H*/

    #pragma comment(lib, "ws2_32.lib")

    typedef SOCKET                      sdk_socket_t;
    #define SDK_SOCKET_ISVALID(S)       ((S)!=INVALID_SOCKET)
    #define SDK_SOCKET_CLOSE(S)         closesocket((S))
    #define SDK_SOCKET_ERRNO()          (WSAGetLastError())
#else
    #ifndef INCLUDED_SYS_TYPES_H
    #define INCLUDED_SYS_TYPES_H
    #include <sys/types.h>
    #endif /*INCLUDED_SYS_TYPES_H*/

    #ifndef INCLUDED_SYS_SOCKET_H
    #define INCLUDED_SYS_SOCKET_H
    #include <sys/socket.h>
    #endif /*INCLUDED_SYS_SOCKET_H*/

    #ifndef INCLUDED_NETINET_IN_H
    #define INCLUDED_NETINET_IN_H
    #include <netinet/in.h>
    #endif /*INCLUDED_NETINET_IN_H*/

    #ifndef INCLUDED_ARPA_INET_H
    #define INCLUDED_ARPA_INET_H
    #include <arpa/inet.h>
    #endif /*INCLUDED_ARPA_INET_H*/

    #ifndef INCLUDED_NETDB_H
    #define INCLUDED_NETDB_H
    #include <netdb.h>
    #endif /*INCLUDED_NETDB_H*/

    #ifndef INCLUDED_UNISTD_H
    #define INCLUDED_UNISTD_H
    #include <unistd.h>
    #endif /*INCLUDED_UNISTD_H*/

    #ifndef INCLUDED_ERRNO_H
    #define INCLUDED_ERRNO_H
    #include <errno.h>
    #endif /*INCLUDED_ERRNO_H*/

    typedef int                         sdk_socket_t;
    #define SDK_SOCKET_ISVALID(S)       ((S) >= 0)
    #define SDK_SOCKET_CLOSE(S)         close((S))
    #define SDK_SOCKET_ERRNO()          (errno)
#endif

#ifndef INCLUDED_SDK_ERRORS_H
#include <sdk_errors.h>
#endif /*INCLUDED_SDK_ERRORS_H*/

#ifndef INCLUDED_STRING_H
#define INCLUDED_STRING_H
#include <string.h>
#endif /*INCLUDED_STRING_H*/


#ifndef INCLUDED_STDIO_H
#define INCLUDED_STDIO_H
#include <stdio.h>
#endif /*INCLUDED_STDIO_H*/

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

#if !defined(IPV6_V6ONLY)
#define IPV6_V6ONLY 27
#endif

/* -------------------------------------------------------------------------------------------------------------- */
/*  */
sdk_err_t sdk_socket_init(void);

void sdk_socket_destroy(void);




#endif /*INCLUDED_SDK_SOCKET_H*/
