#ifndef CAESAR_H
#define CAESAR_H

void caesar_explain_usage();

int convert_to_key_integer(char *key_string);

void encrypt(const char *plaintext, size_t n, char *ciphertext, int key);

void decrypt(const char *ciphertext, size_t n, char *plaintext, int key);

void bruteforce(const char *ciphertext, size_t n);

void analysis(const char *ciphertext, size_t n);

void caesar_encrypt_demo(const char *plaintext, int key);

void caesar_decrypt_demo(const char *ciphertext, int key);

void caesar_bruteforce_demo(const char *ciphertext);

void caesar_analysis_demo(const char *ciphertext);

#endif
