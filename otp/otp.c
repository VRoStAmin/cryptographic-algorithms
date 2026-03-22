#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "otp.h"

/* Information about the program. */
void otp_explain_usage() {
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
void otp_print_hex(const unsigned char *buffer, size_t n) {
    size_t i;
    for(i = 0; i<n; i++) {
        printf("%02x", buffer[i]);
    }
    printf("\n");
}

/* Function for converting a hexadecimal character into its integer value.*/
int otp_hex_value(char c) {
    if (c >= '0' && c <= '9') {return c - '0';}
    if (c >= 'a' && c <= 'f') {return c - 'a' + 10;}
    if (c >= 'A' && c <= 'F') {return c - 'A' + 10;}
    return -1;
}

/* Function that takes a character and returns if the character is considered readable or not.*/
int not_garbage(unsigned char c) {
    if (c >= 'A' && c <= 'Z') {return 1;}
    if (c >= 'a' && c <= 'z') {return 1;}
    if (c == ' ' || c == '.' || c == ',' || c == '!' || c == '?'|| c == '-') {return 1;}
    return 0; 
}

/* Function that tries to XOR a given word to the given buffer at every position inside the buffer.
   If the word is readable, it prints it. Otherwise it skips. */
void try_given_word(const unsigned char *buffer, size_t l, const char *word) {
    size_t n = strlen(word);
    size_t i;
    size_t j;

    for (i = 0; i + n <= l; i++) {
        int valid = 1;
        for (j = 0; j < n; j++) {
            unsigned char c = buffer[i + j] ^ (unsigned char)word[j];
            if (!not_garbage(c)) {
                valid = 0;
                break;
            }
        }
        if (valid) {
            printf("Trying word at position: %zu [", i);
            for (j = 0; j < n; j++) {
                unsigned char c = buffer[i + j] ^ (unsigned char)word[j];
                printf("%c", c);
            }
            printf("]\n");
        }
    }
}

/* Checks if a word is in the file given. */
int is_in_dictionary(const unsigned char *word, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if(fp == NULL) {
        printf("Could not open given filename: %s\n", filename);
        return 0;
    }

    char dict_word[256];
    while(fgets(dict_word, sizeof(dict_word), fp) != NULL) {
        size_t size = strlen(dict_word);
        if(size > 0 && dict_word[size - 1] == '\n') {
            dict_word[size - 1] = '\0';
        }
        
        if(strcmp(dict_word, (const char *)word) == 0) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

/* Scans all words in a dictionary file and tries them against every position 
   in the XORed ciphertext buffer. 
   If both the guessed word and the resulting opposite word look readable, the pair is printed.*/
void words_scanner(const unsigned char *buffer, size_t l, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if(fp == NULL) {
        printf("Could not open given filename: %s\n", filename);
        return;
    }

    char word[256];
    while(fgets(word, sizeof(word), fp) != NULL) {
        size_t n = strlen(word);
        if(n > 0 && word[n-1] == '\n') {
            word[n-1] = '\0';
            n--;
        }

        if(n < 2 || n > 9 || n > l) {
            continue;
        }

        size_t i;
        for (i = 0; i+n <= l; i++) {
            int correct_word = 1;
            char other_word[256];
            size_t j;
            for(j = 0; j < n; j++) {
                unsigned char c = buffer[i + j] ^(unsigned char)word[j];
                if(!not_garbage(c)) {
                    correct_word = 0;
                    break;
                }
                other_word[j] = (char)c;
            }
            if(correct_word) {
                other_word[n] = '\0';
                printf("Position %zu: \"%s\" <=> \"%s\"\n", i, word, other_word);
            }

        }
    }
    fclose(fp);
}

/* Function that converts a hex string into a hex stream.*/
unsigned char *otp_hex_to_bytes(const char* hex_string) {
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
        int first = otp_hex_value(hex_string[i]);
        int second = otp_hex_value(hex_string[i+1]);
        
        if(first < 0 || second < 0) {
            free(output);
            return NULL;
        }
        output[i/2] = (first * 16) + second;  
    }
    return output;
}

void otp_encrypt_demo(const char *plaintext_str) {
    const unsigned char *plaintext = (const unsigned char *) plaintext_str;
    size_t n = strlen(plaintext_str);

    unsigned char *key = malloc(n * sizeof(unsigned char));
    unsigned char *ciphertext = malloc(n * sizeof(unsigned char));

    if (key == NULL || ciphertext == NULL) {
        free(key);
        free(ciphertext);
        printf("Allocation Error\n");
        return;
    }

    FILE *fp = fopen("/dev/urandom", "rb");
    if(fp == NULL) {
        free(ciphertext);
        free(key);
        printf("File reading Error\n");
        return;
    }
    size_t bytes_read = fread(key, sizeof(unsigned char), n, fp);
    if(bytes_read != n) {
        fclose(fp);
        free(ciphertext);
        free(key);
        printf("Error reading from file\n");
        return;
    }

    fclose(fp);

    xor(plaintext, key, ciphertext, n);

    printf("Plaintext: %s\n", plaintext_str);
    printf("KEY (OTP):\n");
    otp_print_hex(key, n);
    printf("Ciphertext:\n");
    otp_print_hex(ciphertext, n);
    
    free(key);
    free(ciphertext);
}

void otp_decrypt_demo(const char *cipherhex, const char *key) {
    size_t n_c = strlen(cipherhex);
    size_t n_k = strlen(key);
    
    if (n_c != n_k) {
        printf("Given Ciphertext and Key does not match in length, Error\n");
        return;
    }

    unsigned char *cipherhex_bytes = otp_hex_to_bytes(cipherhex);
    unsigned char *key_bytes = otp_hex_to_bytes(key);
    
    if(cipherhex_bytes == NULL || key_bytes == NULL) {
        free(cipherhex_bytes);
        free(key_bytes);
        printf("Invalid hex input error\n");
        return;
    }

    size_t decoded_n = n_c / 2;
    unsigned char *plaintext = malloc((decoded_n + 1) * sizeof(unsigned char));
    if(plaintext == NULL) {
        free(cipherhex_bytes);
        free(key_bytes);
        printf("Allocation error\n");
        return;
    }
    plaintext[decoded_n] = '\0';
    xor(cipherhex_bytes, key_bytes, plaintext, decoded_n);
    printf("Given Ciphertext: \n%s\n", cipherhex);
    printf("Given key: \n%s\n", key);
    printf("Result after decoding in hexadecimal:\n");
    otp_print_hex(plaintext, decoded_n);
    printf("Result in ASCII format: \n%s\n", plaintext);

    free(cipherhex_bytes);
    free(key_bytes);
    free(plaintext);
}

void otp_recover_demo() {
    const char *cipher1_string = "72814c04ba04a4f8a05f458d2e26bcef7e433c4ee43d5d7f1c9c0bc8acf8d714c6c9324c30da3dc13f3411e08769b2949535dfd386";
    const char *cipher2_string = "6f904804a949f6febc5c48c83d75f3e3690b294ca9311b2f04975e83acf79e4689c9774a26dc68c73e2548ab8b63e187883edb9087";

    unsigned char *cipher1_hex = otp_hex_to_bytes(cipher1_string);
    unsigned char *cipher2_hex = otp_hex_to_bytes(cipher2_string);
    if(cipher1_hex == NULL || cipher2_hex == NULL) {
        free(cipher1_hex);
        free(cipher2_hex);
        printf("Error in translating cipher to hexadecimal\n");
        return;
    }
    size_t n = strlen(cipher1_string) / 2;
    unsigned char *no_key_ciphers = malloc(n * sizeof(unsigned char));
    unsigned char *key = malloc(n * sizeof(unsigned char));

    if(no_key_ciphers == NULL || key == NULL) {
        printf("Allocation error\n");
        free(cipher1_hex);
        free(cipher2_hex);
        free(key);
        free(no_key_ciphers);
        return;
    }
    
    xor(cipher1_hex, cipher2_hex, no_key_ciphers, n);

    // printf("Scanning dictionary: \n");
    // words_scanner(no_key_ciphers, n, "mywords.txt");
    // size_t i;
    // for(i = 0; i < n; i++) {
    //     unsigned char result = no_key_ciphers[i] ^ ' ';
    //     if(not_garbage(result)) {
    //         printf("Position %zu: one plaintext may have space, the other may have '%c'\n", i, result);
    //     }
    // }

    printf("Trying possible words: \n");

    printf("Trying the phrase:\" the \"\n");
    try_given_word(no_key_ciphers, n, " the ");
    
    printf("Trying the phrase:\" and \"\n");
    try_given_word(no_key_ciphers, n, " and ");
    
    printf("Trying the phrase:\" security \"\n");
    try_given_word(no_key_ciphers, n, " security ");
    
    printf("Trying the phrase:\" key \"\n");
    try_given_word(no_key_ciphers, n, " key ");

    printf("Trying the phrase:\" otp \"\n");
    try_given_word(no_key_ciphers, n, " otp ");

    printf("Trying the phrase:\" one time pad \"\n");
    try_given_word(no_key_ciphers, n, " one time pad ");

    printf("Trying the phrase:\" class \"\n");
    try_given_word(no_key_ciphers, n, " class ");

    printf("Trying the phrase:\" breaks \"\n");
    try_given_word(no_key_ciphers, n, " breaks ");

    /* From these words we can see at position 34 the phrase [esue the k] */
    /* From that we can use reuse the key as a phrase. */
    printf("Trying the phrase:\" reuse the key \"\n");
    try_given_word(no_key_ciphers, n, " reuse the key ");
    /* In position 32 we found [or security iss], since we have an extra s most likely the space after key is wrong. */
    /* Most likely there is a special character, or the word continues, or it is plural (keys) so we can try these next. */
    /* Lets try [or security is ] instead of [or security iss] */
    printf("Trying the phrase:\" or security is \"\n");
    try_given_word(no_key_ciphers, n, " or security is ");
    /* We can now continue with different words that could be in either the first phrase we know so far: */
    /* [ or security is ] or we can guess different words that could be in the second phrase we know so far [r reuse the keys] */
    printf("Trying the phrase: \" never \"\n");
    try_given_word(no_key_ciphers, n, " never ");
    printf("Trying the phrase: \"never\"\n");
    try_given_word(no_key_ciphers, n, "never");
    printf("Trying the phrase: \"ever\"\n");
    try_given_word(no_key_ciphers, n, "ever");
    /* On never we see that at position 0 we have strea, so likely one plaintext starts with stream */
    /* On ever we see that at position 28 we have ey, so likely this is something like key, */
    printf("Trying some next phrases like: \"stream \"\n");
    try_given_word(no_key_ciphers, n, "stream ");
    printf("Trying some next phrases like: \"key, \"\n");
    try_given_word(no_key_ciphers, n, "key, ");
    /* On stream we get never r at position 0, so we have the two phrases [never r], [stream ]*/
    /* Also [r reuse the keys] we saw earlier starts at position 31, it ends at position 46. At position 46 we get a [ gone.!] from stream. 
    So we can add stream to this known phrase as [r reuse the keystream]*/
    /* Trying this phrase we get: */
    printf("Tring phrase: \"ever reuse the keystream\"\n");
    try_given_word(no_key_ciphers, n, "ever reuse the keystream");
    /* Now we get phrase [key, or security is gone.] trying that next we get. */
    printf("Trying phrase: \"key, or security is gone.\"\n");
    try_given_word(no_key_ciphers, n, "key, or security is gone.");
    /* In one of the plaintexts at the start we have a known [never r] so we can guess that as follows. */
    printf("Trying phrase \"never reuse \"\n");
    try_given_word(no_key_ciphers, n, "never reuse ");
    /* We get [stream ciphers], so trying this now we get. */
    printf("Trying phrase: \"stream cipher\"\n");
    try_given_word(no_key_ciphers, n, "stream cipher");
    /* By trying more words we can see that we have [s break if you] at position 13 */
    printf("Trying phrase: \"s break if you \"\n");
    try_given_word(no_key_ciphers, n, "s break if you ");

    /* So now we can start guessing bigger phrases */
    printf("Trying phrase: \"never reuse a one-time pad key, or security is gone.\" \n");
    try_given_word(no_key_ciphers, n, "never reuse a one-time pad key, or security is gone.");
    /* We have two phrases: */
    /* stream ciphers break if you ever reuse the keystream*/
    /* never reuse a one-time pad key, or security is gone.*/
    /* Both phrases are 52 characters, but they should be 53 because dividing the lenght of the given ciphertexts by 2, we get 53.*/
    /* One plaintexts ends with a . while the other one has not ended yet so likely there is something missing from that one. */
    printf("Trying phrase: \"never reuse a one-time pad key, or security is gone..\" \n");
    try_given_word(no_key_ciphers, n, "never reuse a one-time pad key, or security is gone..");
    printf("Trying phrase: \"never reuse a one-time pad key, or security is gone. \" \n");
    try_given_word(no_key_ciphers, n, "never reuse a one-time pad key, or security is gone. ");
    
    /* We have now found the plaintexts. */
    printf("\nPlaintext 1 is: \n");
    try_given_word(no_key_ciphers, n, "never reuse a one-time pad key, or security is gone. ");
    printf("\nPlaintext 2 is: \n");
    try_given_word(no_key_ciphers, n, "stream ciphers break if you ever reuse the keystream!");
    printf("\n");
    
    const char *plaintext1 = "never reuse a one-time pad key, or security is gone. ";
    unsigned char *plain1_bytes = (unsigned char *)plaintext1;

    xor(cipher1_hex, plain1_bytes, key, n);

    printf("Recovered key in hex from plaintext1: never...is gone:\n");
    otp_print_hex(key, n);

    unsigned char *recovered2 = malloc((n + 1) * sizeof(unsigned char));
    if (recovered2 == NULL) {
        free(cipher1_hex);
        free(cipher2_hex);
        free(no_key_ciphers);
        free(key);
        printf("Allocation error\n");
        return;
    }

    xor(cipher2_hex, key, recovered2, n);
    recovered2[n] = '\0';

    printf("Recovered plaintext 2 using the key:\n%s\n", recovered2);

    free(recovered2);

    free(cipher1_hex);
    free(cipher2_hex);
    free(no_key_ciphers);
    free(key);
}
