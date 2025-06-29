#ifndef HT_H
#define HT_H

#include <stdint.h>

#ifndef HT_MAX_DIGEST_LEN
#define HT_MAX_DIGEST_LEN 32
#endif // !HT_MAX_DIGEST_LEN

#ifndef HT_MAX_KEY_LEN
#define HT_MAX_KEY_LEN 256
#endif // !HT_MAX_KEY_LEN

typedef uint32_t (*ht_hash_function)(const char *, uint32_t);

typedef struct s_ht_state {
    void **data;
    uint32_t data_len;
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

static uint32_t ht_str_len(const char *str) {
    uint32_t len = 0;
    while (len < HT_MAX_KEY_LEN && str[len] != '\0') {
        len++;
    }
    return len;
}

ht_code ht_insert(ht_state *state, const char *key, void *value) {
    if (state == NULL || key == NULL || value == NULL || state->data_len == 0 ||
        state->hash_func == NULL)
        return HT_CODE_ERROR_INVALID_INPUT;

    uint32_t len = ht_str_len(key);
    uint32_t hash = state->hash_func(key, len);
    uint32_t idx = hash %= state->data_len;

    // TODO: Implement collision handling

    state->data[idx] = value;
    return HT_CODE_SUCCESS;
}

void *ht_get_value(ht_state *state, const char *key) {
    if (state == NULL || key == NULL || state->data_len == 0 ||
        state->hash_func == NULL)
        return NULL;

    uint32_t len = ht_str_len(key);
    uint32_t hash = state->hash_func(key, len);
    uint32_t idx = hash %= state->data_len;

    // TODO: Implement collision handling

    return state->data[idx];
}

const char *ht_code_to_str(ht_code code) {
    switch (code) {
    case HT_CODE_SUCCESS:
        return "SUCCESS";
    case HT_CODE_ERROR_INVALID_INPUT:
        return "ERROR: INVALID_INPUT";
    case HT_CODE_ERROR_HASH_FUNC:
        return "ERROR: HASH_FUNC";
    case HT_CODE_ERROR_TABLE_FULL:
        return "ERROR: TABLE_FULL";
    default:
        return "UNKNOWN";
    }
}

#endif // HT_IMPLEMENTATION
#endif // !HT_H
