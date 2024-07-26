#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "data_structures/cJSON.h"
#include "socket_utilities.h"
#include "client_request_handler.h"
#include "server_response_parser.h"

#define AVAILABLE_TRIES 3

int main(int arc, char *argv[]) {
    /* Check for the correct number of arguments passed.*/

    if (arc != 2) {
        printf("Usage: %s <server_ip>\n", argv[0]);
        return 1;
    }

    int client_socket = create_client_socket(argv[1]);
    int operation = 0;
    int remaining_tries = AVAILABLE_TRIES;

    printf("Welcome to the phone directory application."
        "Enter the desired operation:\n "
        "1. Add a new contact\n "
        "2. Search for a contact by surname and name\n "
        "3. Search for a contact by number\n "
        "4. Delete a contact\n "
        "5. Update a contact\n ");
    do {
        scanf("%d", &operation);
        if (operation >= 1 && operation <= 6) {
            handle_request(operation, client_socket);
            break;
        } else {
            remaining_tries--;
            printf("Invalid operation. Please try again.\n"
                   "Remaining tries: %d\n", remaining_tries);
            if (remaining_tries == 0) {
                printf("You have exceeded the number of tries\n");
                return EXIT_FAILURE;
            }
        }
    } while (true);
    char *server_response = (char*)calloc(BUFFERSIZE, sizeof(char));
    receive_message(client_socket, server_response);
    cJSON *response = cJSON_Parse(server_response);
    parse_response(response);
    free(server_response);
    return EXIT_SUCCESS;
}

