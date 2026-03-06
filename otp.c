#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void explain_usage() {
    printf(
        "Your input was incorrect. Please use the following format:\n"
        "To encrypt using OTP: ./otp encrypt \"plaintext\"\n"
        "To decrypt using OTP: ./otp decrypt \"ciphertext_hex\" \"key_hex\"\n"
        "To recover the key and the plaintexts from two ciphertexts encrypted with the same key: ./otp recover\n"
    );
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        explain_usage();
        return 1;
    }
    
    if (strcmp(argv[1], "encrypt") == 0 && argc == 3) {
        
    } else if (strcmp(argv[1], "decrypt") == 0 && argc == 4) {
        
    } else if (strcmp(argv[1], "recover") == 0 && argc == 2) {

    } else {
        explain_usage();
        return 1;
    }
}