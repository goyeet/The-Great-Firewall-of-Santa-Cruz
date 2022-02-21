#include "node.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// creates a node
Node *node_create(char *oldspeak, char *newspeak) {
    Node *n = (Node *) malloc(sizeof(Node));
    if (n) {
        n->left = NULL;
        n->right = NULL;
        if (oldspeak == NULL) { // No oldspeak passed in
            n->oldspeak = NULL;
        } else { // Oldspeak passed in
            n->oldspeak = strdup(oldspeak);
        }
        if (newspeak == NULL) { // No newspeak passed in
            n->newspeak = NULL;
        } else { // Newspeak passd in
            n->newspeak = strdup(newspeak);
        }
    }
    if (!n) {
        free(n);
        n = NULL;
    }
    return n;
}

// frees memory allocated to a node
void node_delete(Node **n) {
    if (*n) {
        free((*n)->newspeak);
        (*n)->newspeak = NULL;
        free((*n)->oldspeak);
        (*n)->oldspeak = NULL;
        free(*n);
        *n = NULL;
    }
}

// print node for debugging
void node_print(Node *n) {
    if (n->newspeak == NULL) { // node lacks newspeak
        printf("%s\n", n->oldspeak);
    } else {
        printf("%s -> %s\n", n->oldspeak, n->newspeak);
    }
}
