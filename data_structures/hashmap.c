#include "hashmap.h"
#include <string.h>
#include <stdlib.h>

//TODO: implement rehashing
/***************************** Constructors ******************************/

HashMap* initialize_hashmap() {
    HashMap *map = (HashMap*)malloc(sizeof(HashMap));
    map->capacity = INITIAL_CAPACITY;
    map->number_of_elements = 0;
    map->buckets = (HashNode**)malloc(sizeof(HashNode*) * map->capacity);
    for (int i = 0; i < map->capacity; i++) {
        map->buckets[i] = NULL;
    }
    return map;
}

Contact* initialize_list_node(const char* name, const char *surname, const char *phone_number) {
    Contact *node = (Contact*)malloc(sizeof(Contact));
    node->name = strdup(name);
    node->surname = strdup(surname);
    node->phone_number = strdup(phone_number);
    node->next = NULL;
    return node;
}

HashNode* initialize_hash_node(const char* key, Contact *value) {
    HashNode *node = (HashNode*)malloc(sizeof(HashNode));
    node->key = strdup(key);
    node->value = value;
    node->next = NULL;
    return node;
}
/********************************* Helpers *********************************/

/*
int hash(const char *key, int capacity) {
    int hash_value = atoi(key) % capacity;
    return hash_value;
}
*/

int hash(const char *key, int capacity) {
    char *endptr;
    long hash_value = strtol(key, &endptr, 10) % capacity;
    return (int)hash_value;
}

void rehash(HashMap *map) {
    int old_capacity = map->capacity;
    HashNode **old_buckets = map->buckets;
    map->capacity = 2 * old_capacity;
    map->buckets = (HashNode **)malloc(sizeof(HashNode *) * map->capacity);

    for (int i = 0; i < map->capacity; i++) {
        map->buckets[i] = NULL;
    }

    for (int i = 0; i < old_capacity; i++) {
        HashNode *current = old_buckets[i];
        while (current != NULL) {
            HashNode *next = current->next;
            put(map, current->key, current->value);
            free(current->key);
            free(current);
            current = next;
        }
    }
    free(old_buckets);
}

/******************************** Functions ********************************/


void put(HashMap *map, const char *key, Contact *value) {
    if (map == NULL || key == NULL || value == NULL) {
        return;
    }
    int bucket = hash(key, map->capacity);
    HashNode *newNode = initialize_hash_node(key, value);
    if (map->buckets[bucket] == NULL) {
        map->buckets[bucket] = newNode;
    } else {
        newNode->next = map->buckets[bucket];
        map->buckets[bucket] = newNode;
    }
    map->number_of_elements++;
    if ((float)map->number_of_elements/map->capacity > 0.75) {
        rehash(map);
    }
}

Contact* get(const HashMap *map, const char *key) {
    if (map == NULL || key == NULL) {
        return NULL;
    }
    int bucket = hash(key, map->capacity);
    HashNode *current = map->buckets[bucket];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }
    return NULL;
}

void remove_node(HashMap *map, const char *key) {
    if (map == NULL || key == NULL) {
        return;
    }
    int bucket = hash(key, map->capacity);
    HashNode *current = map->buckets[bucket];
    HashNode *prev = NULL;
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            if (prev == NULL) {
                map->buckets[bucket] = current->next;
            } else {
                prev->next = current->next;
            }
            free(current->key);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
    map->number_of_elements--;
}

void delete_hash_map(HashMap *map) {
    if (map == NULL) {
        return;
    }
    for (int i = 0; i < map->capacity; i++) {
        HashNode *current = map->buckets[i];
        while (current != NULL) {
            HashNode *temp = current;
            current = current->next;
            free(temp->key);
            free(temp);
        }
    }
    free(map->buckets);
    free(map);
}