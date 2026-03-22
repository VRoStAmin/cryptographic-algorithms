#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "secretsharing.h"

void secretsharing_explain_usage() {
    printf(
        "Your input was incorrect. Please use the following format:\n"
        "To create three points from a given secret value D: ./secretsharing create D\n"
        "To recover the secret value D given two points: ./secretsharing recover x1 y1 x2 y2\n"
    );
}

/* string to number */
long int string_to_number(char *str_num) {
    char *endptr;
    double value = strtod(str_num, &endptr);

    if (endptr == str_num || *endptr != '\0') {
        printf("Invalid input...\n\n");
        secretsharing_explain_usage();
        return 0;
    }

    return value;
}

/* creates three points from the secret D that is given */
void create_points(long int D) {
    long int a;
    long int x1, x2, x3;
    long int y1, y2, y3;

    srand(time(NULL));
    a = (rand() % 500) + 1;
    x1 = (rand() % 500) + 1;
    x2 = (rand() % 500) + 1;

    while(x1 == x2) {
        x2 = (rand() % 500) + 1;
    }

    x3 = (rand() % 500) + 1;
    while(x3 == x1 || x3 == x2) {
        x3 = (rand() % 500) + 1;
    }

    y1 = a * x1 + D;
    y2 = a * x2 + D;
    y3 = a * x3 + D;

    printf("Secret D given: %ld\n", D);
    printf("Random a gotten: %ld\n", a);

    printf("The three points generated are:\n");
    printf("[%ld, %ld]\n", x1, y1);
    printf("[%ld, %ld]\n", x2, y2);
    printf("[%ld, %ld]\n", x3, y3);
}

/* Finds D given two points*/
long int recover_d(long int x1, long int y1, long int x2, long int y2) {
    long int a = (y2-y1)/(x2-x1);
    long int D = y1 - (a*x1);
    return D;
}

void secretsharing_create_demo(long int d_value) {
    create_points(d_value);
}

int secretsharing_recover_demo(long int x1, long int y1, long int x2, long int y2) {
    if(x1 == x2) {
        printf("x1 and x2 must be different.\n");
        return 1;
    }

    printf("Input:\n");
    printf("Point 1: [%ld, %ld]\n", x1, y1);
    printf("Point 2: [%ld, %ld]\n", x2, y2);

    long int D = recover_d(x1, y1, x2, y2);
    printf("The secret password D is: %ld\n", D);
    return 0;
}
