#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Information about the program. */
void explain_usage() {
    printf(
        "Your input was incorrect. Please use the following format:\n"
        "To encrypt using OTP: ./otp encrypt \"plaintext\"\n"
        "To decrypt using OTP: ./otp decrypt \"ciphertext_hex\" \"key_hex\"\n"
        "To recover the key and the plaintexts from two ciphertexts encrypted with the same key: ./otp recover\n"
    );
}

/* Function for XORing two buffers input1 and input2 and storing the result in output. 
   User should pass an allocated output buffer of size n. */
void xor(const unsigned char *input1, const unsigned char *input2, unsigned char *output, size_t n) {
    size_t i;
    for(i = 0; i<n; i++) {
        output[i] = input1[i] ^ input2[i];
    }
} 

/* Function that prints each byte as two hexadecimal digits */
void print_hex(const unsigned char *buffer, size_t n) {
    size_t i;
    for(i = 0; i<n; i++) {
        printf("%02x", buffer[i]);
    }
    printf("\n");
}

int hex_value(char c) {
    if (c >= '0' && c <= '9') {return c - '0';}
    if (c >= 'a' && c <= 'f') {return c - 'a' + 10;}
    if (c >= 'A' && c <= 'F') {return c - 'A' + 10;}
    return -1;
}

unsigned char *hex_to_bytes(const char* hex_string) {
    size_t n = strlen(hex_string);
    if(n%2 != 0) {
        return NULL;
    }
    size_t hex_n = n/2;
    
    unsigned char *output = malloc(hex_n * sizeof(unsigned char));
    if(output == NULL) {
        return NULL;
    }

    size_t i;
    for(i=0; i<n; i+=2) {
        int first = hex_value(hex_string[i]);
        int second = hex_value(hex_string[i+1]);
        
        if(first < 0 || second < 0) {
            free(output);
            return NULL;
        }
        output[i/2] = (first * 16) + second;  
    }
    return output;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        explain_usage();
        return 1;
    }
    
    if (strcmp(argv[1], "encrypt") == 0 && argc == 3) {
        const unsigned char *plaintext = (const unsigned char *) argv[2];
        size_t n = strlen(argv[2]);

        unsigned char *key = malloc(n * sizeof(unsigned char));
        unsigned char *ciphertext = malloc(n * sizeof(unsigned char));

        if (key == NULL || ciphertext == NULL) {
            free(key);
            free(ciphertext);
            printf("Allocation Error\n");
            return 1;
        }

        FILE *fp = fopen("/dev/urandom", "rb");
        if(fp == NULL) {
            free(ciphertext);
            free(key);
            printf("File reading Error\n");
            return 1;
        }
        size_t bytes_read = fread(key, sizeof(unsigned char), n, fp);
        if(bytes_read != n) {
            fclose(fp);
            free(ciphertext);
            free(key);
            printf("Error reading from file\n");
            return 1;
        }

        fclose(fp);

        xor(plaintext, key, ciphertext, n);

        printf("Plaintext: %s\n", argv[2]);
        printf("KEY (OTP):\n");
        print_hex(key, n);
        printf("Ciphertext:\n");
        print_hex(ciphertext, n);
        
        free(key);
        free(ciphertext);
    } else if (strcmp(argv[1], "decrypt") == 0 && argc == 4) {
        const char *cipherhex = argv[2];
        const char *key = argv[3];
        size_t n_c = strlen(cipherhex);
        size_t n_k = strlen(key);
        
        if (n_c != n_k) {
            printf("Given Ciphertext and Key does not match in length, Error\n");
            return 1;
        }

        unsigned char *cipherhex_bytes = hex_to_bytes(cipherhex);
        unsigned char *key_bytes = hex_to_bytes(key);
        
        if(cipherhex_bytes == NULL || key_bytes == NULL) {
            free(cipherhex_bytes);
            free(key_bytes);
            printf("Invalid hex input error\n");
            return 1;
        }

        size_t decoded_n = n_c / 2;
        unsigned char *plaintext = malloc((decoded_n + 1) * sizeof(unsigned char));
        if(plaintext == NULL) {
            free(cipherhex_bytes);
            free(key_bytes);
            printf("Allocation error\n");
            return 1;
        }
        plaintext[decoded_n] = '\0';
        xor(cipherhex_bytes, key_bytes, plaintext, decoded_n);
        printf("Given Ciphertext: \n%s\n", cipherhex);
        printf("Given key: \n%s\n", key);
        printf("Result after decoding in hexadecimal:\n");
        print_hex(plaintext, decoded_n);
        printf("Result in ASCII format: \n%s\n", plaintext);

        free(cipherhex_bytes);
        free(key_bytes);
        free(plaintext);

    } else if (strcmp(argv[1], "recover") == 0 && argc == 2) {

    } else {
        explain_usage();
        return 1;
    }
    return 0;
}