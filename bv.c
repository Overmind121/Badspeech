#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>

typedef struct BitVector {
    uint32_t length;
    uint8_t *vector;
} BitVector;

// Cited from Professor Long's bv8
BitVector *bv_create(uint32_t length) {
    BitVector *bv = (BitVector *) malloc(sizeof(BitVector));
    if (bv) {
        uint32_t bytes = length / 8 + (length % 8 ? 1 : 0);
        bv->vector = (uint8_t *) calloc(bytes, sizeof(uint8_t));
        bv->length = length;
        return bv;
    } else {
        return (BitVector *) 0;
    }
}

// Cited from Professor Long's bv8
void bv_delete(BitVector **bv) {
    if (bv && (*bv)->vector) {
        free((*bv)->vector);
    }
    if (bv) {
        free(*bv);
    }
    *bv = NULL;
    return;
}

// Cited from Professor Long's bv8
uint32_t bv_length(BitVector *bv) {
    return bv ? bv->length : 0;
}

// Cited from Professor Long's bv8
bool bv_set_bit(BitVector *bv, uint32_t i) {
    if (i <= bv_length(bv) && bv && bv->vector) {
        bv->vector[i / 8] |= (0x1 << i % 8);
        return true;
    }
    return false;
}

// Cited from Professor Long's bv8
bool bv_clr_bit(BitVector *bv, uint32_t i) {
    if (i <= bv_length(bv) && bv && bv->vector) {
        bv->vector[i / 8] &= ~(0x1 << i % 8);
        return true;
    }
    return false;
}

// Cited from Professor Long's bv8
bool bv_get_bit(BitVector *bv, uint32_t i) {
    if (i <= bv_length(bv) && bv && bv->vector) {
        return (bv->vector[i / 8] >> i % 8) & 0x1;
    }
    return false;
}

void bv_print(BitVector *bv) {
    for (uint32_t i = 0; i < 8; i++) {
        if (bv_get_bit(bv, i)) {
            printf("1");
        } else {
            printf("0");
        }
    }
    printf("\n");
    return;
}
