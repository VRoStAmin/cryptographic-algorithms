#include <stdio.h>
#include <string.h>
#include "otp.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        otp_explain_usage();
        return 1;
    }

    if (strcmp(argv[1], "encrypt") == 0 && argc == 3) {
        otp_encrypt_demo(argv[2]);
    } else if (strcmp(argv[1], "decrypt") == 0 && argc == 4) {
        otp_decrypt_demo(argv[2], argv[3]);
    } else if (strcmp(argv[1], "recover") == 0 && argc == 2) {
        otp_recover_demo();
    } else {
        otp_explain_usage();
        return 1;
    }
    return 0;
}
