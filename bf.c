#include "bf.h"
#include "bv.h"
#include "speck.h"
#include "salts.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct BloomFilter {
    uint64_t primary[2]; // Primary hash function salt.
    uint64_t secondary[2]; // Secondary hash function salt.
    uint64_t tertiary[2]; // Tertiary hash function salt.
    BitVector *filter;
};

// The constructor for a Bloom filter.
BloomFilter *bf_create(uint32_t size) {
    BloomFilter *bf = (BloomFilter *) malloc(sizeof(BloomFilter));
    if (bf) {
        bf->primary[0] = SALT_PRIMARY_LO;
        bf->primary[1] = SALT_PRIMARY_HI;
        bf->secondary[0] = SALT_SECONDARY_LO;
        bf->secondary[1] = SALT_SECONDARY_HI;
        bf->tertiary[0] = SALT_TERTIARY_LO;
        bf->tertiary[1] = SALT_TERTIARY_HI;
        bf->filter = bv_create(size);
        if (!bf->filter) {
            free(bf);
            bf = NULL;
        }
    }
    return bf;
}

// The destructor for a Bloom filter.
void bf_delete(BloomFilter **bf) {
    if (*bf) {
        bv_delete(&(*bf)->filter);
        free(*bf);
        *bf = NULL;
    }
}

// Returns the size of the Bloom filter.
uint32_t bf_size(BloomFilter *bf) {
    return bv_length(bf->filter);
}

// Takes oldspeak and inserts it into the Bloom filter
void bf_insert(BloomFilter *bf, char *oldspeak) {
    // Hash oldspeak with primary salt and set bit at that index.
    bv_set_bit(bf->filter, hash(bf->primary, oldspeak) % bv_length(bf->filter));
    // Hash oldspeak with secondary salt and set bit at that index.
    bv_set_bit(bf->filter, hash(bf->secondary, oldspeak) % bv_length(bf->filter));
    // Hash oldspeak with tertiary salt and set bit at that index.
    bv_set_bit(bf->filter, hash(bf->tertiary, oldspeak) % bv_length(bf->filter));
}

// Probes the Bloom filter for oldspeak.
bool bf_probe(BloomFilter *bf, char *oldspeak) {
    bool primary = bv_get_bit(bf->filter, hash(bf->primary, oldspeak) % bv_length(bf->filter));
    bool secondary = bv_get_bit(bf->filter, hash(bf->secondary, oldspeak) % bv_length(bf->filter));
    bool tertiary = bv_get_bit(bf->filter, hash(bf->tertiary, oldspeak) % bv_length(bf->filter));
    if (primary && secondary && tertiary) { // If all three bits are set
        return true;
    }
    return false;
}

// Returns the number of set bits in the Bloom filter.
uint32_t bf_count(BloomFilter *bf) {
    uint32_t count = 0;
    for (uint32_t i = 0; i < bv_length(bf->filter); i++) { // Loop through the bloom filter.
        if (bv_get_bit(bf->filter, i)) { // If bit is set, increment count.
            count++;
        }
    }
    return count;
}

// A debug function to print out the bits of a Bloom filter.
void bf_print(BloomFilter *bf) {
    bv_print(bf->filter);
}
