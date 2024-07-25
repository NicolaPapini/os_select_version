#ifndef SOCKET_UTILITIES_H
#define SOCKET_UTILITIES_H

#define PORT 8080
#define BACKLOG 100
#define SOCKETERROR -1

/*
 * Utility function to check if a call returned a negative value, meaning an error.
 * The function prints in the stderr the message passed as second argument and
 * exits the program.
 */
void check(int value, const char *msg);

int create_server_socket();
int create_client_socket(const char *ip_address);
int accept_new_connection(int server_socket);

struct sockaddr_in create_addr_struct(const char *ip_address);

#endif //SOCKET_UTILITIES_H
