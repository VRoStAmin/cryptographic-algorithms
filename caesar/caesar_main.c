#include <stdio.h>
#include <string.h>
#include "caesar.h"

int main(int argc, char *argv[]) {
    if(argc < 2) {
        caesar_explain_usage();
        return 1;
    }

    if(strcmp(argv[1], "encrypt") == 0 && (argc == 3 || argc == 4)) {
        int key = 1;

        if(argc == 4) {
            key = convert_to_key_integer(argv[3]);
            if(key == -1) {
                return 1;
            }
        }

        caesar_encrypt_demo(argv[2], key);
    } else if (strcmp(argv[1], "decrypt") == 0 && (argc == 3 || argc == 4)) {
        int key = 1;

        if(argc == 4) {
            key = convert_to_key_integer(argv[3]);
            if(key == -1) {
                return 1;
            }
        }

        caesar_decrypt_demo(argv[2], key);
    } else if(strcmp(argv[1], "bruteforce") == 0 && argc == 3) {
        caesar_bruteforce_demo(argv[2]);
    } else if (strcmp(argv[1], "analysis") == 0 && argc == 3) {
        caesar_analysis_demo(argv[2]);
    } else {
        caesar_explain_usage();
        return 1;
    }

    return 0;
}
