#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "caesar.h"

void caesar_explain_usage() {
    printf(
        "Your input was incorrect. Please use the following format:\n"
        "To encrypt using CAESAR (default key shift of 1): ./caesar encrypt \"plaintext\"\n"
        "To encrypt using CAESAR and a specific key (0-25): ./caesar encrypt \"plaintext\" key\n"
        "To decrypt using CAESAR (default decryption of key 1): ./caesar decrypt \"ciphertext\"\n"
        "To decrypt using CAESAR and a specific key (0-25): ./caesar decrypt \"ciphertext\" key\n"
        "To bruteforce a CAESAR ciphertext: ./caesar bruteforce \"ciphertext\"\n"
        "To perform an analysis attack on a CAESAR ciphertext: ./caesar analysis \"ciphertext\"\n"
    );
}

/* Converts a key from the command line into an integer. */
int convert_to_key_integer(char *key_string) {
    char *endptr; 
    long key_val = strtol(key_string, &endptr, 10);

    if (endptr == key_string || *endptr != '\0' || key_val < 0 || key_val > 25) {
        printf("Invalid key...\n\n");
        caesar_explain_usage();
        return -1;
    }

    return (int)key_val;
}

/* Encrypts a plaintext with the given key */
void encrypt(const char *plaintext, size_t n, char *ciphertext, int key) {
    size_t i;
    for(i=0; i<n; i++) {
        char c = plaintext[i];
        /* c - 'A' make the range of the shift be from 0-25, then it uses % 26 so it can wrap around 
           the alphabet, then this result is how many letters as the key it should shift.
           so we add A to make it into the letter range again and that is the result. */
        if(c >= 'A' && c <= 'Z') {
            ciphertext[i] = 'A' + ((c - 'A' + key) % 26); 
        } else if (c >= 'a' && c <= 'z') {
            ciphertext[i] = 'a' + ((c - 'a' + key) % 26);
        } else {
            ciphertext[i] = c; 
        }
    }
    ciphertext[n] = '\0';
}


void decrypt(const char *ciphertext, size_t n, char *plaintext, int key) {
    size_t i;
    for(i=0; i<n; i++) {
        char c = ciphertext[i];
        /* c - 'A' makes the letter into the range 0-25 again, but this time we subtract
           the key because we are undoing the shift.
           we add 26 so it does not go negative when it wraps around.
           then % 26 keeps it in the alphabet range, and adding A makes it a letter again.
           example: if c is B and key is 3, then 1 - 3 = -2, so adding 26 makes it 24,
           and 24 is Y. */
        if(c >= 'A' && c <= 'Z') {
            plaintext[i] = 'A' + ((c - 'A' - key + 26) % 26); 
        } else if (c >= 'a' && c <= 'z') {
            plaintext[i] = 'a' + ((c - 'a' - key + 26) % 26);
        } else {
            plaintext[i] = c;
        }
    }
    plaintext[n] = '\0';
}

void bruteforce(const char *ciphertext, size_t n) {
    char *plaintext = malloc((n+1) * sizeof(char));
    if(plaintext == NULL) {
        printf("Allocation error\n");
        return;
    }

    int i;
    for(i = 0; i<=25; i++) {
        decrypt(ciphertext, n, plaintext, i);
        printf("Key[%d]:\n", i);
        printf("%s\n", plaintext);
    }
    free(plaintext);
}

void analysis(const char *ciphertext, size_t n) {
    int freq_table[26] = {0};
    size_t i;
    for(i=0; i<n; i++) {
        char c = ciphertext[i];
        /* if the letter is uppercase, convert it into a number 0-25
           and count how many times it appears. */
        if(c >= 'A' && c <= 'Z') {
            freq_table[c - 'A']++;
        } else if(c >= 'a' && c <= 'z') {
            freq_table[c - 'a']++;
        }
    }

    /* start by assuming the most common letter is a */
    int most_letter_pos = 0;
    int pos;
    for(pos=1; pos<=25; pos++) {
        if(freq_table[pos] > freq_table[most_letter_pos]) {
            most_letter_pos = pos;
        }
    }

    int keys[3];
    keys[0] = (most_letter_pos - ('e' - 'a') + 26) % 26;
    keys[1] = (most_letter_pos - ('t' - 'a') + 26) % 26;
    /* the key[2] is just most_letter_pos but we should show that this means that most_letter_pos is getting mapped with the letter a.*/
    keys[2] = (most_letter_pos - ('a' - 'a') + 26) % 26; 
    char most_used_enlgish_letters[3] = {'e', 't', 'a'};

    char *plaintext = malloc((n+1) * sizeof(char));
    if(plaintext == NULL) {
        printf("Allocation error\n");
        return;
    }

    printf("Analysis attack results: \n");
    printf("Most frequent ciphertext letter: %c\n", 'a' + most_letter_pos);
    for(pos=0; pos<3; pos++) {
        decrypt(ciphertext, n, plaintext, keys[pos]);
        printf("Decrypted with key[%d]\n", keys[pos]);
        printf("Letter [%c] is truly the letter [%c]\n", 'a' + most_letter_pos, most_used_enlgish_letters[pos]);
        printf("%s\n", plaintext);
    }
    free(plaintext);
}

void caesar_encrypt_demo(const char *plaintext, int key) {
    size_t n = strlen(plaintext);
    char *ciphertext = malloc((n+1) * sizeof(char));
    if(ciphertext == NULL) {
        printf("Allocation error\n");
        return;
    }
    ciphertext[n] = '\0';
    encrypt(plaintext, n, ciphertext, key);
    printf("Plaintext Input:\n");
    printf("%s\n", plaintext);
    printf("Ciphertext Output with key [%d]: \n", key);
    printf("%s\n", ciphertext);
    free(ciphertext);
}

void caesar_decrypt_demo(const char *ciphertext, int key) {
    size_t n = strlen(ciphertext);
    char *plaintext = malloc((n+1) * sizeof(char));
    if(plaintext == NULL) {
        printf("Allocation error\n");
        return;
    }
    plaintext[n] = '\0';
    decrypt(ciphertext, n, plaintext, key);
    printf("Ciphertext Input:\n");
    printf("%s\n", ciphertext);
    printf("Plaintext Output with key [%d]: \n", key);
    printf("%s\n", plaintext);
    free(plaintext);
}

void caesar_bruteforce_demo(const char *ciphertext) {
    size_t n = strlen(ciphertext);
    printf("Ciphertext given: \n");
    printf("%s\n", ciphertext);
    printf("Results of bruteforcing: \n");
    bruteforce(ciphertext, n);
}

void caesar_analysis_demo(const char *ciphertext) {
    size_t n = strlen(ciphertext);
    printf("Ciphertext given: \n");
    printf("%s\n", ciphertext);
    printf("Results of analysis attack: \n");
    analysis(ciphertext, n);
}
