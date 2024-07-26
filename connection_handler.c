#include "connection_handler.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "socket_utilities.h"
#include "data_structures/cJSON.h"
#include "connection_utilities.h"

pthread_mutex_t phone_dir_mutex = PTHREAD_MUTEX_INITIALIZER;

void insert_into_phone_dir(const cJSON *contact_data_ptr, const PhoneDirectory *phone_dir_ptr, char *sendline);
void search_contact_by_surname_name(const cJSON *contact_data_ptr, const PhoneDirectory *phone_dir_ptr, char *sendline);
void search_contact_by_number(const cJSON *contact_data_ptr, const PhoneDirectory *phone_dir_ptr, char *sendline);
void remove_from_phone_dir(const cJSON *contact_data_ptr, const PhoneDirectory *phone_dir_ptr, char *sendline);
void update_in_phone_dir(const cJSON *contact_data_ptr, const PhoneDirectory *phone_dir_ptr, char *sendline);
void exit_program(int client_socket, const cJSON *contact_data_ptr, const PhoneDirectory *phone_dir_ptr, char *sendline);


void handle_connection(int *client_socket, const PhoneDirectory *phone_dir_ptr) {
    printf("Entered handle connection\n\n");
    char *recvline = calloc(BUFFERSIZE, sizeof(char));
    int bytes_received = receive_message(*client_socket, recvline);

    if (bytes_received <= 0) {
        printf("Client disconnected or error occurred\n");
        close(*client_socket);
        free(recvline);
        return;
    }

    printf("Message received: %s\n", recvline);

    const cJSON *request = cJSON_Parse(recvline);
    const cJSON *operation_json = cJSON_GetObjectItem(request, "operation");
    Operation operation;
    check (operation = get_operation(operation_json->valuestring),
        "Invalid operation");
    char *sendline = calloc(BUFFERSIZE, sizeof(char));
    switch (operation) {
        case ADD_CONTACT:
            printf("Add contact\n");
            insert_into_phone_dir(request, phone_dir_ptr, sendline);
            break;
        case SEARCH_CONTACT_SURNAME_NAME:
            printf("Search contact by surname and name\n");
            search_contact_by_surname_name(request, phone_dir_ptr, sendline);
            break;
        case SEARCH_CONTACT_NUMBER:
            printf("Search contact by number\n");
            search_contact_by_number(request, phone_dir_ptr, sendline);
            break;
        case DELETE_CONTACT:
            printf("Delete contact\n");
            remove_from_phone_dir(request, phone_dir_ptr, sendline);
            break;
        case UPDATE_CONTACT:
            printf("Update contact\n");
            update_in_phone_dir(request, phone_dir_ptr, sendline);
            break;
        default:
            printf("Invalid operation\n");
            return;
    }
    send_message(*client_socket, sendline, strlen(sendline));
    free(sendline);
    free(recvline);
}

void insert_into_phone_dir(const cJSON *contact_data_ptr, const PhoneDirectory *phone_dir_ptr, char *sendline) {
    cJSON *response = cJSON_CreateObject();
    cJSON_AddStringToObject(response, "operation", "ADD");
    if (!check_password(contact_data_ptr)) {
        invalid_password_procedure(response, sendline);
        return;
    }
    char name[NAME_SIZE], surname[SURNAME_SIZE], number[NUMBER_SIZE];
    get_common_fields(name, surname, number, contact_data_ptr);

    pthread_mutex_lock(&phone_dir_mutex);
    const Status result = insert_record(phone_dir_ptr, name, surname, number);
    pthread_mutex_unlock(&phone_dir_mutex);

    add_result_to_response(response, result);
    strcpy(sendline, cJSON_PrintUnformatted(response));
    cJSON_Delete(response);
}

void search_contact_by_surname_name(const cJSON *contact_data_ptr, const PhoneDirectory *phone_dir_ptr, char *sendline) {
    char *name = (cJSON_GetObjectItem(contact_data_ptr, "name"))->valuestring;
    char *surname = (cJSON_GetObjectItem(contact_data_ptr, "surname"))->valuestring;

    Contact *contact = search_record(phone_dir_ptr, name, surname);

    cJSON *response = cJSON_CreateObject();
    cJSON_AddStringToObject(response, "operation", "SEARCH_SN");
    if (contact == NULL) {
        add_result_to_response(response, RECORD_NOT_FOUND);
    } else {
        create_multiple_entry_jSON(response, contact);
        add_result_to_response(response, SUCCESS);
    }
    strcpy(sendline, cJSON_PrintUnformatted(response));
    cJSON_Delete(response);
}

void search_contact_by_number(const cJSON *contact_data_ptr, const PhoneDirectory *phone_dir_ptr, char *sendline) {
    char *number = (cJSON_GetObjectItem(contact_data_ptr, "number"))->valuestring;

    const Contact *contact = search_record_by_number(phone_dir_ptr, number);

    cJSON *response = cJSON_CreateObject();
    cJSON_AddStringToObject(response, "operation", "SEARCH_N");
    if (contact == NULL) {
        add_result_to_response(response, RECORD_NOT_FOUND);
    } else {
        cJSON *temp = cJSON_CreateObject();
        cJSON_AddStringToObject(temp, "name", contact->name);
        cJSON_AddStringToObject(temp, "surname", contact->surname);
        cJSON_AddStringToObject(temp, "number", contact->phone_number);
        cJSON_AddItemToObject(response, "contact", temp);
        add_result_to_response(response, SUCCESS);
    }
    strcpy(sendline, cJSON_PrintUnformatted(response));
    cJSON_Delete(response);
}

void remove_from_phone_dir(const cJSON *contact_data_ptr, const PhoneDirectory *phone_dir_ptr, char *sendline) {
    cJSON *response = cJSON_CreateObject();
    cJSON_AddStringToObject(response, "operation", "DEL");
    if (!check_password(contact_data_ptr)) {
        invalid_password_procedure(response, sendline);
        return;
    }

    char name[NAME_SIZE], surname[SURNAME_SIZE], number[NUMBER_SIZE];
    get_common_fields(name, surname, number, contact_data_ptr);

    pthread_mutex_lock(&phone_dir_mutex);
    const Status result = delete_record(phone_dir_ptr, name, surname, number);
    pthread_mutex_unlock(&phone_dir_mutex);

    add_result_to_response(response, result);
    strcpy(sendline, cJSON_PrintUnformatted(response));
    cJSON_Delete(response);
}

void update_in_phone_dir(const cJSON *contact_data_ptr, const PhoneDirectory *phone_dir_ptr, char *sendline) {
    cJSON *response = cJSON_CreateObject();
    cJSON_AddStringToObject(response, "operation", "UPD");
    if (!check_password(contact_data_ptr)) {
        invalid_password_procedure(response, sendline);
        return;
    }
    char name[NAME_SIZE], surname[SURNAME_SIZE], number[NUMBER_SIZE];
    get_common_fields(name, surname, number, contact_data_ptr);
    char *new_number = (cJSON_GetObjectItem(contact_data_ptr, "new_number"))->valuestring;

    pthread_mutex_lock(&phone_dir_mutex);
    const Status result = update_record(phone_dir_ptr, surname, name, number, new_number);
    pthread_mutex_unlock(&phone_dir_mutex);


    add_result_to_response(response, result);
    strcpy(sendline, cJSON_PrintUnformatted(response));
    cJSON_Delete(response);
}


