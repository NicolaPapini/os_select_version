#include "client_request_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "connection_handler.h"
#include "data_structures/cJSON.h"

void add_contact(int client_socket, char* buffer);
void search_by_surname_name(int client_socket, char* buffer);
void search_by_number(int client_socket, char* buffer);
void delete_contact(int client_socket, char* buffer);
void update_contact(int client_socket, char* buffer);
void prompt_for_common_fields(char *name, char *surname, char *number, char *password);

void handle_request(int operation, int client_socket) {
    char *buffer = (char*)calloc(BUFFERSIZE, sizeof(char));
    switch (operation-1) {
        case ADD_CONTACT:
            printf("Add contact procedure.\n");
            add_contact(client_socket, buffer);
        break;
        case SEARCH_CONTACT_SURNAME_NAME:
            printf("Search contact by surname and name\n");
            search_by_surname_name(client_socket, buffer);
        break;
        case SEARCH_CONTACT_NUMBER:
            printf("Search contact by number\n");
            search_by_number(client_socket, buffer);
        break;
        case DELETE_CONTACT:
            printf("Delete contact\n");
            delete_contact(client_socket, buffer);
        break;
        case UPDATE_CONTACT:
            printf("Update contact\n");
            update_contact(client_socket, buffer);
        break;
        case EXIT:
            exit(EXIT_SUCCESS);
        default:
            printf("Invalid operation\n");
    }
    send_message(client_socket, buffer, strlen(buffer));
    free(buffer);
}

void add_contact(int client_socket, char* buffer) {
    char name[NAME_SIZE], surname[SURNAME_SIZE], number[NUMBER_SIZE], password[PASSWORD_SIZE];
    cJSON *request = cJSON_CreateObject();
    prompt_for_common_fields(name, surname, number, password);
    cJSON_AddStringToObject(request, "operation", "ADD");
    cJSON_AddStringToObject(request, "password", password);
    cJSON_AddStringToObject(request, "name", name);
    cJSON_AddStringToObject(request, "surname", surname);
    cJSON_AddStringToObject(request, "number", number);
    strcpy(buffer, cJSON_Print(request));
    cJSON_Delete(request);
}

void search_by_surname_name(int client_socket, char* buffer) {
    char name[NAME_SIZE], surname[SURNAME_SIZE];
    cJSON *request = cJSON_CreateObject();
    printf("Enter the name: ");
    scanf("%s", name);
    printf("Enter the surname: ");
    scanf("%s", surname);
    cJSON_AddStringToObject(request, "operation", "SEARCH_SN");
    cJSON_AddStringToObject(request, "name", name);
    cJSON_AddStringToObject(request, "surname", surname);
    strcpy(buffer, cJSON_Print(request));
    cJSON_Delete(request);
}

void search_by_number(int client_socket, char* buffer) {
    char number[NUMBER_SIZE];
    cJSON *request = cJSON_CreateObject();
    printf("Enter the number: ");
    scanf("%s", number);
    cJSON_AddStringToObject(request, "operation", "SEARCH_N");
    cJSON_AddStringToObject(request, "number", number);
    strcpy(buffer, cJSON_Print(request));
    cJSON_Delete(request);
}

void delete_contact(int client_socket, char* buffer) {
    char name[NAME_SIZE], surname[SURNAME_SIZE], number[NUMBER_SIZE], password[PASSWORD_SIZE];
    cJSON *request = cJSON_CreateObject();
    prompt_for_common_fields(name, surname, number, password);
    cJSON_AddStringToObject(request, "operation", "DEL");
    cJSON_AddStringToObject(request, "password", password);
    cJSON_AddStringToObject(request, "name", name);
    cJSON_AddStringToObject(request, "surname", surname);
    cJSON_AddStringToObject(request, "number", number);
    strcpy(buffer, cJSON_Print(request));
    cJSON_Delete(request);
}

void update_contact(int client_socket, char* buffer) {
    char name[NAME_SIZE], surname[SURNAME_SIZE], number[NUMBER_SIZE], new_number[NUMBER_SIZE], password[PASSWORD_SIZE];
    cJSON *request = cJSON_CreateObject();
    prompt_for_common_fields(name, surname, number, password);
    printf("Enter the new number: ");
    scanf("%s", new_number);
    cJSON_AddStringToObject(request, "operation", "UPD");
    cJSON_AddStringToObject(request, "password", password);
    cJSON_AddStringToObject(request, "name", name);
    cJSON_AddStringToObject(request, "surname", surname);
    cJSON_AddStringToObject(request, "number", number);
    cJSON_AddStringToObject(request, "new_number", new_number);
    strcpy(buffer, cJSON_Print(request));
    cJSON_Delete(request);
}

void prompt_for_common_fields(char *name, char *surname, char *number, char *password) {
    printf("Password required to execute this operation\n");
    scanf("%s", password);
    printf("Enter the name: ");
    scanf("%s", name);
    printf("Enter the surname: ");
    scanf("%s", surname);
    printf("Enter the number: ");
    scanf("%s", number);
}