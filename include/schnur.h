/**
 *  schnur.h
 *
 *  'Schnur' is a german translation for the word 'string'.
 *  schnur.h is a simple STB-Style library for dealing with strings in C.
 *
 *  If you want the implementation, this should be defined once:
 *      #define SCHNUR_IMPLEMENTATION
 *      #include <schnur.h>
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

#ifndef SCHNUR_H
#define SCHNUR_H
#include <stddef.h>

#ifndef SCHNUR_MAX_STRLEN
#define SCHNUR_MAX_STRLEN 32768
#endif

#ifndef SCHNUR_MAX_APPEND_LEN
#define SCHNUR_MAX_APPEND_LEN 4096
#endif

/**
 *  string_n_len
 *
 *  Get the length of a null-terminated string.
 *  Return:
 *      - 0 if the string is NULL
 *      - n if the length of the string is >= n
 *      - length otherwise
 */
size_t string_n_len(const char *str, size_t n);

/**
 *  string_len
 *
 *  Get the length of a null-terminated string.
 *  Return:
 *      - 0 if the string is NULL
 *      - SCHNUR_MAX_STRLEN if the length of the string is >= n
 *      - length otherwise
 */
size_t string_len(const char *str);

/**
 *  string_append
 *
 *  Appends 'append' to 'str' without modifying 'str' or 'append'.
 *  Return:
 *      - A heap-allocated string that contains 'str'+'append' on success
 *      - NULL if 'str' or 'append' is NULL
 */
char *string_append(const char *str, const char *append);

#ifdef SCHNUR_IMPLEMENTATION
#include <stdlib.h>

size_t string_n_len(const char *str, size_t n) {
    if (str == NULL) return 0;

    for (size_t i = 0; i < n; ++i) {
        if (str[i] == '\0') return i;
    }
    return n;
}

size_t string_len(const char *str) {
    if (str == NULL) return 0;
    return string_n_len(str, SCHNUR_MAX_STRLEN);
}

char *string_append(const char *str, const char *append) {
    if (str == NULL || append == NULL) return NULL;

    size_t append_len = string_len(append);
    size_t str_len = string_len(str);
    char *new_str = calloc(str_len+append_len+1, sizeof(char));
    if (new_str == NULL) return NULL;
    if (append_len == 0) return new_str;
    
    for (size_t i = 0; i < str_len; ++i) {
        new_str[i] = str[i];
    }
    for (size_t i = str_len; i < str_len+append_len; ++i) {
        new_str[i] = append[i-str_len];
    }
    new_str[str_len+append_len] = '\0';
    return new_str;
}

#endif // SCHNUR_IMPLEMENTATION
#endif // SCHNUR_H
