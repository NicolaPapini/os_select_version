#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* create_db_entry(Contact *node);

bool serialize_trie(FILE *file, TrieNode *node) {
    if (node == NULL) {
        return false;
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        serialize_trie(file, node->children[i]);
    }

    if (node->is_end_of_word && node->head != NULL) {
        Contact *temp = node->head;
        while (temp != NULL) {
            char *to_write = create_db_entry(temp);
            fwrite(to_write, strlen(to_write), 1, file);
            temp = temp->next;
        }
    }
    return true;
}

bool serialize(const PhoneDirectory *phone_dir, const char *filename){
    FILE *file = fopen(filename, "w");

    if(file == NULL){
        return false;
    }

    serialize_trie(file, phone_dir->root);
    fclose(file);
    return true;
}

void deserialize(PhoneDirectory *phone_dir, const char *filename){
    FILE *file = fopen(filename, "r");
    if(file == NULL){
        return;
    }
    char line[MAXLINE];
    while(fgets(line, sizeof(line), file)){
        char *name = strtok(line, ",");
        char *surname = strtok(NULL, ",");
        char *phone_number = strtok(NULL, ",");
        phone_number[strlen(phone_number) - 1] = 0;
        insert_record(phone_dir, name, surname, phone_number);
    }
    fclose(file);
}

char* create_db_entry(Contact *node) {
    const int totalLength = strlen(node->name) + strlen(node->surname) + strlen(node->phone_number) + 3 + 1;
    char *entry = (char *)malloc(totalLength * sizeof(char));
    snprintf(entry, totalLength, "%s,%s,%s\n", node->name, node->surname, node->phone_number);
    return entry;
}