#include "phone_directory.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char* standardize(const char *name, const char *surname);
Contact *visit_subtree(const TrieNode *root, Contact *head);
bool check_if_number(const char *str);
void delete_pd_helper(TrieNode* root);
int get_index(char c);

/***************************** Constructors ******************************/

TrieNode *initialize_trie_node() {
    TrieNode *node = malloc(sizeof(TrieNode));
    node->is_end_of_word = false;
    node->head = NULL;
    for (int i = 0; i<ALPHABET_SIZE; i++) {
        node->children[i] = NULL;
    }
    return node;
}

PhoneDirectory *initialize_phone_directory() {
    PhoneDirectory *phone_dir = malloc(sizeof(PhoneDirectory));
    phone_dir->root = initialize_trie_node();
    phone_dir->hash_map = initialize_hashmap();
    return phone_dir;
}

/******************************** Functions ********************************/

Status insert_record(const PhoneDirectory *phone_dir, const char *name, const char *surname, const char *phone_number) {
    if (phone_dir == NULL ||
        name == NULL ||
        surname == NULL ||
        phone_number == NULL ||
        !check_if_number(phone_number)) {
        return INVALID_INPUT;
    }

    char *standardized_name = standardize(name, surname);

    //Ricerca del nodo corretto nell'albero
    TrieNode *current = phone_dir->root;
    for (int i = 0; i < strlen(standardized_name); i++) {
        int index = get_index(standardized_name[i]);
        if (current->children[index] == NULL) {
            current->children[index] = initialize_trie_node();

        }
        current = current->children[index];
    }
    free(standardized_name);
    current->is_end_of_word = true;
    //Controllo se il numero di telefono è già presente
    if (get(phone_dir->hash_map, phone_number) != NULL) {
        return RECORD_EXISTS;
    }
    //Inserimento del nodo nella lista del nodo Trie
    Contact *new_node = initialize_list_node(name, surname, phone_number);
    if (current->head == NULL) {
        current->head = new_node;
    } else {
        new_node->next = current->head;
        current->head = new_node;
    }
    put(phone_dir->hash_map, phone_number, new_node);
    return SUCCESS;
}

Contact* search_record(const PhoneDirectory *phone_dir, const char *name, const char *surname) {
    if (phone_dir == NULL ||
        name == NULL ||
        surname == NULL) {
        return NULL;
    }
    char *standardized_name = standardize(name, surname);
    TrieNode *current = phone_dir->root;
    for (int i = 0; i < strlen(standardized_name); i++) {
        int index = get_index(standardized_name[i]);
        if (current->children[index] == NULL) {
            free(standardized_name);
            return NULL;
        }
        current = current->children[index];
    }
    free(standardized_name);
    if (current->is_end_of_word) {
        return current->head;
    } else {
        Contact *empty = NULL;
        return visit_subtree(current, empty);
    }
}

Contact* search_record_by_number(const PhoneDirectory *phone_dir, const char *phone_number) {
    return get(phone_dir->hash_map, phone_number);
}

Status delete_record(const PhoneDirectory *phone_dir, const char* name, const char *surname, const char *phone_number) {
    if (phone_dir == NULL ||
        surname == NULL ||
        name == NULL ||
        phone_number == NULL ||
        !check_if_number(phone_number)) {
        return INVALID_INPUT;
    }

    char *standardized_name = standardize(name, surname);
    TrieNode *current = phone_dir->root;

    for (int i = 0; i < strlen(standardized_name); i++) {
        int index = get_index(standardized_name[i]);
        if (current->children[index] == NULL) {
            free(standardized_name);
            return RECORD_NOT_FOUND;
        }
        current = current->children[index];
    }
    free(standardized_name);
    if (!current->is_end_of_word) {
        return RECORD_NOT_FOUND;
    }
    Contact *temp = current->head;
    Contact *prev = NULL;
    while (temp != NULL) {
        if (strcmp(temp->phone_number, phone_number) == 0) {
            if (prev == NULL) {
                current->head = temp->next;
            } else {
                prev->next = temp->next;
            }
            if (current->head == NULL) {
                current->is_end_of_word = false;
            }
            free(temp->name);
            free(temp->surname);
            free(temp->phone_number);
            free(temp);
            remove_node(phone_dir->hash_map, phone_number);
            return SUCCESS;
        }
        prev = temp;
        temp = temp->next;
    }
    return SUCCESS;
}

Status update_record(const PhoneDirectory *phone_dir, const char* surname, const char* name, const char *phone_number, const char *new_phone_number) {
    if (phone_dir == NULL ||
        surname == NULL ||
        name == NULL ||
        !check_if_number(phone_number) ||
        !check_if_number(new_phone_number)) {
        return INVALID_INPUT;
    }

    if (get(phone_dir->hash_map, phone_number) == NULL ||
        get(phone_dir->hash_map, new_phone_number) != NULL) {
        return RECORD_NOT_FOUND;
    }

    char *standardized_name = standardize(name, surname);
    TrieNode *current = phone_dir->root;
    for (int i = 0; i < strlen(standardized_name); i++) {
        int index = get_index(standardized_name[i]);
        if (current->children[index] == NULL) {
            free(standardized_name);
            return RECORD_NOT_FOUND;
        }
        current = current->children[index];
    }
    free(standardized_name);
    if (!current->is_end_of_word) {
        return RECORD_NOT_FOUND;
    }
    Contact *temp = current->head;
    while (temp != NULL) {
        if (strcmp(temp->phone_number, phone_number) == 0) {
            free(temp->phone_number);
            temp->phone_number = strdup(new_phone_number);
            remove_node(phone_dir->hash_map, phone_number);
            put(phone_dir->hash_map, new_phone_number, temp);
            return SUCCESS;
        }
        temp = temp->next;
    }
    return RECORD_NOT_FOUND;
}

/******************************** Helper functions ********************************/

char* standardize(const char *name, const char *surname) {
    int len = strlen(name) + strlen(surname);
    char *result = malloc(len + 1);
    //In caso avessi cognomi e nomi con lettere maiuscole e minuscole
    for (int i = 0; surname[i]; i++) {
        result[i] = tolower(surname[i]);
    }
    for (int i = 0; name[i]; i++) {
        result[strlen(surname) + i] = tolower(name[i]);
    }
    result[len] = '\0';
    return result;
}

Contact *visit_subtree(const TrieNode *root, Contact *head) {
    if (root == NULL) {
        return head;
    }
    if (root->is_end_of_word) {
        Contact *temp = root->head;
        while (temp != NULL) {
            Contact *new_node = initialize_list_node(temp->name, temp->surname, temp->phone_number);
            new_node->next = head;
            head = new_node;
            temp = temp->next;
        }
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        head = visit_subtree(root->children[i], head);
    }
    return head;
}

bool check_if_number(const char *str) {
    for (int i = 0; str[i]; i++) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

int get_index(char c) {
    if (c == ' ') {
        return 26;
    }
    return c - 'a';
}

