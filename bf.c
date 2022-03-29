#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

#include "salts.h"
#include "bv.h"
#include "speck.h"
typedef struct BloomFilter {
    uint64_t primary[2];
    uint64_t secondary[2];
    uint64_t tertiary[2];
    BitVector *filter;
} BloomFilter;

BloomFilter *bf_create(uint32_t size) {
    BloomFilter *bf = (BloomFilter *) malloc(sizeof(BloomFilter));
    bf->primary[0] = SALT_PRIMARY_LO;
    bf->primary[1] = SALT_PRIMARY_HI;
    bf->secondary[0] = SALT_SECONDARY_LO;
    bf->secondary[1] = SALT_SECONDARY_HI;
    bf->tertiary[0] = SALT_TERTIARY_LO;
    bf->tertiary[1] = SALT_TERTIARY_HI;
    bf->filter = bv_create(size);
    return bf;
}

void bf_delete(BloomFilter **bf) {
    bv_delete(&(*bf)->filter);
    free(*bf);
    *bf = NULL;
    return;
}

uint32_t bf_size(BloomFilter *bf) {
    return bv_length(bf->filter);
}

void bf_insert(BloomFilter *bf, char *oldspeak) {
    uint32_t hashed_oldspeak = 0;
    hashed_oldspeak = hash(bf->primary, oldspeak);
    hashed_oldspeak = hashed_oldspeak % bv_length(bf->filter);
    bv_set_bit(bf->filter, hashed_oldspeak);

    hashed_oldspeak = hash(bf->secondary, oldspeak);
    hashed_oldspeak = hashed_oldspeak % bv_length(bf->filter);
    bv_set_bit(bf->filter, hashed_oldspeak);

    hashed_oldspeak = hash(bf->tertiary, oldspeak);
    hashed_oldspeak = hashed_oldspeak % bv_length(bf->filter);
    bv_set_bit(bf->filter, hashed_oldspeak);
    return;
}

bool bf_probe(BloomFilter *bf, char *oldspeak) {
    uint32_t hashed_oldspeak = 0;
    hashed_oldspeak = hash(bf->primary, oldspeak);
    hashed_oldspeak = hashed_oldspeak % bv_length(bf->filter);
    if (!bv_get_bit(bf->filter, hashed_oldspeak)) {
        return false;
    }

    hashed_oldspeak = hash(bf->secondary, oldspeak);
    hashed_oldspeak = hashed_oldspeak % bv_length(bf->filter);
    if (!bv_get_bit(bf->filter, hashed_oldspeak)) {
        return false;
    }

    hashed_oldspeak = hash(bf->tertiary, oldspeak);
    hashed_oldspeak = hashed_oldspeak % bv_length(bf->filter);
    if (!bv_get_bit(bf->filter, hashed_oldspeak)) {
        return false;
    }
    return true;
}

uint32_t bf_count(BloomFilter *bf) {
    uint32_t count = 0;
    for (uint32_t i = 0; i < bf_size(bf); i++) {
        if (bv_get_bit(bf->filter, i)) {
            count += 1;
        }
    }
    return count;
}

void bf_print(BloomFilter *bf) {
    return;
}
