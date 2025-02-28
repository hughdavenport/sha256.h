#define SHA256_IMPLEMENTATION
#include "sha256.h"

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <seed>\n", argv[0]);
        return 1;
    }
    char *seed_s = argv[1];
    if (strlen(seed_s) != SHA256_DIGEST_HEX_LENGTH) {
        fprintf(stderr, "Invalid seed length %ld. Expect %d hex digits\n", strlen(seed_s), SHA256_DIGEST_HEX_LENGTH);
        fprintf(stderr, "Usage: %s <seed>\n", argv[0]);
        return 1;
    }
    uint8_t seed[SHA256_DIGEST_BYTE_LENGTH];
    for (size_t c = 0; c < SHA256_DIGEST_BYTE_LENGTH; c ++) {
        uint8_t b = 0;
        switch (seed_s[2 * c]) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                b = seed_s[2 * c] - '0';
                break;

            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
                b = 10 + seed_s[2 * c] - 'a';
                break;

            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
                b = 10 + seed_s[2 * c] - 'A';
                break;

            default:
                fprintf(stderr, "Invalid seed digit %c\n", seed_s[2 * c]);
                fprintf(stderr, "Usage: %s <seed>\n", argv[0]);
                return 1;
        }
        switch (seed_s[2 * c + 1]) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                b = b << 4 | (seed_s[2 * c + 1] - '0');
                break;

            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
                b = b << 4 | (10 + seed_s[2 * c + 1] - 'a');
                break;

            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
                b = b << 4 | (10 + seed_s[2 * c + 1] - 'A');
                break;

            default:
                fprintf(stderr, "Invalid seed digit %c\n", seed_s[2 * c + 1]);
                fprintf(stderr, "Usage: %s <seed>\n", argv[0]);
                return 1;
        }
        seed[c] = b;
    }

#define MD(idx) md[(idx) * SHA256_DIGEST_BYTE_LENGTH]
    uint8_t md[1003 * SHA256_DIGEST_BYTE_LENGTH];
    uint8_t m[3 * SHA256_DIGEST_BYTE_LENGTH + 1];

    for (int j = 0; j < 100; j ++) {
        memcpy(&MD(0), seed, SHA256_DIGEST_BYTE_LENGTH);
        memcpy(&MD(1), seed, SHA256_DIGEST_BYTE_LENGTH);
        memcpy(&MD(2), seed, SHA256_DIGEST_BYTE_LENGTH);
        for (int i = 3; i < 1003; i ++) {
            memcpy(m, &MD(i-3), SHA256_DIGEST_BYTE_LENGTH);
            memcpy(m + SHA256_DIGEST_BYTE_LENGTH, &MD(i-2), SHA256_DIGEST_BYTE_LENGTH);
            memcpy(m + 2 * SHA256_DIGEST_BYTE_LENGTH, &MD(i-1), SHA256_DIGEST_BYTE_LENGTH);
            if (!sha256_digest(m, 3 * SHA256_DIGEST_BYTE_LENGTH, &MD(i))) {
                fprintf(stderr, "Unexpected error\n");
                return 1;
            }
        }
        memcpy(seed, &MD(1002), SHA256_DIGEST_BYTE_LENGTH);
        SHA256_PRINTF_HEX(seed);
        printf("\n");
    }
    return 0;
}
