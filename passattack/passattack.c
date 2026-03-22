#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "passattack.h"

const char *pass = "ABC";

void passattack_explain_usage() {
    printf(
        "Your input was incorrect. Please use the following format:\n"
        "To test a password guess againt the vulnerable server: ./passattack input \"guess\"\n"
        "To run a timing attack and and find the password: ./passattack attack\n"
    );
}

int insecure_check(const char *guess) {
    size_t pass_size = strlen(pass);
    size_t guess_size = strlen(guess);
    size_t i;

    /* Checks the guess one character at a time, like the exercise given.*/
    for(i=0; i<pass_size; i++) {
        if(i >= guess_size) {return 0;} /* if guess is too short it is wrong. */
        if(guess[i] != pass[i]) {return 0;} /* if one char is wrong we stop. */
        usleep(30000); /* if it is correct there is a small delay before continuing. */
    }

    /* if the for loop is finished and the sizes are correct then return that the pass
       is correct too. */
    return pass_size == guess_size;
}

double time_insecure_check(const char *guess) {
    /* We use this type of clock because the other ones will not count 
    during sleep and we want to count sleep as well. */
    struct timespec begin;
    struct timespec end;

    clock_gettime(CLOCK_MONOTONIC, &begin);
    insecure_check(guess); /* times how long a guess takes to run. */
    clock_gettime(CLOCK_MONOTONIC, &end);

    double seconds = (double)(end.tv_sec-begin.tv_sec);
    double nanoseconds = (double)(end.tv_nsec-begin.tv_nsec);

    return seconds*1000.0+nanoseconds/1000000.0; /* return in mill seconds */
}

double average_time_insecure_check(const char *guess) {
    double sum = 0.0;
    int i;
    /* runs the check 10 times and then averages it so there is no noise. */
    for(i=0; i<10; i++) {
        sum += time_insecure_check(guess); 
    }
    double avg = sum/10;
    return avg;
}

void attack_password() {
    /* all the characters that could be in the password. */
    const char *password_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890!@#$%^&*()-_=+[]{}|;:'\",.<>/?\\`~";
    size_t password_chars_size = strlen(password_chars);
    size_t max_password_size = 256;

    /* we start bruteforce by having a password be the maximum size of the password
       that can be created in the server. */
    char *guess = malloc((max_password_size + 1) * sizeof(char));
    if (guess == NULL) {
        printf("Allocation error\n");
        return;
    }

    /* filling it with null chars at first. */
    size_t i;
    for(i=0; i<=max_password_size; i++) {
        guess[i] = '\0';
    }

    /* we find one char at a time. */
    printf("Attack started\n");
    for(i=0; i<max_password_size; i++) {
        char best_pass = password_chars[0];
        double faster_time = 0.0;
        double avg;
        size_t j;
        /* try every possible char in this pos. */
        for(j=0; j<password_chars_size; j++) {
            guess[i] = password_chars[j];
            guess[i+1] = '\0';
            /* measure average time taken for this char for this guess. */
            avg = average_time_insecure_check(guess);
            /* the slowest guess is the correct one. */
            if(avg > faster_time) {
                faster_time = avg;
                best_pass = password_chars[j];
            }
        }

        /* keep the best char that was found */
        guess[i] = best_pass;
        guess[i+1] = '\0';
        printf("Current password guessed: %s\n", guess);
        /* stop if the password found so far is the one. */
        if(insecure_check(guess) == 1) {
            printf("Password attack success!!!\n");
            printf("Password is: %s\n", guess);
            free(guess);
            return;
        }
    }
    printf("Attack not successfull with maximum password lenght of: %zu\n", max_password_size);
    free(guess);
}

void passattack_input_demo(const char *guess) {
    int guessed = insecure_check(guess);
    printf("Guess password: \n");
    printf("%s\n", guess);

    if(guessed) {
        printf("Password correct\n");
    } else {
        printf("Password incorrect\n");
    }
}

void passattack_attack_demo() {
    attack_password();
}
