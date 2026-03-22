#ifndef OTP_H
#define OTP_H

void otp_explain_usage();

void xor(const unsigned char *input1, const unsigned char *input2, unsigned char *output, size_t n);

void otp_print_hex(const unsigned char *buffer, size_t n);

int otp_hex_value(char c);

int not_garbage(unsigned char c);

void try_given_word(const unsigned char *buffer, size_t l, const char *word);

int is_in_dictionary(const unsigned char *word, const char *filename);

void words_scanner(const unsigned char *buffer, size_t l, const char *filename);

unsigned char *otp_hex_to_bytes(const char *hex_string);

void otp_recover_demo();

void otp_decrypt_demo(const char *cipherhex, const char *key);

void otp_encrypt_demo(const char *plaintext_str);

#endif
