#include <stdio.h>
#include <string.h>
#include "prngstream.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        prngstream_explain_usage();
        return 1;
    }

    if (strcmp(argv[1], "encrypt") == 0 && argc == 4) {
        int seed = convert_to_seed_integer(argv[3]);
        if (seed == -1) {
            return 1;
        }

        prngstream_encrypt_demo(argv[2], seed);
    } else if (strcmp(argv[1], "decrypt") == 0 && argc == 4) {
        int seed = convert_to_seed_integer(argv[3]);
        if (seed == -1) {
            return 1;
        }

        prngstream_decrypt_demo(argv[2], seed);
    } else if (strcmp(argv[1], "demo") == 0 && argc == 2) {
        prngstream_reuse_demo();
    } else {
        prngstream_explain_usage();
        return 1;
    }

    return 0;
}
