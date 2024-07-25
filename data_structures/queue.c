//
// Created by Nicola Papini on 20/07/24.
//

#include "queue.h"
#include <stddef.h>
#include <stdlib.h>

Node *head = NULL;
Node *tail = NULL;

void enqueue(int *client_socket) {
    Node *new_node = (Node*) malloc(sizeof(Node));
    new_node->client_socket = client_socket;
    new_node->next = NULL;
    if (tail == NULL) {
        head = new_node;
    } else {
        tail->next = new_node;
    }
    tail = new_node;
}

/*Return NULL if there are no elements in the queue*/
int *dequeue() {
    if (head == NULL) {
        return NULL;
    } else {
        int* result = head->client_socket;
        Node *temp = head;
        head = head->next;
        if (head == NULL) {
            tail = NULL;
        }
        return result;
    }
}
