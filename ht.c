#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

#include "node.h"
#include "bst.h"
#include "salts.h"
#include "speck.h"

uint64_t lookups = 0;
typedef struct {
    uint64_t salt[2];
    uint32_t size;
    Node **trees;
} HashTable;

HashTable *ht_create(uint32_t size) {
    HashTable *ht = (HashTable *) malloc(sizeof(HashTable));
    if (ht) {
        ht->size = size;
        ht->salt[0] = SALT_HASHTABLE_LO;
        ht->salt[1] = SALT_HASHTABLE_HI;
        ht->trees = (Node **) calloc(size, sizeof(Node *));
        if (!ht->trees) {
            free(ht);
            ht = NULL;
        }
    }

    return ht;
}

void ht_delete(HashTable **ht) {
    if ((*ht && (*ht)->trees)) {
        for (uint32_t i = 0; i < (*ht)->size; i++) {
            bst_delete(&(*ht)->trees[i]);
        }
        free((*ht)->trees);
        free(*ht);
        *ht = NULL;
    }
    return;
}

uint32_t ht_size(HashTable *ht) {
    return ht->size;
}

Node *ht_lookup(HashTable *ht, char *oldspeak) {
    uint32_t index = hash(ht->salt, oldspeak);
    index = index % ht->size;
    lookups += 1;
    Node *n = bst_find(ht->trees[index], oldspeak);
    return n;
}

void ht_insert(HashTable *ht, char *oldspeak, char *newspeak) {
    if (ht) {
        uint32_t index = 0;
        lookups += 1;
        index = hash(ht->salt, oldspeak) % ht->size;
        ht->trees[index] = bst_insert(ht->trees[index], oldspeak, newspeak);
    }
    return;
}

uint32_t ht_count(HashTable *ht) {
    uint32_t count = 0;
    for (uint32_t i = 0; i < ht->size; i++) {
        if (ht->trees[i]) {
            count += 1;
        }
    }
    return count;
}

double ht_avg_bst_size(HashTable *ht) {
    double sum = 0.0;
    double denom = 0.0;
    double avg = 0.0;
    for (uint32_t i = 0; i < ht->size; i++) {
        sum += bst_size(ht->trees[i]);
        if (ht->trees[i]) {
            denom += 1;
        }
    }
    avg = sum / denom;
    return avg;
}

double ht_avg_bst_height(HashTable *ht) {
    double sum = 0.0;
    double denom = 0.0;
    double avg = 0.0;
    for (uint32_t i = 0; i < ht->size; i++) {
        sum += bst_height(ht->trees[i]);
        if (ht->trees[i]) {
            denom += 1;
        }
    }
    avg = sum / denom;
    return avg;
}

void ht_print(HashTable *ht) {
    printf("------------HashTable------------\n");
    for (uint32_t i = 0; i < ht->size; i++) {
        if (ht->trees[i]) {
            printf("-------------------------------\n");
            bst_print(ht->trees[i]);
        }
    }
    printf("-------------------------------\n");
    return;
}
