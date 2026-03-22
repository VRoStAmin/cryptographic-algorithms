#ifndef PRNGSTREAM_H
#define PRNGSTREAM_H

void prngstream_explain_usage();

int convert_to_seed_integer(char *seed_string);

void xor_stream_cipher(const unsigned char *input, size_t n, unsigned char *output, unsigned int seed);

void prngstream_print_hex(const unsigned char *buffer, size_t n);

int prngstream_hex_value(char c);

unsigned char *prngstream_hex_to_bytes(const char *hex_string);

void reuse_seed_demo();
void prngstream_encrypt_demo(const char *plaintext, int seed);
void prngstream_decrypt_demo(const char *ciphertext_hex, int seed);
void prngstream_reuse_demo();

#endif
