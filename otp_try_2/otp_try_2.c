#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "otp_try_2.h"

/* Information about the program. */
void otp_try_2_explain_usage() {
    printf(
        "Your input was incorrect. Please use the following format:\n"
        "To encrypt using OTP: ./otp_try_2 encrypt \"plaintext\"\n"
        "To decrypt using OTP: ./otp_try_2 decrypt \"ciphertext_hex\" \"key_hex\"\n"
        "To recover the key and the plaintexts from two ciphertexts encrypted with the same key: ./otp_try_2 recover\n"
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
void otp_try_2_print_hex(const unsigned char *buffer, size_t n) {
    size_t i;
    for(i = 0; i<n; i++) {
        printf("%02x", buffer[i]);
    }
    printf("\n");
}

/* Function for converting a hexadecimal character into its integer value.*/
int otp_try_2_hex_value(char c) {
    if (c >= '0' && c <= '9') {return c - '0';}
    if (c >= 'a' && c <= 'f') {return c - 'a' + 10;}
    if (c >= 'A' && c <= 'F') {return c - 'A' + 10;}
    return -1;
}

/* Function that takes a character and returns if the character is considered readable or not.*/
int not_garbage(unsigned char c) {
    if (c >= 'A' && c <= 'Z') {return 1;}
    if (c >= 'a' && c <= 'z') {return 1;}
    if (c == ' ' || c == '.' || c == ',' || c == '!' || c == '?'|| c == '-' || c == '\'' || c == ':' || c == ';') {return 1;}
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
int is_in_dictionary(const char *word, const char *filename) {
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
        
        if(strcmp(dict_word, word) == 0) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

/* Function that takes a buffer and tries to xor every position with the char ' ' 
   if it returns something that is not garbage and not the same char ' ' then
   there is a big chance there is a space there. */
size_t collect_space_positions(const unsigned char *buffer, size_t n, size_t *positions) {
    size_t count = 0;
        size_t i;

        for(i = 0; i < n; i++) {
            unsigned char result = buffer[i] ^ ' ';
            if(not_garbage(result) && result != ' ') {
                positions[count++] = i;
            }
        }

        return count;
}

/* Function that takes a prefix, a word that is used to search in filename 
and two word sizes, then searches words that has the prefix given, and is the same size as the two word sizes given and then it 
prints them if found. */
void search_dict_prefix(char *prefix, char *word, size_t word_1, size_t word_2, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Could not open given filename: %s\n", filename);
        return;
    }

    char dict_word[256];
    size_t prefix_len = strlen(prefix);
    int found = 0;

    while (fgets(dict_word, sizeof(dict_word), fp) != NULL) {
        size_t len = strlen(dict_word);

        if (len > 0 && dict_word[len - 1] == '\n') {
            dict_word[len - 1] = '\0';
            len--;
        }

        if (len != word_1 && len != word_2) {
            continue;
        }

        if (len < prefix_len) {
            continue;
        }

        if (strncmp(dict_word, prefix, prefix_len) == 0) {
            if(found == 0) {
                printf("Trying word [%s]-> ", word);
            }
            printf("%s ", dict_word);
            found = 1;
        }
    }

    if (found) {
        printf("\n");
    }

    fclose(fp);
}

/* Function that scans possible space positions inside the buffer, 
   from each possible space position it computes two possible word sizes 
   then it tries to find prefixes or whole words that are of these sizes and are inside the filename given
   and can be xored to find matching pairs, if a prefix is found it tries to match it to whole words of the given two sizes. */
void scan_words_between_spaces(const unsigned char *buffer, size_t n, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if(fp == NULL) {
        printf("Could not open given filename: %s\n", filename);
        return;
    }

    size_t *space_positions = malloc(n * sizeof(size_t));
    if(space_positions == NULL) {
        printf("Allocation error\n");
        fclose(fp);
        return;
    }

    size_t number_of_spaces = collect_space_positions(buffer, n, space_positions);
    size_t s;
    for(s=0; s+2 < number_of_spaces; s++) {
        size_t word_end_1 = space_positions[s+1];
        size_t word_end_2 = space_positions[s+2];
        size_t word_start;
        if(s == 0) {
            word_start = 0;
            word_end_1 = space_positions[s];
            word_end_2 = space_positions[s+1];
        } else {
            word_start = space_positions[s];
            word_end_1 = space_positions[s+1];
            word_end_2 = space_positions[s+2];
        }

        size_t word_length_1 = word_end_1 - word_start;
        size_t word_length_2 = word_end_2 - word_start;

        printf("Scanning words of length %zu that match length %zu or length %zu... \n", word_length_1, word_length_1, word_length_2);
        char word[256];
        while(fgets(word, sizeof(word), fp) != NULL) {
            size_t len = strlen(word);
            if(len > 0 && word[len-1] == '\n') {
                word[len-1] = '\0';
                len--;
            }

            if(len != word_length_1 && len != word_length_2) {
                continue;
            }

            char other_pref_word[256];
            int exists = 1;
            size_t j;
            for(j = 0; j<word_length_1; j++) {
                unsigned char c = buffer[word_start+j] ^ (unsigned char) word[j];
                if(!not_garbage(c)) {
                    exists = 0;
                    break;
                }
                other_pref_word[j] = (char)c;
            }
            other_pref_word[word_length_1] = '\0';
            if(exists) {
                search_dict_prefix(other_pref_word, word, word_length_1, word_length_2, filename);
            }
        }
        rewind(fp);
    }

    free(space_positions);
    fclose(fp);
}

/* Function that converts a hex string into a hex stream.*/
unsigned char *otp_try_2_hex_to_bytes(const char* hex_string) {
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
        int first = otp_try_2_hex_value(hex_string[i]);
        int second = otp_try_2_hex_value(hex_string[i+1]);
        
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
        otp_try_2_explain_usage();
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
        otp_try_2_print_hex(key, n);
        printf("Ciphertext:\n");
        otp_try_2_print_hex(ciphertext, n);
        
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

        unsigned char *cipherhex_bytes = otp_try_2_hex_to_bytes(cipherhex);
        unsigned char *key_bytes = otp_try_2_hex_to_bytes(key);
        
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
        otp_try_2_print_hex(plaintext, decoded_n);
        printf("Result in ASCII format: \n%s\n", plaintext);

        free(cipherhex_bytes);
        free(key_bytes);
        free(plaintext);

    } else if (strcmp(argv[1], "recover") == 0 && argc == 2) {
            const char *cipher1_string = "72814c04ba04a4f8a05f458d2e26bcef7e433c4ee43d5d7f1c9c0bc8acf8d714c6c9324c30da3dc13f3411e08769b2949535dfd386";
            const char *cipher2_string = "6f904804a949f6febc5c48c83d75f3e3690b294ca9311b2f04975e83acf79e4689c9774a26dc68c73e2548ab8b63e187883edb9087";

            unsigned char *cipher1_hex = otp_try_2_hex_to_bytes(cipher1_string);
            unsigned char *cipher2_hex = otp_try_2_hex_to_bytes(cipher2_string);
            if(cipher1_hex == NULL || cipher2_hex == NULL) {
                free(cipher1_hex);
                free(cipher2_hex);
                printf("Error in translating cipher to hexadecimal\n");
                return 1;
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
                return 1;
            }
            
            xor(cipher1_hex, cipher2_hex, no_key_ciphers, n);

            size_t *space_positions = malloc(n * sizeof(size_t));
            if (space_positions == NULL) {
                printf("Allocation error\n");
                free(cipher1_hex);
                free(cipher2_hex);
                free(no_key_ciphers);
                free(key);
                return 1;
            }

            size_t number_of_spaces = collect_space_positions(no_key_ciphers, n, space_positions);

            printf("Space positions (%zu): ", number_of_spaces);
            for (size_t i = 0; i < number_of_spaces; i++) {
                printf("%zu ", space_positions[i]);
            }
            printf("\n");
            
            scan_words_between_spaces(no_key_ciphers, n, "words.txt");
            
    } else {
        otp_try_2_explain_usage();
        return 1;
    }
    return 0;
}
