#include "server_response_parser.h"

#include <stdio.h>

#include "connection_utilities.h"

void print_contact(const cJSON *contact);
void print_all_contacts(const cJSON *contacts);

void parse_search_contact_number(const cJSON *response) {
    cJSON *status = cJSON_GetObjectItem(response, "status");
    cJSON *contact = cJSON_GetObjectItem(response, "contact");

    char *status_message = status->valuestring;
    char *contact_data = contact->valuestring;

    printf("%s\n", status_message);
    printf("%s\n", contact_data);
}

void parse_response(const cJSON *response) {
    cJSON *operation = cJSON_GetObjectItem(response, "operation");
    cJSON *result = cJSON_GetObjectItem(response, "result");

    char *operation_str = operation->valuestring;
    Operation op = get_operation(operation_str);
    char *status_message = result->valuestring;

    switch (op) {
        case ADD_CONTACT:
            printf("%s: %s\n", operation_str, status_message);
            break;
        case SEARCH_CONTACT_SURNAME_NAME:
            print_all_contacts(cJSON_GetObjectItem(response, "contacts"));
            printf("%s: %s\n", operation_str, status_message);
            break;
        case SEARCH_CONTACT_NUMBER:
            print_contact(cJSON_GetObjectItem(response, "contact"));
            printf("%s: %s\n", operation_str, status_message);
            break;
        case DELETE_CONTACT:
            printf("%s: %s\n", operation_str, status_message);
            break;
        case UPDATE_CONTACT:
            printf("%s: %s\n", operation_str, status_message);
            break;
        default:
            printf("Invalid operation\n");
    }
}

void print_contact(const cJSON *contact) {
    const char *name = cJSON_GetObjectItem(contact, "name")->valuestring;
    const char *surname = cJSON_GetObjectItem(contact, "surname")->valuestring;
    const char *number = cJSON_GetObjectItem(contact, "number")->valuestring;
    printf("Name: %s\nSurname: %s\nNumber: %s\n", name, surname, number);
}

void print_all_contacts(const cJSON *contacts) {
    int size = cJSON_GetArraySize(contacts);
    for (int i = 0; i < size; i++) {
        cJSON *contact = cJSON_GetArrayItem(contacts, i);
        print_contact(contact);
        if (i < size - 1) {
            printf("\n");
        }
    }
}