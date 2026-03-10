#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void explain_usage() {
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

int convert_to_key_integer(char *key_string) {
    char *endptr; 
    long key_val = strtol(key_string, &endptr, 10);

    if (endptr == key_string || *endptr != '\0' || key_val < 0 || key_val > 25) {
        printf("Invalid key...\n\n");
        explain_usage();
        return -1;
    }

    return (int)key_val;
}

void encrypt(const char *plaintext, size_t n, char *ciphertext, int key) {
    size_t i;
    for(i=0; i<n; i++) {
        char c = plaintext[i];
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
        if(c >= 'A' && c <= 'Z') {
            freq_table[c - 'A']++;
        } else if(c >= 'a' && c <= 'z') {
            freq_table[c - 'a']++;
        }
    }

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

int main(int argc, char *argv[]) {
    if(argc < 2) {
        explain_usage(); 
        return 1;
    }

    if(strcmp(argv[1], "encrypt") == 0 && (argc == 3 || argc == 4)) {
        const char *plaintext = argv[2];
        int key = 1;
        
        if(argc == 4) {
            key = convert_to_key_integer(argv[3]);
            if(key == -1) {
                return 1;
            }
        }

        size_t n = strlen(plaintext);
        char *ciphertext = malloc((n+1) * sizeof(char));
        if(ciphertext == NULL) {
            printf("Allocation error\n");
            return 1;
        }
        ciphertext[n] = '\0';
        encrypt(plaintext, n, ciphertext, key);
        printf("Plaintext Input:\n");
        printf("%s\n", plaintext);
        printf("Ciphertext Output with key [%d]: \n", key);
        printf("%s\n", ciphertext);
        free(ciphertext);
    } else if (strcmp(argv[1], "decrypt") == 0 && (argc == 3 || argc == 4)) {
        const char *ciphertext = argv[2];
        int key = 1;
        
        if(argc == 4) {
            key = convert_to_key_integer(argv[3]);
            if(key == -1) {
                return 1;
            }
        }

        size_t n = strlen(ciphertext);
        char *plaintext = malloc((n+1) * sizeof(char));
        if(plaintext == NULL) {
            printf("Allocation error\n");
            return 1;
        }
        plaintext[n] = '\0';
        decrypt(ciphertext, n, plaintext, key);
        printf("Ciphertext Input:\n");
        printf("%s\n", ciphertext);
        printf("Plaintext Output with key [%d]: \n", key);
        printf("%s\n", plaintext);
        free(plaintext);
    } else if(strcmp(argv[1], "bruteforce") == 0 && argc == 3) {
        const char *ciphertext = argv[2];
        size_t n = strlen(ciphertext);
        printf("Ciphertext given: \n");
        printf("%s\n", ciphertext);
        printf("Results of bruteforcing: \n");
        bruteforce(ciphertext, n);
    } else if (strcmp(argv[1], "analysis") == 0 && argc == 3) {
        const char *ciphertext = argv[2];
        size_t n = strlen(ciphertext);
        printf("Ciphertext given: \n");
        printf("%s\n", ciphertext);
        printf("Results of analysis attack: \n");
        analysis(ciphertext, n);
    } else {
        explain_usage();
        return 1;
    }
}