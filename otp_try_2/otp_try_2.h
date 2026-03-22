#ifndef OTP_TRY_2_H
#define OTP_TRY_2_H

void otp_try_2_explain_usage();

void xor(const unsigned char *input1, const unsigned char *input2, unsigned char *output, size_t n);

void otp_try_2_print_hex(const unsigned char *buffer, size_t n);

int otp_try_2_hex_value(char c);

int not_garbage(unsigned char c);

void try_given_word(const unsigned char *buffer, size_t l, const char *word);

int is_in_dictionary(const char *word, const char *filename);

size_t collect_space_positions(const unsigned char *buffer, size_t n, size_t *positions);

void search_dict_prefix(char *prefix, char *word, size_t word_1, size_t word_2, const char *filename);

void scan_words_between_spaces(const unsigned char *buffer, size_t n, const char *filename);

unsigned char *otp_try_2_hex_to_bytes(const char *hex_string);

#endif
