////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020 Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission, CS 354 Fall 2020, Deb Deppeler
//
////////////////////////////////////////////////////////////////////////////////
// Main File:        division.c
// This File:        division.c
// Other Files:      mySigHandler.c, sendsig.c
// Semester:         CS 354 Fall 2020
//
// Author:           Aiden Tepper
// Email:            ajtepper@wisc.edu
// CS Login:         Tepper
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   Fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          N/A
//
// Online sources:   N/A
//
////////////////////////////////////////////////////////////////////////////////

#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// counter for total number of successul operations
int counter = 0;

/* handler_SIGFPE:
 * signal handler for SIGFPE, triggered by a division by 0
 */
void handler_SIGFPE() {

    printf("Error: a division by 0 operation was attempted.\n");
    printf("Total number of operations completed successfully: %d\n", counter);
    printf("The program will be terminated.\n");

    exit(0);

}

/* handler_SIGINT:
 * signal handler for the SIGINT signal, triggered by ctrl+c
 */
void handler_SIGINT() {

    printf("\nTotal number of operations completed successfully: %d\n", counter);
    printf("The program will be terminated.\n");

    exit(0);

}

/* main:
 * main function that registers/binds signal handlers and runs an infinite
 * loop that asks the user for two numbers and divides them, printing the output
 */
int main() {

    // register the SIGFPE signal handler
    struct sigaction sa1; // create the sigaction struct
    memset(&sa1, 0, sizeof(sa1)); // execute the memset instruction, fills the block of memory with 0s
    sa1.sa_handler = &handler_SIGFPE; // assign the handler function to the sa_handler member of the struct
    if(sigaction(SIGFPE, &sa1, NULL) != 0) { // bind the signal handler and check that it's successful
        printf("Error binding SIGFPE handler\n");
        exit(1);
    }

    // register the SIGINT signal handler
    struct sigaction sa2; // create the sigaction struct
    memset(&sa2, 0, sizeof(sa2)); // execute the memset instruction, fills the block of memory with 0s
    sa2.sa_handler = &handler_SIGINT; // assign the handler function to the sa_handler member of the struct
    if(sigaction(SIGINT, &sa2, NULL) != 0) { // bind the signal handler and check that it's successful
        printf("Error binding SIGINT handler\n");
        exit(1);
    }

    // infinite while loop
    while(1) {

        // get an integer from user input
        char num1char[100];
        int num1;
        printf("Enter first integer: ");
        if(fgets(num1char, sizeof(num1char), stdin) == NULL) { // get keyboard input and check for error
            printf("fgets() was not successfully completed\n");
            exit(1);
        }
        num1 = atoi(num1char); // convert char[] to int

        char num2char[100];
        int num2;
        printf("Enter second integer: ");
        if(fgets(num2char, sizeof(num2char), stdin) == NULL) { // get keyboard input and check for error
            printf("fgets() was not successfully completed\n");
            exit(1);
        }
        num2 = atoi(num2char); // convert char[] to int

        int answer = num1 / num2; // divide the two numbers to find the answer
        int remainder = num1 % num2; // mod the two numbers to find the remainder

        printf("%d / %d is %d with a remainder of %d\n", num1, num2, answer, remainder);

        counter++; // increment the counter

    }

    return 0;

}
