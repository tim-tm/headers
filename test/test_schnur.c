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
    LOGGER_ASSERT(strcmp(string_append(s, "def"), "abcdef") == 0);
}
