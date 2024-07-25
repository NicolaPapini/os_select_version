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
)
add_executable(client client.c
        socket_utilities.c
        socket_utilities.h
        connection_handler.c
        connection_handler.h
        cJSON/cJSON.c
        cJSON/cJSON.h
        client_request_handler.c
        client_request_handler.h
)
