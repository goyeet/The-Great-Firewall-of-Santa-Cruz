#include "bf.h"
#include "bst.h"
#include "ht.h"
#include "messages.h"
#include "node.h"
#include "parser.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <inttypes.h>
#include <regex.h>
#include <ctype.h>

#define OPTIONS "hst:f:"
#define WORD    "((')*[a-zA-Z0-9_]+(('|-)[a-zA-Z0-9_]*)*)"

// prints help/synopsis page to stderr
static void help() {
    fprintf(stderr, "SYNOPSIS\n");
    fprintf(stderr, "  A word filtering program for the GPRSC.\n");
    fprintf(stderr, "  Filters out and reports bad words parsed from stdin.\n\n");
    fprintf(stderr, "USAGE\n");
    fprintf(stderr, "  ./banhammer [-hs] [-t size] [-f size]\n\n");
    fprintf(stderr, "OPTIONS\n");
    fprintf(stderr, "  -h           Program usage and help.\n");
    fprintf(stderr, "  -s           Print program statistics.\n");
    fprintf(stderr, "  -t size      Specify hash table size (default: 2^16).\n");
    fprintf(stderr, "  -f size      Specify Bloom filter size (default: 2^20).\n");
}

// driver code of program
int main(int argc, char **argv) {
    bool statistics = false;
    uint32_t hash_table_size = (uint32_t) pow(2, 16); // Default size of hash table (2^16)
    uint32_t bloom_filter_size = (uint32_t) pow(2, 20); // Default size of bloom filter (2^20)
    int32_t opt = 0;

    // CITE: Regex taken from asgn7 pdf
    regex_t re;
    if (regcomp(&re, WORD, REG_EXTENDED)) {
        fprintf(stderr, "Failed to compile regex.\n");
        return 1;
    }

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': help(); return 1;
        case 's': statistics = true; break;
        case 't':
            hash_table_size = strtoul(optarg, NULL, 10); // Set hash table size to user input
            break;
        case 'f':
            bloom_filter_size = strtoul(optarg, NULL, 10); // Set bloom filter size to user input
            break;
        default: help(); return 1;
        }
    }

    // Initialize Bloom filter and hash table.
    BloomFilter *bf = bf_create(bloom_filter_size);
    HashTable *ht = ht_create(hash_table_size);

    int temp;
    char badspeak[1024];
    // Read in a list of badspeak words with fscanf().
    FILE *badspeak_file = fopen("badspeak.txt", "r");
    if (!badspeak_file) {
        fprintf(stderr, "Failed to open badspeak.txt.\n");
    }
    while ((temp = fscanf(badspeak_file, "%s", badspeak)) != EOF) {
        bf_insert(bf, badspeak); // Insert badspeak word into bloom filter
        ht_insert(ht, badspeak, NULL); // Insert badspeak word into hash table
    }

    char oldspeak[1024];
    char newspeak[1024];
    // Read in a list of oldspeak and newspeak pairs with fscanf().
    FILE *newspeak_file = fopen("newspeak.txt", "r");
    if (!newspeak_file) {
        fprintf(stderr, "Failed to open newspeak.txt.\n");
    }
    while ((temp = fscanf(newspeak_file, "%s %s", oldspeak, newspeak)) != EOF) {
        bf_insert(bf, oldspeak); // Insert oldspeak word into bloom filter
        ht_insert(ht, oldspeak, newspeak); // Insert oldspeak and newspeak words into hash table
    }

    // Read words in from stdin using the supplied parsing module (CITE: Assignment 7 pdf).
    char *word = NULL;
    Node *badspeak_list = bst_create();
    Node *oldspeak_list = bst_create();
    while ((word = next_word(stdin, &re)) != NULL) {
        for (int i = 0; word[i]; i++) { // for every character in word
            // Convert entire word to lowercase in case word is typed in uppercase
            word[i] = tolower(word[i]);
        }
        // For each word that is read in, check to see if it has been added to the Bloom filter.
        bool in_bloom_filter = bf_probe(bf, word);
        if (in_bloom_filter) { // if word is in bloom filter
            Node *n = ht_lookup(ht, word);
            if (n != NULL) { // Hash table contains word
                // If the hash table contains the word and the word does not have a newspeak translation
                if ((n->oldspeak != NULL) && (n->newspeak == NULL)) {
                    badspeak_list = bst_insert(badspeak_list, n->oldspeak, n->newspeak);
                } // If the hash table contains the word, and the word does have a newspeak translation.
                else if ((n->oldspeak != NULL) && (n->newspeak != NULL)) {
                    oldspeak_list = bst_insert(oldspeak_list, n->oldspeak, n->newspeak);
                }
            }
        }
    }

    // If statistics are enabled, disable all other messages the program may print.
    if (statistics) {
        double avg_branches_traversed = (double) branches / (double) lookups;
        double ht_load = 100 * ((double) ht_count(ht) / ht_size(ht));
        double bf_load = 100 * ((double) bf_count(bf) / bf_size(bf));
        printf("Average BST size: %.6f\n", ht_avg_bst_size(ht));
        printf("Average BST height: %.6f\n", ht_avg_bst_height(ht));
        printf("Average branches traversed: %.6f\n", avg_branches_traversed);
        printf("Hash table load: %.6f%%\n", ht_load);
        printf("Bloom filter load: %.6f%%\n", bf_load);
    } else {
        // If the citizen is accused of thoughtcrime and requires counseling on proper Rightspeak
        if ((bst_size(badspeak_list) > 0) && (bst_size(oldspeak_list) > 0)) {
            printf("%s", mixspeak_message);
            bst_print(badspeak_list); // Print entirety of badspeak_list
            bst_print(oldspeak_list); // Print entirety of oldspeak_list
        }

        // If the citizen is accused solely of thoughtcrime
        if ((bst_size(badspeak_list) > 0) && (bst_size(oldspeak_list) == 0)) {
            printf("%s", badspeak_message);
            bst_print(badspeak_list); // Print entirety of badspeak_list
        }

        // If the citizen only requires counseling
        if ((bst_size(badspeak_list) == 0) && (bst_size(oldspeak_list) > 0)) {
            printf("%s", goodspeak_message);
            bst_print(oldspeak_list); // Print entirety of oldspeak_list
        }
    }

    // Close files, delete data structures, and free any memory that was used.
    fclose(badspeak_file);
    fclose(newspeak_file);

    bf_delete(&bf);
    ht_delete(&ht);
    bst_delete(&badspeak_list);
    bst_delete(&oldspeak_list);
    clear_words();
    regfree(&re);

    return 0;
}
