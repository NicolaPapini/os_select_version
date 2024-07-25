#ifndef CONNECTION_HANDLER_H
#define CONNECTION_HANDLER_H
#define BUFFERSIZE 2048

typedef enum{
    ADD_CONTACT,
    SEARCH_CONTACT_SURNAME_NAME,
    SEARCH_CONTACT_NUMBER,
    DELETE_CONTACT,
    UPDATE_CONTACT,
    EXIT
} Operation;

#include <sys/_types/_size_t.h>
/*
 * Remember to free memory allocated to the returned message after using it.
 */
char *receive_message(int socket, char* buffer);
void send_message(int socket, const char *message, size_t len);
void *handle_connection(int *client_socket);
#endif //CONNECTION_HANDLER_H
