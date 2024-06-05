#define SCHNUR_IMPLEMENTATION
#include "schnur.h"

#define LOGGER_IMPLEMENTATION
#include "logger.h"

#include <string.h>

int main(void) {
    LOGGER_ASSERT(string_len("abc") == 3);
    LOGGER_ASSERT(string_len(NULL) == 0);
    LOGGER_ASSERT(string_n_len("abc", 15) == 3);
    LOGGER_ASSERT(string_n_len("abc", 2) == 2);
    LOGGER_ASSERT(string_n_len(NULL, 2) == 0);

    char *s = "abc";
    char *s1 = string_append(s, "def");
    LOGGER_ASSERT(s1 != NULL);
    LOGGER_ASSERT(strcmp(s1, "abcdef") == 0);
    free(s1);

    char *str = calloc(5, sizeof(char));
    LOGGER_ASSERT(str != NULL);
    LOGGER_ASSERT(string_n_copy(str, "abcd", 32) != NULL);
    LOGGER_ASSERT(strcmp(str, "abcd") == 0);
    free(str);
}
