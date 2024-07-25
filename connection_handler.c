#include "connection_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char *receive_message(int socket, char* buffer) {
    int msg_size = 0;
    size_t bytes_read;
    while ((bytes_read = read(socket, buffer + msg_size, BUFFERSIZE - msg_size - 1)) > 0) {
        msg_size += bytes_read;
        if (msg_size > BUFFERSIZE - 1 || buffer[msg_size - 1] == '\n') {
            break;
        }
    }
    buffer[msg_size] = '\0';
    return buffer;
}

void send_message(int socket, const char *message, size_t len) {
    int remaining_tries = 20;
    while (write(socket, message, len) != len) {
        printf("Write failed. Remaining tries: %d\n", remaining_tries--);
        if (remaining_tries == 0) {
            perror("Write failed\n");
            exit(EXIT_FAILURE);
        }
    }
}

void *handle_connection(int *client_socket) {
    printf("Entered handle connection\n\n");
    char *recvline = calloc(BUFFERSIZE, sizeof(char));
    receive_message(*client_socket, recvline);
    printf("Message receive: %s\n", recvline);
    free(recvline);
    return NULL;
}