#include "ht.h"
#include "speck.h"
#include "salts.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

// Global Variable lookups
uint64_t lookups = 0;

struct HashTable {
    uint64_t salt[2];
    uint32_t size;
    Node **trees;
};

// The constructor for a hash table.
HashTable *ht_create(uint32_t size) {
    HashTable *ht = (HashTable *) malloc(sizeof(HashTable));
    if (ht) {
        ht->salt[0] = SALT_HASHTABLE_LO;
        ht->salt[1] = SALT_HASHTABLE_HI;
        ht->size = size;
        ht->trees = (Node **) calloc(size, sizeof(Node *));
        if (!ht->trees) {
            free(ht);
            ht = NULL;
        }
    }
    return ht;
}

// The destructor for a hash table.
void ht_delete(HashTable **ht) {
    if (*ht) {
        for (uint32_t i = 0; i < ht_size((*ht)); i++) {
            bst_delete(&(*ht)->trees[i]);
        }
        free((*ht)->trees);
        (*ht)->trees = NULL;
        free((*ht));
        *ht = NULL;
    }
}

// Returns the hash tables size.
uint32_t ht_size(HashTable *ht) {
    return ht->size;
}

// Searches for an entry, a node, in the hash table that contains oldspeak.
Node *ht_lookup(HashTable *ht, char *oldspeak) {
    lookups++;
    uint32_t index = hash(ht->salt, oldspeak) % ht_size(ht);
    if (ht->trees[index] == NULL) {
        return NULL;
    } else {
        return bst_find(ht->trees[index], oldspeak);
    }
}

// Inserts the specified oldspeak and its corresponding newspeak translation into the hash table.
void ht_insert(HashTable *ht, char *oldspeak, char *newspeak) {
    lookups++;
    uint32_t index = hash(ht->salt, oldspeak) % ht_size(ht);
    if (ht->trees[index] == NULL) {
        ht->trees[index] = bst_create();
    }
    ht->trees[index] = bst_insert(ht->trees[index], oldspeak, newspeak);
}

// Returns the number of non-NULL binary search trees in the hash table.
uint32_t ht_count(HashTable *ht) {
    uint32_t count = 0;
    for (uint32_t i = 0; i < ht_size(ht); i++) { // Loop through hash table
        if (ht->trees[i] != NULL) { // If BST is non-NULL, increment count
            count++;
        }
    }
    return count;
}

// Returns the average binary search tree size.
double ht_avg_bst_size(HashTable *ht) {
    uint32_t sum_of_sizes = 0;
    for (uint32_t i = 0; i < ht_size(ht); i++) { // Loop through hash table
        if (ht->trees[i] != NULL) {
            sum_of_sizes += bst_size(ht->trees[i]); // Add size of current BST to sum
        }
    }
    return (double) sum_of_sizes / ht_count(ht);
}

// Returns the average binary search tree height.
double ht_avg_bst_height(HashTable *ht) {
    uint32_t sum_of_heights = 0;
    for (uint32_t i = 0; i < ht_size(ht); i++) { // Loop through hash table
        if (ht->trees[i] != NULL) {
            sum_of_heights += bst_height(ht->trees[i]); // Add height of current BST to sum
        }
    }
    return (double) sum_of_heights / ht_count(ht);
}

// A debug function to print out the contents of a hash table.
void ht_print(HashTable *ht) {
    for (uint32_t i = 0; i < ht_size(ht); i++) { // Loop through hash table
        if (ht->trees[i]) {
            bst_print(ht->trees[i]);
        }
    }
}
