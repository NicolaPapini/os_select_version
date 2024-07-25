#ifndef QUEUE_H
#define QUEUE_H

typedef struct Node {
    struct Node *next;
    int *client_socket;
} Node;

void enqueue(int *client_socket);
int *dequeue();

#endif
