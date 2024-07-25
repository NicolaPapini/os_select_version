#include "socket_utilities.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

int create_server_socket() {
    int socket_fd;
    struct sockaddr_in server_address = create_addr_struct(NULL);
    check (socket_fd = socket(AF_INET, SOCK_STREAM, 0),
        "Socket creation failed");
    check (bind(socket_fd, (struct sockaddr*) &server_address, sizeof(server_address)),
        "Binding failed");
    check (listen(socket_fd, BACKLOG),
        "Listening failed");
    return socket_fd;
}

int create_client_socket(const char *ip_address) {
    int socket_fd;
    struct sockaddr_in server_address = create_addr_struct(ip_address);
    check (socket_fd = socket(AF_INET, SOCK_STREAM, 0),
        "Socket creation failed");
    check (connect(socket_fd, (struct sockaddr*) &server_address, sizeof(server_address)),
        "Connection failed");
    return socket_fd;
}

int accept_new_connection(int server_socket) {
    int client_socket;
    struct sockaddr_in client_address;
    int addr_size = sizeof(struct sockaddr_in);
    check (client_socket = accept(server_socket, (struct sockaddr*) &client_address, (socklen_t*) &addr_size),
        "Accepting client failed");
    return client_socket;
}
struct sockaddr_in create_addr_struct(const char *ip_address) {
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);

    if(ip_address == NULL) {
        addr.sin_addr.s_addr = INADDR_ANY;
    } else {
        int result = inet_pton(AF_INET, ip_address, &addr.sin_addr);
        if (result <= 0) {
            printf("Invalid address\n");
            exit(EXIT_FAILURE);
        }
    }
    return addr;
}



void check(int value, const char *msg) {
    if (value == SOCKETERROR) {
        perror(msg);
        exit(EXIT_FAILURE);
    }
}
