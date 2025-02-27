# sha256.h: A SHA-256 stb-style header only library

[![patreon](https://img.shields.io/badge/patreon-FF5441?style=for-the-badge&logo=Patreon)](https://www.patreon.com/hughdavenport)
[![youtube](https://img.shields.io/badge/youtube-FF0000?style=for-the-badge&logo=youtube)](https://www.youtube.com/watch?v=TODO&list=TODO)

This repo contains a [stb-style](https://github.com/nothings/stb/blob/master/docs/stb_howto.txt) header only library. You only need the [sha256.h](https://github.com/hughdavenport/sha256.h/raw/refs/heads/main/sha256.h) file.

SHA256 is defined in [RFC 6234](https://datatracker.ietf.org/doc/html/rfc6234).

To use the library, `#define SHA256_IMPLEMENTATION` exactly once (in your main.c may be a good place). You can `#include` the file as many times as you like.

An example file is shown below.
```c
#define SHA256_IMPLEMENTATION
#include "sha256.h"

#include <stdio.h>
#include <string.h>

int main() {
    const char *test = "Hello, SHA256\n";
    uint8_t result[SHA256_DIGEST_BYTE_LENGTH];

    if (sha256_digest(test, strlen(test), result)) {
        SHA256_PRINTF_HEX(result);
        printf("\n");
        return 0;
    }
    fprintf(stderr, "Unexpected error\n");
    return 1;
}
```

Test by saving the above to a file `example.c` and then running this
```sh
cc example.c
./a.out
```

Expected output: `3b3fdf6b05c42c6335d8009c474cf91211d9d31293f7d0201fa855199e0d63f1`

Please leave any comments about what you liked. Feel free to suggest any features or improvements.

You are welcome to support me financially if you would like on my [patreon](https://www.patreon.com/hughdavenport).
