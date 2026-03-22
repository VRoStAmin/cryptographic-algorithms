#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prngstream.h"

void prngstream_explain_usage() {
    printf(
        "Your input was incorrect. Please use the following format:\n"
        "To encrypt using XOR stream cipher: ./prngstream encrypt \"plaintext\" seed\n"
        "To decrypt using XOR stream cipher: ./prngstream decrypt \"ciphertext_hex\" seed\n"
        "To demonstrate seed reuse danger: ./prngstream demo\n"
    );
}

/* Function that converts a seed from the command line into an int. */
int convert_to_seed_integer(char *seed_string) {
    char *endptr;
    long seed_val = strtol(seed_string, &endptr, 10);

    if (endptr == seed_string || *endptr != '\0' || seed_val < 0) {
        printf("Invalid seed...\n\n");
        prngstream_explain_usage();
        return -1;
    }

    return (int)seed_val;
}

/* Function that does encryption or decryption using the xor stream cipher and a seed. */
void xor_stream_cipher(const unsigned char *input, size_t n, unsigned char *output, unsigned int seed) {
    size_t i;
    srand(seed);

    for (i = 0; i < n; i++) {
        unsigned char keystream_byte = rand() % 256;
        output[i] = input[i] ^ keystream_byte;
    }
}
/* Prints as hex */
void prngstream_print_hex(const unsigned char *buffer, size_t n) {
    size_t i;
    for (i = 0; i < n; i++) {
        printf("%02x", buffer[i]);
    }
    printf("\n");
}

/* Function for converting a hexadecimal character into its integer value. */
int prngstream_hex_value(char c) {
    if (c >= '0' && c <= '9') { return c - '0'; }
    if (c >= 'a' && c <= 'f') { return c - 'a' + 10; }
    if (c >= 'A' && c <= 'F') { return c - 'A' + 10; }
    return -1;
}

/* Function that converts a hex string into a hex stream.*/
unsigned char *prngstream_hex_to_bytes(const char *hex_string) {
    size_t n = strlen(hex_string);
    if (n % 2 != 0) {
        return NULL;
    }

    size_t bytes_n = n / 2;
    unsigned char *output = malloc(bytes_n);
    if (output == NULL) {
        return NULL;
    }

    size_t i;
    for (i = 0; i < n; i += 2) {
        int first = prngstream_hex_value(hex_string[i]);
        int second = prngstream_hex_value(hex_string[i + 1]);

        if (first < 0 || second < 0) {
            free(output);
            return NULL;
        }

        output[i / 2] = (first * 16) + second;
    }

    return output;
}

/* Demo that shows why reusing the same key is dangerous. */
void reuse_seed_demo() {
    const unsigned char *plaintext1 = (const unsigned char *)"attack at dawn";
    const unsigned char *plaintext2 = (const unsigned char *)"attack at dusk";
    unsigned int seed = 12345;
    size_t n = strlen((const char *)plaintext1);

    unsigned char *ciphertext1 = malloc(n);
    unsigned char *ciphertext2 = malloc(n);
    unsigned char *cipher_xor = malloc(n);
    unsigned char *plain_xor = malloc(n);

    if (ciphertext1 == NULL || ciphertext2 == NULL || cipher_xor == NULL || plain_xor == NULL) {
        printf("Allocation error\n");
        free(ciphertext1);
        free(ciphertext2);
        free(cipher_xor);
        free(plain_xor);
        return;
    }

    xor_stream_cipher(plaintext1, n, ciphertext1, seed);
    xor_stream_cipher(plaintext2, n, ciphertext2, seed);

    size_t i;
    for (i = 0; i < n; i++) {
        cipher_xor[i] = ciphertext1[i] ^ ciphertext2[i];
        plain_xor[i] = plaintext1[i] ^ plaintext2[i];
    }

    printf("Why reusing the same seed is dangerous... \n");
    printf("Seed used: %u\n\n", seed);

    printf("Plaintext 1:\n%s\n", plaintext1);
    printf("Plaintext 2:\n%s\n\n", plaintext2);

    printf("Ciphertext 1 (hex):\n");
    prngstream_print_hex(ciphertext1, n);

    printf("Ciphertext 2 (hex):\n");
    prngstream_print_hex(ciphertext2, n);

    printf("Ciphertext1 XOR Ciphertext2 (hex):\n");
    prngstream_print_hex(cipher_xor, n);

    free(ciphertext1);
    free(ciphertext2);
    free(cipher_xor);
    free(plain_xor);
}

void prngstream_encrypt_demo(const char *plaintext_str, int seed) {
    const unsigned char *plaintext = (const unsigned char *)plaintext_str;
    size_t n = strlen(plaintext_str);
    unsigned char *ciphertext = malloc(n);
    if (ciphertext == NULL) {
        printf("Allocation error\n");
        return;
    }

    xor_stream_cipher(plaintext, n, ciphertext, (unsigned int)seed);

    printf("Plaintext Input:\n");
    printf("%s\n", plaintext);
    printf("Ciphertext Output (hex) with seed [%d]:\n", seed);
    prngstream_print_hex(ciphertext, n);

    free(ciphertext);
}

void prngstream_decrypt_demo(const char *ciphertext_hex, int seed) {
    unsigned char *ciphertext = prngstream_hex_to_bytes(ciphertext_hex);
    if (ciphertext == NULL) {
        printf("Invalid hex ciphertext...\n\n");
        prngstream_explain_usage();
        return;
    }

    size_t n = strlen(ciphertext_hex) / 2;
    unsigned char *plaintext = malloc(n + 1);
    if (plaintext == NULL) {
        printf("Allocation error\n");
        free(ciphertext);
        return;
    }

    xor_stream_cipher(ciphertext, n, plaintext, (unsigned int)seed);
    plaintext[n] = '\0';

    printf("Ciphertext Input (hex):\n");
    printf("%s\n", ciphertext_hex);
    printf("Plaintext Output with seed [%d]:\n", seed);
    printf("%s\n", plaintext);

    free(ciphertext);
    free(plaintext);
}

void prngstream_reuse_demo() {
    reuse_seed_demo();
}
