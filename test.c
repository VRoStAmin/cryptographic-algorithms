#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "caesar/caesar.h"
#include "otp/otp.h"
#include "passattack/passattack.h"
#include "prngstream/prngstream.h"
#include "secretsharing/secretsharing.h"

int main() {
    printf("ASSIGNMENT 1, CSD5255 MASTORAKIS EMMANOUIL\n");
    printf("TESTING FUNCTIONS OF CRYPTOGRAPHIC ALGORITHMS\n");
    printf("______________________________________________\n");
    printf("\n");
    
    printf("Testing otp\n");
    printf("Encrypting: Hello world!\n");
    otp_encrypt_demo("Hello world!");

    printf("\n");
    printf("Decrypting: \"cbe0e9b2ea57469cd15ac7aa\"\n");
    printf("Using OTP key: \"838585de857731f3a336a38b\"\n");
    otp_decrypt_demo("cbe0e9b2ea57469cd15ac7aa", "838585de857731f3a336a38b");
    printf("\n");

    printf("Testing the recovering of the two ciphertexts encrypted with the same OTP key\n");
    otp_recover_demo();

    printf("\n");
    printf("______________________________________________\n");
    printf("Testing caesar\n");
    printf("Encrypting: \"Caesar is easy to break!\"\n");
    printf("Using key: 3\n");
    caesar_encrypt_demo("Caesar is easy to break!", 3);
    printf("\n");
    
    printf("Decrypting: \"Fdhvdu lv hdvb wr euhdn!\"\n");
    printf("Using key: 3\n");
    caesar_decrypt_demo("Fdhvdu lv hdvb wr euhdn!", 3);
    printf("\n");

    printf("Bruteforcing: \"Fdhvdu lv hdvb wr euhdn!\"\n");
    caesar_bruteforce_demo("Fdhvdu lv hdvb wr euhdn!");
    printf("\n");

    printf("Frequency analysis attack: \"Hyhub hyhqlqj lv wkh hqg ri hyhub gdb\"\n");
    caesar_analysis_demo("Hyhub hyhqlqj lv wkh hqg ri hyhub gdb");
    printf("\n");

    printf("______________________________________________\n");
    printf("Testing PRNG stream cipher\n");
    printf("Encrypting: \"Hello stream cipher!\"\n");
    printf("Using seed: 12345\n");
    prngstream_encrypt_demo("Hello stream cipher!", 12345);
    printf("\n");

    printf("Decrypting: \"0ff081570ef0b2baea9d7a2c3976e7a5baa5fb99\"\n");
    printf("Using seed: 12345\n");
    prngstream_decrypt_demo("0ff081570ef0b2baea9d7a2c3976e7a5baa5fb99", 12345);
    printf("\n");

    printf("Testing reuse of same seed in stream cipher\n");
    prngstream_reuse_demo();
    printf("\n");

    printf("______________________________________________\n");
    printf("Testing secret sharing\n");
    printf("Creating 3 points for secret D = 100\n");
    secretsharing_create_demo(100);
    printf("\n");

    printf("Recovering secret using points from line y = 68x + 10\n");
    printf("Using points (141, 9598) and (391, 26598)\n");
    secretsharing_recover_demo(141, 9598, 391, 26598);
    printf("\n");

    printf("______________________________________________\n");
    printf("Testing password timing attack\n");
    printf("Trying direct guess: \"ABABABAB\"\n");
    passattack_input_demo("ABABABAB");
    printf("\n");

    printf("Running timing attack demo\n");
    passattack_attack_demo();
    printf("\n");

    return 0;
}
