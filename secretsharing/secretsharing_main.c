#include <stdio.h>
#include <string.h>
#include "secretsharing.h"

int main(int argc, char *argv[]) {
    if(argc < 2) {
        secretsharing_explain_usage();
        return 1;
    }

    if(strcmp(argv[1], "create") == 0 && argc == 3) {
        long int d_value = string_to_number(argv[2]);
        secretsharing_create_demo(d_value);
    } else if (strcmp(argv[1], "recover") == 0 && argc == 6) {
        long int x1 = string_to_number(argv[2]);
        long int y1 = string_to_number(argv[3]);
        long int x2 = string_to_number(argv[4]);
        long int y2 = string_to_number(argv[5]);

        return secretsharing_recover_demo(x1, y1, x2, y2);
    } else {
        secretsharing_explain_usage();
        return 1;
    }

    return 0;
}
