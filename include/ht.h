#ifndef HT_H
#define HT_H

#include <stddef.h>

#ifndef HT_MAX_DIGEST_LEN
#define HT_MAX_DIGEST_LEN 32
#endif // !HT_MAX_DIGEST_LEN

#ifndef HT_MAX_KEY_LEN
#define HT_MAX_KEY_LEN 256
#endif // !HT_MAX_KEY_LEN

typedef unsigned char *(*ht_hash_function)(const char *, size_t);

typedef struct s_ht_state {
    void **data;
    size_t data_len;
    ht_hash_function hash_func;
} ht_state;

typedef enum e_ht_code {
    HT_CODE_SUCCESS = 0,
    HT_CODE_ERROR_INVALID_INPUT = 1,
    HT_CODE_ERROR_HASH_FUNC = 2,
    HT_CODE_ERROR_TABLE_FULL = 3,
} ht_code;

ht_code ht_insert(ht_state *state, const char *key, void *value);
void *ht_get_value(ht_state *state, const char *key);
const char *ht_code_to_str(ht_code code);

#ifdef HT_IMPLEMENTATION

static size_t ht_str_len(const char *str) {
    size_t len = 0;
    while (str[len] != '\0' && len <= HT_MAX_KEY_LEN) {
        len++;
    }
    return len;
}

static size_t ht_compute_index(unsigned char *hash, size_t data_len) {
    size_t sum = 0;
    size_t n = 0;
    while (hash[n] != '\0' && n <= HT_MAX_DIGEST_LEN) {
        sum += hash[n];
        n++;
    }
    return sum % data_len;
}

ht_code ht_insert(ht_state *state, const char *key, void *value) {
    if (state == NULL || key == NULL || value == NULL || state->data_len == 0 ||
        state->hash_func == NULL)
        return HT_CODE_ERROR_INVALID_INPUT;

    size_t len = ht_str_len(key);
    unsigned char *hash = state->hash_func(key, len);
    if (hash == NULL) {
        return HT_CODE_ERROR_HASH_FUNC;
    }

    size_t idx = ht_compute_index(hash, state->data_len);
    state->data[idx] = value;
    return HT_CODE_SUCCESS;
}

void *ht_get_value(ht_state *state, const char *key) {
    if (state == NULL || key == NULL || state->data_len == 0 ||
        state->hash_func == NULL)
        return NULL;

    size_t len = ht_str_len(key);
    unsigned char *hash = state->hash_func(key, len);
    if (hash == NULL) {
        return NULL;
    }

    size_t idx = ht_compute_index(hash, state->data_len);
    return state->data[idx];
}

const char *ht_code_to_str(ht_code code) {
    switch (code) {
        case HT_CODE_SUCCESS: return "SUCCESS";
        case HT_CODE_ERROR_INVALID_INPUT: return "ERROR: INVALID_INPUT";
        case HT_CODE_ERROR_HASH_FUNC: return "ERROR: HASH_FUNC";
        case HT_CODE_ERROR_TABLE_FULL: return "ERROR: TABLE_FULL";
        default: return "UNKNOWN";
    }
}

#endif // HT_IMPLEMENTATION
#endif // !HT_H
