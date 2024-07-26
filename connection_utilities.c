#include "connection_utilities.h"
#include <stdio.h>
#include <string.h>
#include "socket_utilities.h"

void add_result_to_response(cJSON *json, const Status result) {
    char *to_be_added;
    switch (result) {
        case SUCCESS:
            to_be_added = "SUCCESS";
        break;
        case INVALID_INPUT:
            to_be_added = "INVALID_INPUT";
        break;
        case RECORD_EXISTS:
            to_be_added = "RECORD_EXISTS";
        break;
        case RECORD_NOT_FOUND:
            to_be_added = "RECORD_NOT_FOUND";
        break;
        default:
            to_be_added = "INVALID";
    }
    cJSON_AddStringToObject(json, "result", to_be_added);
}

Operation get_operation(const char *operation_str) {
    if (strcmp(operation_str, "ADD") == 0) {
        return ADD_CONTACT;
    }
    if (strcmp(operation_str, "SEARCH_SN") == 0) {
        return SEARCH_CONTACT_SURNAME_NAME;
    }
    if (strcmp(operation_str, "SEARCH_N") == 0) {
        return SEARCH_CONTACT_NUMBER;
    }
    if (strcmp(operation_str, "DEL") == 0) {
        return DELETE_CONTACT;
    }
    if (strcmp(operation_str, "UPD") == 0) {
        return UPDATE_CONTACT;
    }
    return -1;
}

void create_multiple_entry_jSON(cJSON *response, Contact *contact) {
    cJSON *contacts_array = cJSON_CreateArray();
    while (contact != NULL) {
        cJSON *contact_json = cJSON_CreateObject();
        cJSON_AddStringToObject(contact_json, "name", contact->name);
        cJSON_AddStringToObject(contact_json, "surname", contact->surname);
        cJSON_AddStringToObject(contact_json, "number", contact->phone_number);
        cJSON_AddItemToArray(contacts_array, contact_json);
        contact = contact->next;
    }
    cJSON_AddItemToObject(response, "contacts", contacts_array);
}

bool check_password(const cJSON *request) {
    const cJSON *password_json = cJSON_GetObjectItem(request, "password");
    if (password_json == NULL) {
        printf("Password not found\n");
        return false;
    }
    return strcmp(password_json->valuestring, PASSWORD) == 0;
}

void get_common_fields(char *name, char *surname, char *number, const cJSON *request) {
    const cJSON *name_json = cJSON_GetObjectItem(request, "name");
    const cJSON *surname_json = cJSON_GetObjectItem(request, "surname");
    const cJSON *number_json = cJSON_GetObjectItem(request, "number");
    strcpy(name, name_json->valuestring);
    strcpy(surname, surname_json->valuestring);
    strcpy(number, number_json->valuestring);
}

void invalid_password_procedure(cJSON *response, char *sendline) {
    cJSON_AddStringToObject(response, "result", "INVALID_PASSWORD");
    strcpy(sendline, cJSON_PrintUnformatted(response));
    cJSON_Delete(response);
}