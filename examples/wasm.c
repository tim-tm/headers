#include <stddef.h>

#define HT_IMPLEMENTATION
#include "ht.h"

#define DATA_LEN 512

uint32_t hash_function(const char *key, uint32_t len) {
    if (key == NULL)
        return 0;

    // Implementation of Daniel J. Bernstein's djb2-hash
    // (https://theartincode.stanis.me/008-djb2/)

    uint32_t hash = 5381;
    for (uint32_t i = 0; i < len - 1; i++) {
        hash = ((hash << 5) + hash) + key[i];
    }
    return hash;
}

static int *data[DATA_LEN];
static ht_state table = {
    .data = (void **)data, .data_len = DATA_LEN, .hash_func = hash_function};

ht_state *get_table(void) { return &table; }
