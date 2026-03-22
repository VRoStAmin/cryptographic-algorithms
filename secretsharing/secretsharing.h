#ifndef SECRETSHARING_H
#define SECRETSHARING_H

void secretsharing_explain_usage();

long int string_to_number(char *str_num);

void create_points(long int D);

long int recover_d(long int x1, long int y1, long int x2, long int y2);
void secretsharing_create_demo(long int d_value);
int secretsharing_recover_demo(long int x1, long int y1, long int x2, long int y2);

#endif
