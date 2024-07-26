#ifndef CONNECTION_HANDLER_H
#define CONNECTION_HANDLER_H
#include "data_structures/phone_directory.h"

typedef enum{
    ADD_CONTACT,
    SEARCH_CONTACT_SURNAME_NAME,
    SEARCH_CONTACT_NUMBER,
    DELETE_CONTACT,
    UPDATE_CONTACT
} Operation;

void handle_connection(int *client_socket, const PhoneDirectory *phone_dir_ptr);
#endif //CONNECTION_HANDLER_H
