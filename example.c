#define SHA256_IMPLEMENTATION
#include "sha256.h"

#include <stdio.h>
#include <string.h>

int main(void) {
    const unsigned char test[] = "Hello, SHA256\n";
    uint8_t result[SHA256_DIGEST_BYTE_LENGTH];

    if (sha256_digest(test, sizeof(test) - 1, result)) {
        SHA256_PRINTF_HEX(result);
        printf("\n");
        return 0;
    }
    fprintf(stderr, "Unexpected error\n");
    return 1;
}
