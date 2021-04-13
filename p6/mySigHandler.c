////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020 Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission, CS 354 Fall 2020, Deb Deppeler
//
////////////////////////////////////////////////////////////////////////////////
// Main File:        mySigHandler.c
// This File:        mySigHandler.c
// Other Files:      sendsig.c, division.c
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

// establish current time
time_t now;

// SIGUSR1 counter
int counter;

/* handler_SIGALRM:
 * signal handler for the SIGALRM signal, triggered by the alarm() function
 */
void handler_SIGALRM() {

    printf("PID: %d ", getpid());

    // check that setting the current time doesn't cause an error
    if(time(&now) == ((time_t)(-1))) {
        printf("time() was not successfully completed\n");
        exit(1);
    }

    // format the datetime
    char *formattedTime = ctime(&now);

    // check that the ctime() function doesn't cause an error
    if(formattedTime == NULL) {
        printf("ctime() was not successfully completed\n");
        exit(1);
    }

    printf("CURRENT TIME: %s", formattedTime);

    // set another 3-second alarm
    alarm(3);

}

/* handler_SIGUSR1:
 * signal handler for the SIGUSR1 signal, triggered by kill()
 */
void handler_SIGUSR1() {

    // increment the counter and print to console
    counter++;
    printf("SIGUSR1 handled and counted!\n");

}

/* handler_SIGINT:
 * signal handler for the SIGINT signal, triggered by ctrl+c
 */
void handler_SIGINT() {

    printf("\nSIGINT handled.\n");
    printf("SIGUSR1 was handled %d times. Exiting now.\n", counter);

    exit(0);

}

/* main:
 * main function that sets a 3 second alarm, registers signal handlers, and
 * enters an endless loop
 */
int main() {

    printf("Pid and time print every 3 seconds.\n");
    printf("Enter Ctrl-C to end the program.\n");

    // set the initial 3 second alarm
    alarm(3);

    // register the SIGALRM signal handler
    struct sigaction sa1; // create the sigaction struct
    memset(&sa1, 0, sizeof(sa1)); // execute the memset instruction, fills the block of memory with 0s
    sa1.sa_handler = &handler_SIGALRM; // assign the handler function to the sa_handler member of the struct
    if(sigaction(SIGALRM, &sa1, NULL) != 0) { // bind the signal handler and check that it's successful
        printf("Error binding SIGALRM handler\n");
        exit(1);
    }

    // register the SIGUSR1 signal handler
    struct sigaction sa2; // create the sigaction struct
    memset(&sa2, 0, sizeof(sa2)); // execute the memset instruction, fills the block of memory with 0s
    sa2.sa_handler = &handler_SIGUSR1; // assign the handler function to the sa_handler member of the struct
    if(sigaction(SIGUSR1, &sa2, NULL) != 0) { // bind the signal handler and check that it's successful
        printf("Error binding SIGUSR1 handler\n");
        exit(1);
    }

    // register the SIGINT signal handler
    struct sigaction sa3; // create the sigaction struct
    memset(&sa3, 0, sizeof(sa3)); // execute the memset instruction, fills the block of memory with 0s
    sa3.sa_handler = &handler_SIGINT; // assign the handler function to the sa_handler member of the struct
    if(sigaction(SIGINT, &sa3, NULL) != 0) { // bind the signal handler and check that it's successful
        printf("Error binding SIGINT handler\n");
        exit(1);
    }

    // endless loop
    while(1) {
    }

    return 0;

}
