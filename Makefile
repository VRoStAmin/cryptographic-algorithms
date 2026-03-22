CC = gcc
CFLAGS = -Wall -Wextra

all: build_caesar build_otp build_otp_try_2 build_passattack build_prngstream build_secretsharing build_test

build_caesar: caesar/caesar.c caesar/caesar.h caesar/caesar_main.c
	$(CC) $(CFLAGS) -o caesar/caesar caesar/caesar.c caesar/caesar_main.c

build_otp: otp/otp.c otp/otp.h otp/otp_main.c
	$(CC) $(CFLAGS) -o otp/otp otp/otp.c otp/otp_main.c

build_otp_try_2: otp_try_2/otp_try_2.c otp_try_2/otp_try_2.h
	$(CC) $(CFLAGS) -o otp_try_2/otp_try_2 otp_try_2/otp_try_2.c

build_passattack: passattack/passattack.c passattack/passattack.h passattack/passattack_main.c
	$(CC) $(CFLAGS) -o passattack/passattack passattack/passattack.c passattack/passattack_main.c

build_prngstream: prngstream/prngstream.c prngstream/prngstream.h prngstream/prngstream_main.c
	$(CC) $(CFLAGS) -o prngstream/prngstream prngstream/prngstream.c prngstream/prngstream_main.c

build_secretsharing: secretsharing/secretsharing.c secretsharing/secretsharing.h secretsharing/secretsharing_main.c
	$(CC) $(CFLAGS) -o secretsharing/secretsharing secretsharing/secretsharing.c secretsharing/secretsharing_main.c

build_test: test.c caesar/caesar.c caesar/caesar.h otp/otp.c otp/otp.h passattack/passattack.c passattack/passattack.h prngstream/prngstream.c prngstream/prngstream.h secretsharing/secretsharing.c secretsharing/secretsharing.h
	$(CC) $(CFLAGS) -o test test.c caesar/caesar.c otp/otp.c passattack/passattack.c prngstream/prngstream.c secretsharing/secretsharing.c


clean:
	rm -f caesar/caesar otp/otp otp_try_2/otp_try_2 passattack/passattack prngstream/prngstream secretsharing/secretsharing test
