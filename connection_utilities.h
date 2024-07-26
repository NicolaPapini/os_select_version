#ifndef CONNECTION_UTILITIES_H
#define CONNECTION_UTILITIES_H
#include "connection_handler.h"
#include "data_structures/cJSON.h"
#include "data_structures/phone_directory.h"

void add_result_to_response(cJSON *json, const Status result);
void create_multiple_entry_jSON(cJSON *response, Contact *contact);
Operation get_operation(const char *operation_str);
bool check_password(const cJSON *request);
void get_common_fields(char *name, char *surname, char *number, const cJSON *request);
void invalid_password_procedure(cJSON *response, char *sendline);

#endif //CONNECTION_UTILITIES_H
