#include "sdk_socket.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){
    sdk_socket_init();

    printf("Configuring local address...\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo *bind_address;
    getaddrinfo(0, "8090", &hints, &bind_address);


    printf("Creating socket...\n");
    sdk_socket_t socket_listen;
    socket_listen = socket(bind_address->ai_family,
                           bind_address->ai_socktype, bind_address->ai_protocol);
    if (!SDK_SOCKET_ISVALID(socket_listen)) {
        fprintf(stderr, "socket() failed. (%d)\n", SDK_SOCKET_ERRNO());
        return 1;
    }

    printf("Binding socket to local address...\n");
    if (bind(socket_listen,
             bind_address->ai_addr, bind_address->ai_addrlen)) {
        fprintf(stderr, "bind() failed. (%d)\n", SDK_SOCKET_ERRNO());
        return 1;
    }
    freeaddrinfo(bind_address);

    printf("Listening...\n");
    if (listen(socket_listen, 10) < 0) {
        fprintf(stderr, "listen() failed. (%d)\n", SDK_SOCKET_ERRNO());
        return 1;
    }


    fd_set master;
    FD_ZERO(&master);
    FD_SET(socket_listen, &master);
    sdk_socket_t max_socket = socket_listen;

    printf("Waiting for connections...\n");

    while(1) {
        fd_set reads;
        reads = master;
        if (select(max_socket+1, &reads, 0, 0, 0) < 0) {
            fprintf(stderr, "select() failed. (%d)\n", SDK_SOCKET_ERRNO());
            return 1;
        }
        SOCKET i;
        for(i = 1; i <= max_socket; ++i) {
            if (FD_ISSET(i, &reads)) {
                //Handle socket
                if (i == socket_listen) {
                    struct sockaddr_storage client_address;
                    socklen_t client_len = sizeof(client_address);
                    SOCKET socket_client = accept(socket_listen,
                                                  (struct sockaddr *) &client_address,
                                                  &client_len);
                    if (!SDK_SOCKET_ISVALID(socket_client)) {
                        fprintf(stderr, "accept() failed. (%d)\n",
                                SDK_SOCKET_ERRNO());
                        return 1;
                    }

                    FD_SET(socket_client, &master);
                    if (socket_client > max_socket)
                        max_socket = socket_client;

                    char address_buffer[100];
                    getnameinfo((struct sockaddr *) &client_address,
                                client_len,
                                address_buffer, sizeof(address_buffer), 0, 0,
                                NI_NUMERICHOST);
                    printf("New connection from %s\n", address_buffer);
                } else {
                    char read[1024];
                    int bytes_received = recv(i, read, 1024, 0);
                    if (bytes_received < 1) {
                        FD_CLR(i, &master);
                        SDK_SOCKET_CLOSE(i);
                        continue;
                    }

                    int j;
                    for (j = 0; j < bytes_received; ++j)
                        read[j] = toupper(read[j]);
                    send(i, read, bytes_received, 0);
                }
            } //if FD_ISSET
        } //for i to max_socket
    } //while(1)

    printf("Closing listening socket...\n");
    SDK_SOCKET_CLOSE(socket_listen);

    sdk_socket_destroy();
    return 0;
}