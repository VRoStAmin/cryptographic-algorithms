#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

const char *pass = "TESTTT";

void explain_usage() {
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

    for(i=0; i<pass_size; i++) {
        if(i >= guess_size) {return 0;}
        if(guess[i] != pass[i]) {return 0;}
        usleep(30000);
    }

    return pass_size == guess_size;
}

double time_insecure_check(const char *guess) {
    struct timespec begin;
    struct timespec end;

    clock_gettime(CLOCK_MONOTONIC, &begin);
    insecure_check(guess);
    clock_gettime(CLOCK_MONOTONIC, &end);

    double seconds = (double)(end.tv_sec-begin.tv_sec);
    double nanoseconds = (double)(end.tv_nsec-begin.tv_nsec);

    return seconds*1000.0+nanoseconds/1000000.0;
}

double average_time_insecure_check(const char *guess) {
    double sum = 0.0;
    int i;
    for(i=0; i<10; i++) {
        sum += time_insecure_check(guess);
    }
    double avg = sum/10;
    return avg;
}

void attack_password() {
    const char *password_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890!@#$%^&*()-_=+[]{}|;:'\",.<>/?\\`~";
    size_t password_chars_size = strlen(password_chars);
    size_t max_password_size = 256;

    char *guess = malloc((max_password_size + 1) * sizeof(char));
    if (guess == NULL) {
        printf("Allocation error\n");
        return;
    }

    size_t i;
    for(i=0; i<=max_password_size; i++) {
        guess[i] = '\0';
    }

    printf("Attack started\n");
    for(i=0; i<max_password_size; i++) {
        char best_pass = password_chars[0];
        double faster_time = 0.0;
        double avg;
        size_t j;
        for(j=0; j<password_chars_size; j++) {
            guess[i] = password_chars[j];
            guess[i+1] = '\0';
            avg = average_time_insecure_check(guess);
            if(avg > faster_time) {
                faster_time = avg;
                best_pass = password_chars[j];
            }
        }

        guess[i] = best_pass;
        guess[i+1] = '\0';
        printf("Current password guessed: %s\n", guess);
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

int main(int argc, char *argv[]) {
    if(argc < 2) {
        explain_usage();
        return 1;
    }

    if(strcmp(argv[1], "input") == 0 && argc == 3) {
        const char *guess = argv[2];
        int guessed = insecure_check(guess);
        printf("Guess password: \n");
        printf("%s\n", guess);

        if(guessed) {
            printf("Password correct\n");
        } else {
            printf("Password incorrect\n");
        }
    } else if(strcmp(argv[1], "attack") == 0 && argc == 2) {
        attack_password();
    } else {
        explain_usage();
        return 1;
    }
}

