#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <pthread.h>
#include <signal.h>
#include "socket_utilities.h"
#include "connection_handler.h"
#include "database.h"
#include "data_structures/queue.h"
#include "data_structures//cJSON.h"
#include "data_structures/phone_directory.h"
#define THREAD_POOL_SIZE 30

PhoneDirectory *phone_directory_ptr = NULL;
pthread_t thread_pool[THREAD_POOL_SIZE];
pthread_cond_t queue_condition = PTHREAD_COND_INITIALIZER;
pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
void *thread_loop(void *args);
void sigint_handler(int i);

int main(void) {
    int server_socket = create_server_socket();
    signal(SIGINT, sigint_handler);
    phone_directory_ptr = initialize_phone_directory();
    deserialize(phone_directory_ptr, PHONE_DIRECTORY_FILE);
    /*
     * https://man7.org/linux/man-pages/man2/select.2.html
     * Upon return, each of the file descriptor sets is
     * modified in place to indicate which file descriptors are
     * currently "ready".  Thus, if using select() within a loop, the
     * sets must be reinitialized before each call.
     */
    for (int i = 0; i < THREAD_POOL_SIZE; i++) {
        /*
         * First argument: pointer to the thread id.
         * Second argument: thread attributes. NULL = default attributes.
         * Third argument: pointer to the function to be executed by the thread.
         * Fourth argument: argument to be passed to the function. NULL = no argument.
         */
        pthread_create(&thread_pool[i], NULL, thread_loop, NULL);
    }

    fd_set current_sockets, reader_set;
    /* Initialize the set to 0. */
    FD_ZERO(&current_sockets);
    /*Add the server socket to the current sockets.*/
    FD_SET(server_socket, &current_sockets);
    printf("Server online\n");
    while (true) {
        reader_set = current_sockets;
        /*
         * First argument is the highest file descriptor in any of the sets + 1.
         * Second argument is the set of file descriptors to read from.
         * Third argument is the set of file descriptors to write to.
         * Fourth argument is the set of file descriptors to check for errors.
         * Fifth argument is the maximum time to wait for an event.
         */
        check (select(FD_SETSIZE, &reader_set, NULL, NULL, NULL),
            "Select failed");

        /*
         * Loop throught the file descriptors to check which one is ready. If the
         * ready one is the server socket it means that
         */
        for (int i = 0; i < FD_SETSIZE; i++) {
             if (FD_ISSET(i, &reader_set)) {
                 if (i == server_socket) {
                     int client_socket = accept_new_connection(server_socket);
                     /*FD_SET adds a fd to a set of fds*/
                     FD_SET(client_socket, &current_sockets);
                 } else {
                     int *client_socket = malloc(sizeof(int));
                     *client_socket = i;
                     pthread_mutex_lock(&queue_mutex);
                     enqueue(client_socket);
                     pthread_cond_signal(&queue_condition);
                     pthread_mutex_unlock(&queue_mutex);
                     /*FD_CLEAR removes a fd to a set of fds*/
                     FD_CLR(i, &current_sockets);
                 }
             }
        }

    }
    return EXIT_SUCCESS;
}

void *thread_loop(void *args) {
    while (true) {
        int *client_ptr;
        pthread_mutex_lock(&queue_mutex);
        while ((client_ptr = dequeue()) == NULL) {
            pthread_cond_wait(&queue_condition, &queue_mutex);
        }
        pthread_mutex_unlock(&queue_mutex);
        printf("Handling connection in thread %p\n", pthread_self());
        handle_connection(client_ptr, phone_directory_ptr);
        close(*client_ptr);
        free(client_ptr);
        printf("Thread %p finished handling connection\n", pthread_self());
    }
}

void sigint_handler (int sig_num) {
    (void)sig_num;

    printf("\nCaught signal Ctrl-C (SIGINT), saving database...\n");
    while (!serialize(phone_directory_ptr, PHONE_DIRECTORY_FILE)) {
        printf("Failed to save database, trying again...\n");
    }

    exit(EXIT_SUCCESS);
}
