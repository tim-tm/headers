#define HT_IMPLEMENTATION
#include "ht.h"

#define DATA_LEN 512

unsigned char *hash_function(const char *key, size_t len) {
    if (key == NULL)
        return NULL;

    static unsigned char digest[HT_MAX_DIGEST_LEN];
    for (size_t i = 0; i < len - 1; i++) {
        digest[i] = key[i] + key[i + 1];
    }
    return digest;
}

static int *data[DATA_LEN];
static ht_state table = {
    .data = (void **)data, .data_len = DATA_LEN, .hash_func = hash_function};

ht_state *get_table(void) { return &table; }
