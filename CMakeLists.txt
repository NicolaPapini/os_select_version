cmake_minimum_required(VERSION 3.28)
project(os_select_version C)

set(CMAKE_C_STANDARD 11)

add_executable(server server.c
        socket_utilities.c
        socket_utilities.h
        connection_handler.c
        connection_handler.h
        data_structures/queue.c
        data_structures/queue.h
        data_structures/cJSON.c
        data_structures/cJSON.h
        data_structures/phone_directory.c
        data_structures/phone_directory.h
        data_structures/hashmap.c
        data_structures/hashmap.h
        connection_utilities.c
        connection_utilities.h
        database.c
        database.h
)

add_executable(client client.c
        socket_utilities.c
        socket_utilities.h
        connection_handler.c
        connection_handler.h
        data_structures/cJSON.c
        data_structures/cJSON.h
        client_request_handler.c
        client_request_handler.h
        connection_utilities.c
        connection_utilities.h
        data_structures/phone_directory.c
        data_structures/phone_directory.h
        data_structures/hashmap.c
        data_structures/hashmap.h
        server_response_parser.c
        server_response_parser.h
)
