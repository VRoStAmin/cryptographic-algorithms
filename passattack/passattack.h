#ifndef PASSATTACK_H
#define PASSATTACK_H

void passattack_explain_usage();

int insecure_check(const char *guess);

double time_insecure_check(const char *guess);

double average_time_insecure_check(const char *guess);

void attack_password();

void passattack_input_demo(const char *guess);

void passattack_attack_demo();

#endif
