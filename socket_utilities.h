#ifndef SOCKET_UTILITIES_H
#define SOCKET_UTILITIES_H

#define PORT 8123
#define BACKLOG 400
#define SOCKETERROR -1
#define BUFFERSIZE 4096
#define PASSWORD "qwerty"
#define NAME_SIZE 32
#define SURNAME_SIZE 32
#define NUMBER_SIZE 32
#define PASSWORD_SIZE 16

#include <sys/_types/_size_t.h>

/*
 * Utility function to check if a call returned a negative value, meaning an error.
 * The function prints in the stderr the message passed as second argument and
 * exits the program.
 */
void check(int value, const char *msg);

int create_server_socket();
int create_client_socket(const char *ip_address);
int accept_new_connection(int server_socket);
int receive_message(int socket, char* buffer);
void send_message(int socket, const char *message, size_t len);

struct sockaddr_in create_addr_struct(const char *ip_address);

#endif //SOCKET_UTILITIES_H
