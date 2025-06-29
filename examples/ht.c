#include <openssl/crypto.h>
#include <openssl/sha.h>
#include <stdio.h>

#define HT_MAX_DIGEST_LEN SHA512_DIGEST_LENGTH
#define HT_IMPLEMENTATION
#include "ht.h"

#define DATA_LEN 256

uint32_t hash_function(const char *key, uint32_t len) {
    if (key == NULL)
        return 0;

    unsigned char digest[SHA512_DIGEST_LENGTH];
    if (SHA512((unsigned char *)key, len, digest) == NULL)
        return 0;

    uint32_t hash = 0;
    for (uint32_t i = 0; i < SHA512_DIGEST_LENGTH; i++) {
        hash += digest[i];
    }
    return hash;
}

int main(void) {
    static int *data[DATA_LEN];
    ht_state table = {.data = (void **)data,
                      .data_len = DATA_LEN,
                      .hash_func = hash_function};

    int hello = 1337;
    ht_code code;
    if ((code = ht_insert(&table, "Hello", &hello)) != HT_CODE_SUCCESS) {
        printf("Failed to insert into hashtable: %s\n", ht_code_to_str(code));
        return 1;
    }
    printf("Successfully inserted %d into hashtable!\n", hello);

    void *result = ht_get_value(&table, "Hello");
    if (result == NULL) {
        printf("Failed to get value from hashtable!\n");
        return 1;
    }
    int get_hello = *((int *)result);
    printf("Got value from hashtable: %d\n", get_hello);
    return 0;
}
