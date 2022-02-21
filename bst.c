#include "bst.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// Global Variable branches
uint64_t branches = 0;

// Constructor for a binary search tree that constructs an empty tree.
Node *bst_create(void) {
    return NULL;
}

// CITE: Prof Long, Lecture 18 slide 76
// Destructor for a binary search tree rooted at root.
void bst_delete(Node **root) {
    if (*root) {
        bst_delete(&(*root)->left);
        bst_delete(&(*root)->right);
        node_delete(root);
    }
}

// CITE: Prof Long, Lecture 18 slide 55
// Helper function for bst_height
// Returns max of x and y.
static uint32_t max(uint32_t x, uint32_t y) {
    return x > y ? x : y;
}

// CITE: Prof Long, Lecture 18 slide 55
// Returns the height of the binary search tree rooted at root.
uint32_t bst_height(Node *root) {
    if (root) {
        return 1 + max(bst_height(root->left), bst_height(root->right));
    }
    return 0;
}

// Returns the size of the binary search tree rooted at root.
uint32_t bst_size(Node *root) {
    if (root == NULL) { // tree is empty
        return 0;
    } else { // recursively call to left and right child and add 1 (the root)
        return 1 + bst_size(root->left) + bst_size(root->right);
    }
}

// CITE: Prof Long, Lecture 18 slide 57
// Searches for a node containing oldspeak in the binary search tree rooted at root.
// If a node is found, the pointer to the node is returned. Else, a NULL pointer is returned.
Node *bst_find(Node *root, char *oldspeak) {
    if (root) {
        int compare = strcmp(root->oldspeak, oldspeak);
        if (compare > 0) { // Current node’s oldspeak is greater than oldspeak.
            branches++;
            return bst_find(root->left, oldspeak);
        } else if (compare < 0) { // Current node’s oldspeak is less than oldspeak.
            branches++;
            return bst_find(root->right, oldspeak);
        }
    }
    return root; // Current node’s key matches key.
}

// CITE: Eugene, Section 11/30/21
// Inserts a new node containing the specified oldspeak and newspeak
// into the binary search tree rooted at root.
Node *bst_insert(Node *root, char *oldspeak, char *newspeak) {
    if (root == NULL) { // tree is empty
        return node_create(oldspeak, newspeak);
    } // tree is non-empty
    int compare = strcmp(root->oldspeak, oldspeak);
    if (compare > 0) { // Current node’s oldspeak is greater than oldspeak.
        branches++;
        root->left = bst_insert(root->left, oldspeak, newspeak);
    } else if (compare < 0) { // Current node’s oldspeak is less than oldspeak.
        branches++;
        root->right = bst_insert(root->right, oldspeak, newspeak);
    }
    return root;
}

// CITE: Prof Long, Lecture 18 slide 22
// Prints out each node in the binary search tree through an inorder traversal.
void bst_print(Node *root) {
    if (root) {
        bst_print(root->left);
        node_print(root);
        bst_print(root->right);
    }
}
