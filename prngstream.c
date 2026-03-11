#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void explain_usage() {
    printf(
        "Your input was incorrect. Please use the following format:\n"
        "To encrypt using XOR stream cipher: ./prngstream encrypt \"plaintext\" seed\n"
        "To decrypt using XOR stream cipher: ./prngstream decrypt \"ciphertext_hex\" seed\n"
        "To demonstrate seed reuse danger: ./prngstream demo\n"
    );
}

int convert_to_seed_integer(char *seed_string) {
    char *endptr;
    long seed_val = strtol(seed_string, &endptr, 10);

    if (endptr == seed_string || *endptr != '\0' || seed_val < 0) {
        printf("Invalid seed...\n\n");
        explain_usage();
        return -1;
    }

    return (int)seed_val;
}

void xor_stream_cipher(const unsigned char *input, size_t n, unsigned char *output, unsigned int seed) {
    size_t i;
    srand(seed);

    for (i = 0; i < n; i++) {
        unsigned char keystream_byte = rand() % 256;
        output[i] = input[i] ^ keystream_byte;
    }
}

void print_hex(const unsigned char *buffer, size_t n) {
    size_t i;
    for (i = 0; i < n; i++) {
        printf("%02x", buffer[i]);
    }
    printf("\n");
}

int hex_value(char c) {
    if (c >= '0' && c <= '9') { return c - '0'; }
    if (c >= 'a' && c <= 'f') { return c - 'a' + 10; }
    if (c >= 'A' && c <= 'F') { return c - 'A' + 10; }
    return -1;
}

unsigned char *hex_to_bytes(const char *hex_string) {
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
        int first = hex_value(hex_string[i]);
        int second = hex_value(hex_string[i + 1]);

        if (first < 0 || second < 0) {
            free(output);
            return NULL;
        }

        output[i / 2] = (first * 16) + second;
    }

    return output;
}

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

    printf("Demonstration of seed reuse danger\n");
    printf("Seed used: %u\n\n", seed);

    printf("Plaintext 1:\n%s\n", plaintext1);
    printf("Plaintext 2:\n%s\n\n", plaintext2);

    printf("Ciphertext 1 (hex):\n");
    print_hex(ciphertext1, n);

    printf("Ciphertext 2 (hex):\n");
    print_hex(ciphertext2, n);

    printf("Ciphertext1 XOR Ciphertext2 (hex):\n");
    print_hex(cipher_xor, n);

    printf("Plaintext1 XOR Plaintext2 (hex):\n");
    print_hex(plain_xor, n);

    free(ciphertext1);
    free(ciphertext2);
    free(cipher_xor);
    free(plain_xor);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        explain_usage();
        return 1;
    }

    if (strcmp(argv[1], "encrypt") == 0 && argc == 4) {
        const unsigned char *plaintext = (const unsigned char *)argv[2];
        int seed = convert_to_seed_integer(argv[3]) ;
        if (seed == -1) {
            return 1;
        }

        size_t n = strlen((const char *)plaintext);
        unsigned char *ciphertext = malloc(n);
        if (ciphertext == NULL) {
            printf("Allocation error\n");
            return 1;
        }

        xor_stream_cipher(plaintext, n, ciphertext, (unsigned int)seed);

        printf("Plaintext Input:\n");
        printf("%s\n", plaintext);
        printf("Ciphertext Output (hex) with seed [%d]:\n", seed);
        print_hex(ciphertext, n);

        free(ciphertext);

    } else if (strcmp(argv[1], "decrypt") == 0 && argc == 4) {
        const char *ciphertext_hex = argv[2];
        int seed = convert_to_seed_integer(argv[3]);
        if (seed == -1) {
            return 1;
        }

        unsigned char *ciphertext = hex_to_bytes(ciphertext_hex);
        if (ciphertext == NULL) {
            printf("Invalid hex ciphertext...\n\n");
            explain_usage();
            return 1;
        }

        size_t n = strlen(ciphertext_hex) / 2;
        unsigned char *plaintext = malloc(n + 1);
        if (plaintext == NULL) {
            printf("Allocation error\n");
            free(ciphertext);
            return 1;
        }

        xor_stream_cipher(ciphertext, n, plaintext, (unsigned int)seed);
        plaintext[n] = '\0';

        printf("Ciphertext Input (hex):\n");
        printf("%s\n", ciphertext_hex);
        printf("Plaintext Output with seed [%d]:\n", seed);
        printf("%s\n", plaintext);

        free(ciphertext);
        free(plaintext);

    } else if (strcmp(argv[1], "demo") == 0 && argc == 2) {
        reuse_seed_demo();

    } else {
        explain_usage();
        return 1;
    }

    return 0;
}