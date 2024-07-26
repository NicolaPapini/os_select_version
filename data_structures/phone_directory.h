#ifndef PHONE_DIRECTORY_H
#define PHONE_DIRECTORY_H
// #define ALPHABET_SIZE 26
#define ALPHABET_SIZE 27
#define ALPHABET "abcdefghijklmnopqrstuvwxyz "
#include <stdbool.h>
#include "hashmap.h"

typedef struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    bool is_end_of_word;
    Contact *head;
} TrieNode;

typedef struct PhoneDirectory {
    TrieNode *root;
    HashMap *hash_map;
} PhoneDirectory;

typedef enum {
    SUCCESS,
    INVALID_INPUT,
    RECORD_EXISTS,
    RECORD_NOT_FOUND,
} Status;

PhoneDirectory *initialize_phone_directory();
Status insert_record(const PhoneDirectory *phone_dir, char *name, char *surname, char *phone_number);
Contact* search_record(const PhoneDirectory *phone_dir, char *name, char *surname);
Contact* search_record_by_number(const PhoneDirectory *phone_dir, char *phone_number);
Status delete_record(const PhoneDirectory *phone_dir, char *name, char *surname, char *phone_number);
Status update_record(const PhoneDirectory *phone_dir, char *surname, char *name, char *phone_number, char *new_phone_number);

#endif //PHONE_DIRECTORY_H
