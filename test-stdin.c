#define SHA256_IMPLEMENTATION
#include "sha256.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define array_t(type) struct { \
    type *data; \
    size_t size; \
    size_t capacity; \
}
#define _array_ensure(arr, n) do { \
    if ((arr).size + (n) > (arr).capacity) { \
        size_t new_cap = ((arr).capacity == 0 ? 16 : 2 * (arr).capacity); \
        while ((arr).size + (n) > new_cap) { \
            new_cap *= 2; \
        } \
        (arr).data = realloc((arr).data, sizeof((arr).data[0]) * new_cap); \
        assert((arr).data != NULL); \
        (arr).capacity = new_cap; \
    } \
} while (false)
#define array_add_many(arr, b, n) do { \
    _array_ensure((arr), (n)); \
    memcpy(&(arr).data[(arr).size], (b), sizeof((arr).data[0]) * (n)); \
    (arr).size += (n); \
} while (false)
#define array_add(arr, b) do { \
    _array_ensure((arr), 1); \
    (arr).data[(arr).size++] = (b); \
} while (false)

int main() {
    array_t(uint8_t) input = {0};
    uint8_t buf[4096];
    uint8_t hash[SHA256_DIGEST_BYTE_LENGTH];

    while (true) {
        ssize_t n = read(STDIN_FILENO, buf, sizeof(buf)/sizeof(buf[0]));
        if (n == 0) break;
        if (n == -1) {
            free(input.data);
            return errno;
        }
        array_add_many(input, buf, n);
    }
    if (sha256_digest(input.data, input.size, hash)) {
        SHA256_PRINTF_HEX(hash);
        free(input.data);
        return 0;
    }
    fprintf(stderr, "Unexpected error\n");
    free(input.data);
    return 1;
}
