#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define HASHTABLE_IMPLEMENTATION
#include "hashtable.h"

#define LOGGER_IMPLEMENTATION
#include "logger.h"

struct Token {
    size_t line;
};

size_t hash_func(const char *str, size_t n);

int main(void) {
    FILE *fp = fopen("t8.shakespeare.txt", "r");
    LOGGER_ASSERT_MSG(fp != NULL, "Could not open t8.shakespeare.txt!");

    HashTable ht = {0};
    LOGGER_ASSERT_MSG(hashtable_create(&ht, 500, hash_func) == HASHTABLE_OK, "Could not create HashTable!");

    size_t line = 1;
    char str[HASHTABLE_MAX_VALUE_LEN];

    // Get first 100 lines of t8.shakespeare.txt
    for (int i = 0; i < 100 && fgets(str, HASHTABLE_MAX_VALUE_LEN, fp); ++i) {
        // Split line by ' '
        char *ptr = strtok(str, " ");
        while (ptr != NULL) {
            size_t ptr_len = strnlen(ptr, HASHTABLE_MAX_VALUE_LEN);
            if (ptr[ptr_len-1] == '\n') {
                ptr[ptr_len-1] = '\0';
            }

            struct Token *t = calloc(1, sizeof(struct Token));
            LOGGER_ASSERT_MSG(t != NULL, "Failed to create Token (no mem)!");
            t->line = line;
            LOGGER_ASSERT_MSG(hashtable_insert(&ht, ptr, t) == HASHTABLE_OK, "Failed to insert Token into HashTable!");

            ptr = strtok(NULL, " ");
        }
        line++;
    }
    fclose(fp);

    struct Token *t = hashtable_remove(&ht, "VERSION");
    if (t != NULL) free(t);

    t = hashtable_get(&ht, "VERSION");
    LOGGER_ASSERT_MSG(t == NULL, "Failed hashtable_get");

    hashtable_destroy(&ht);
    return 0;
}

size_t hash_func(const char *str, size_t n) {
    size_t result = 0;
    int c;
    while ((c = *str++)) {
        result = (result * 42) + c;
    }
    return result % n;
}
