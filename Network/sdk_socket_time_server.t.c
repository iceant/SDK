#include <sdk_net.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char** argv){
    sdk_net_init();
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
    socket_listen = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);
    if(!SDK_SOCKET_ISVALID(socket_listen)){
        fprintf(stderr, "socket() failed. (%d)\n", SDK_SOCKET_ERRNO());
        return 1;
    }

    printf("Binding socket to local address...\n");
    if(bind(socket_listen, bind_address->ai_addr, bind_address->ai_addrlen)){
        fprintf(stderr, "bind() failed. (%d)\n", SDK_SOCKET_ERRNO());
        return 2;
    }
    freeaddrinfo(bind_address);

    printf("Listening...\n");
    if(listen(socket_listen, 10)<0){
        fprintf(stderr, "listen() failed. (%d)\n", SDK_SOCKET_ERRNO());
        return 3;
    }

    printf("Waiting for connection... \n");
    struct sockaddr_storage client_address;
    socklen_t client_len = sizeof(client_address);
    sdk_socket_t socket_client = accept(socket_listen,
                                  (struct sockaddr*) &client_address, &client_len);
    if (!SDK_SOCKET_ISVALID(socket_client)) {
        fprintf(stderr, "accept() failed. (%d)\n", SDK_SOCKET_ERRNO());
        return 4;
    }

    printf("Client is connected... ");
    char address_buffer[100];
    getnameinfo((struct sockaddr*)&client_address,
                client_len, address_buffer, sizeof(address_buffer), 0, 0,
                NI_NUMERICHOST);
    printf("%s\n", address_buffer);

    printf("Reading request...\n");
    char request[1024];
    int bytes_received = recv(socket_client, request, 1024, 0);
    printf("Received %d bytes.\n", bytes_received);

    printf("%.*s", bytes_received, request);

    printf("Sending response...\n");
    const char *response =
            "HTTP/1.1 200 OK\r\n"
            "Connection: close\r\n"
            "Content-Type: text/plain\r\n\r\n"
            "Local time is: ";
    int bytes_sent = send(socket_client, response, strlen(response), 0);
    printf("Sent %d of %d bytes.\n", bytes_sent, (int)strlen(response));

    time_t timer;
    time(&timer);
    char *time_msg = ctime(&timer);
    bytes_sent = send(socket_client, time_msg, strlen(time_msg), 0);
    printf("Sent %d of %d bytes.\n", bytes_sent, (int)strlen(time_msg));

    printf("Closing connection...\n");
    SDK_SOCKET_CLOSE(socket_client);

    printf("Closing listening socket...\n");
    SDK_SOCKET_CLOSE(socket_listen);

    sdk_net_destroy();
    printf("Finished.\n");



    return 0;
}