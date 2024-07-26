#ifndef HASHMAP_H
#define HASHMAP_H
#define INITIAL_CAPACITY 15

typedef struct Contact{
    char *name;
    char *surname;
    char *phone_number;
    struct Contact *next;
} Contact;

typedef struct HashNode{
    char *key;
    Contact *value;
    struct HashNode *next;
} HashNode;

typedef struct HashMap{
    HashNode **buckets;
    int capacity;
    int number_of_elements;
} HashMap;

HashMap* initialize_hashmap();
Contact* initialize_list_node(const char* name, const char *surname, const char *phone_number);
void put(HashMap *map, const char *key, Contact *value);
Contact* get(const HashMap* map, const char *key);
void remove_node(HashMap *map, const char *key);
#endif