/**
 *  hashtable.h
 *
 *  hashtable.h is a simple STB-Style library for dealing with hashtables in C.
 *  
 *  If you want the implementation, this should be defined once:
 *      #define HASHTABLE_IMPLEMENTATION
 *      #include <hashtable.h>
 *
 *
 *  MIT License
 *
 *  Copyright (c) 2024 Tim Teichmann
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <stddef.h>
#include <stdbool.h>

#ifndef HASHTABLE_MAX_KEY_LEN
#define HASHTABLE_MAX_KEY_LEN 1024
#endif // !HASHTABLE_MAX_KEY_LEN

#ifndef HASHTABLE_MAX_VALUE_LEN
#define HASHTABLE_MAX_VALUE_LEN 2048
#endif // !HASHTABLE_MAX_VALUE_LEN

typedef size_t (*hash_function)(const char *string, size_t table_size);

typedef struct _HashTableElement_ {
    char *key;
    void *value;
    size_t hash;
    bool defined;
    size_t occurrences;
} HashTableElement;

typedef struct _HashTable_ {
    HashTableElement *elements;
    size_t size;
    hash_function hash_func;
} HashTable;

typedef enum _HashTableReturn_ {
    // One or more input was invalid (NULL)
    HASHTABLE_ERROR_INVALID_INPUT,

    // Could not allocate memory
    HASHTABLE_ERROR_NO_MEMORY,

    // The hashfunction returned invalid output (hash was bigger than table_size)
    // If the hashfunction is somehow NULL, HASHTABLE_ERROR_INVALID_INPUT will be used.
    HASHTABLE_ERROR_HASH_FUNCTION,
    
    // The table is full, try making it larger
    HASHTABLE_ERROR_TABLE_OVERFLOW,
    
    // Everything is okay
    HASHTABLE_OK
} HashTableReturn;

/**
 *  hashtable_create
 *
 *  Creates an empty hashtable with the size of 'table_size' using the hashing function 'hash_func'.
 *
 *  Return:
 *      - HashTableReturn corresponding to the error occourred or OK
 */
HashTableReturn hashtable_create(HashTable *table, size_t table_size, hash_function hash_func);

/**
 *  hashtable_destroy
 *
 *  Free's internal memory.
 *  The HashTable pointer 'table' is NOT being free'd because
 *  it might be allocated on the stack.
 *  This calls free on every element (also on the void *value) that was added via. hashtable_insert.
 */
void hashtable_destroy(HashTable *table);

/**
 *  hashtable_insert
 *
 *  Insert an element of the value 'data' with the key 'key' into the hashtable.
 *  Return:
 *      - HashTableReturn corresponding to the error occourred or OK
 */
HashTableReturn hashtable_insert(HashTable *table, const char *key, void *value);

/**
 *  hashtable_get
 *
 *  Get the element corresponding to the key 'key' from the hashtable.
 *  Return:
 *      - the void* that was provided at hashtable_insert on success
 *      - NULL on failure
 */
void *hashtable_get(HashTable *table, const char *key);

/**
 *  hashtable_remove
 *
 *  Remove an element from the hashtable.
 *  Return:
 *      - pointer to the element on success
 *      - NULL on failure
 */
void *hashtable_remove(HashTable *table, const char *key);

#ifdef HASHTABLE_IMPLEMENTATION
#include <stdlib.h>
#include <string.h>

HashTableReturn hashtable_create(HashTable *table, size_t table_size, hash_function hash_func) {
    if (table == NULL || hash_func == NULL) return HASHTABLE_ERROR_INVALID_INPUT;

    table->size = table_size;
    table->hash_func = hash_func;
    table->elements = calloc(table_size, sizeof(HashTableElement));
    if (table->elements == NULL) return HASHTABLE_ERROR_NO_MEMORY;
    return HASHTABLE_OK;
}

void hashtable_destroy(HashTable *table) {
    if (table == NULL || table->elements == NULL) return;

    for (size_t i = 0; i < table->size; ++i) {
        if (table->elements[i].defined) {
            if (table->elements[i].key != NULL) free(table->elements[i].key);
            if (table->elements[i].value != NULL) free(table->elements[i].value);
        }
    }
    free(table->elements);
}

HashTableReturn hashtable_insert(HashTable *table, const char *key, void *value) {
    if (table == NULL || table->elements == NULL || table->hash_func == NULL || key == NULL || value == NULL) return HASHTABLE_ERROR_INVALID_INPUT;

    size_t h = table->hash_func(key, table->size);
    // NOTE: h < 0 is impossible because h is a size_t (aka. unsigned long)
    if (h >= table->size) return HASHTABLE_ERROR_HASH_FUNCTION;

    for (size_t i = 0; i < table->size && table->elements[h].defined && strncmp(key, table->elements[h].key, HASHTABLE_MAX_KEY_LEN) != 0; ++i) {
        h = (h + 1) % table->size;
    }

    if (table->elements[h].defined) {
        if (strncmp(key, table->elements[h].key, HASHTABLE_MAX_KEY_LEN) != 0) {
            return HASHTABLE_ERROR_TABLE_OVERFLOW;
        }
        table->elements[h].occurrences++;
    } else {
        table->elements[h].key = calloc(HASHTABLE_MAX_KEY_LEN, sizeof(char));
        if (table->elements[h].key == NULL) return false;
        strncpy(table->elements[h].key, key, HASHTABLE_MAX_KEY_LEN);

        table->elements[h].value = value;
        table->elements[h].hash = h;
        table->elements[h].occurrences = 1;
        table->elements[h].defined = true;
    }
    return HASHTABLE_OK;
}

void *hashtable_get(HashTable *table, const char *key) {
    if (table == NULL || table->elements == NULL || key == NULL) return NULL;
    
    size_t h = table->hash_func(key, table->size);
    // NOTE: h < 0 is impossible because h is a size_t (aka. unsigned long)
    if (h >= table->size) return NULL;

    if (table->elements[h].defined) {
        return table->elements[h].value;
    }
    return NULL;
}

void *hashtable_remove(HashTable *table, const char *key) {
    if (table == NULL || table->elements == NULL || key == NULL) return NULL;
    
    size_t h = table->hash_func(key, table->size);
    // NOTE: h < 0 is impossible because h is a size_t (aka. unsigned long)
    if (h >= table->size) return NULL;

    if (table->elements[h].defined) {
        table->elements[h].defined = false;
        return table->elements[h].value;
    }
    return NULL;
}

#endif // HASHTABLE_IMPLEMENTATION
#endif // !HASHTABLE_H
