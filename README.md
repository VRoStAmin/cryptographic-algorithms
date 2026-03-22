This is the assignment 1 of HY457 Introduction to secure systems

Mastorakis Emmanouil csd5255

The implemented sections are as follows: 
 - One time pad (encryption, decryption and reused key analysis)
 - Caesar algorithm (encryption, decryption, brute-force attack and frequency analysis)
 - PRNG stream cipher algorithm (encryption, decryption, demo of why seed reuse is bad) - Password guessing timing attack (using average execution time and timers)
 - Secret sharing y=ax+D (creating the three points and reconstructing D out of any two point)

Each part is implemented as follows to its dedicated directory
 there is a .c file containing the implementation of the algorithm
 there is a .h file containing function declarations
 there is a main.c for each program allowing use of the algorithm from the cmd
 
There is also a separate test.c program that utilizes the implemented functions for every algorithm. 

In the given otp.c algorithm words.txt is not used, 
there is a seperate .c file called otp_try_2.c that tries to use the words.txt file 
to break the given ciphertexts decrypted with the same otp key with the following logic: 
 
 C1 XOR C2 = (P1 XOR SAME_KEY) XOR (P2 XOR SAME_KEY) = P1 XOR P2
 In otp.c, words.txt was not used, instead I tried to guess the plaintexts xoring the P1 XOR P2 with different known words.
 By guessing words and by placing them in all the positions that fit, then xoring them, I was given back some texts that I could then 
 try to place in different areas again, or that helped me guess the plaintext even more. 

 After talking with the teaching assistant I got the information that there is a way to do it with the words.txt
 So in the otp_try_2.c what I tried to do was use the words.txt like this: 
    First xor all the characters in the P1 XOR P2 with the space char ' ', 
    if what was returned was not garbage and not ' ' then there was a high chance there was a space in that index. 
 
 So after mapping the boundaries of the words in P1 XOR P2 
 I tried to take all the words in words.txt and xor them with all the words in length that matched the boundaries in that position. 
 Since there is no way to know which plaintext has what space, I tried taking more than one boundary and just checking all the words of two specific lengths each time. 
 This logic seemed really slow still, checking every word in a specific length and then checking the xored result still exists was a slow proccess... 
 I managed to find the beggining of the plaintexts this way... but then automatically finding the plaintexts like that wouldn't have worked...
 
 I could have then taken the first two words like this, then start guessing again. 


 

