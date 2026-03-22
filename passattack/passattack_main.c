#include <stdio.h>
#include <string.h>
#include "passattack.h"

int main(int argc, char *argv[]) {
    if(argc < 2) {
        passattack_explain_usage();
        return 1;
    }

    if(strcmp(argv[1], "input") == 0 && argc == 3) {
        passattack_input_demo(argv[2]);
    } else if(strcmp(argv[1], "attack") == 0 && argc == 2) {
        passattack_attack_demo();
    } else {
        passattack_explain_usage();
        return 1;
    }

    return 0;
}
