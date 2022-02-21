#include "bv.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

struct BitVector {
    uint32_t length;
    uint8_t *vector;
};

// CITE: Prof Long, bv.h in Code Comments Repo
// The constructor for a bit vector that holds length bits.
BitVector *bv_create(uint32_t length) {
    BitVector *bv = (BitVector *) malloc(sizeof(BitVector));
    if (bv) {
        bv->vector = (uint8_t *) calloc(length, sizeof(uint8_t));
        bv->length = length;
    } else if (bv == NULL) { // return NULL if sufficient memory cannot be allocated.
        return NULL;
    } else {
        return (BitVector *) 0;
    }
    return bv;
}

// The destructor for a bit vector.
void bv_delete(BitVector **bv) {
    if (*bv && (*bv)->vector) {
        free((*bv)->vector);
        (*bv)->vector = NULL;
        free(*bv);
        *bv = NULL;
    }
}

// Returns the length of a bit vector.
uint32_t bv_length(BitVector *bv) {
    return bv->length;
}

// CITE: Prof Long, bv.h in Code Comments Repo
// Sets the ith bit in a bit vector. If i is out of range, return false.
// Otherwise, return true to indicate success.
bool bv_set_bit(BitVector *bv, uint32_t i) {
    if (i > bv_length(bv) || i < 0) {
        return false;
    }
    bv->vector[i / 8] |= (1 << (i % 8));
    return true;
}

// CITE: Prof Long, bv.h in Code Comments Repo
// Clears the ith bit in the bit vector. If i is out of range, return false.
// Otherwise, return true to indicate success.
bool bv_clr_bit(BitVector *bv, uint32_t i) {
    if (i > bv_length(bv) || i < 0) {
        return false;
    }
    bv->vector[i / 8] &= ~(1 << (i % 8));
    return true;
}

// CITE: Prof Long, bv.h in Code Comments Repo
// Returns the ith bit in the bit vector. If i is out of range, return false.
// Otherwise, return false if the value of bit i is 0 and return true if the value of bit i is 1.
bool bv_get_bit(BitVector *bv, uint32_t i) {
    if (i > bv_length(bv) || i < 0) {
        return false;
    }
    return (bv->vector[i / 8] >> i % 8) & 0x1;
}

// A debug function to print the bits of a bit vector.
void bv_print(BitVector *bv) {
    for (uint32_t i = 0; i < bv_length(bv); i++) {
        if (bv_get_bit(bv, i)) {
            printf("1");
        } else {
            printf("0");
        }
    }
}
