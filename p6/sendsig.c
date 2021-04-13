////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020 Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission, CS 354 Fall 2020, Deb Deppeler
//
////////////////////////////////////////////////////////////////////////////////
// Main File:        mySigHandler.c
// This File:        sendsig.c
// Other Files:      mySigHandler.c, division.c
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

/* main:
 * main method that accepts command line arguments and sends the appropriate
 * signal to the appropriate PID
 *
 * int argc - the number of command line arguments
 * char *argv[] - pointer to array of command line arguments
 */
int main(int argc, char *argv[]) {

    // if incorrect number of arguments, show usage info
    if(argc != 3) {
        printf("Usage: <signal type> <pid>\n");
        return 0;
    }

    // set chars for valid command line arguments
    char str1[] = "-i";
    char str2[] = "-u";

    // handle each command line argument appropriately
    if(strcmp(argv[1], str1) == 0) { // if -i:
        if(kill(atoi(argv[2]), 2) == -1) { // execute kill() and check that it was successful
            printf("kill() was not successfully completed\n");
            exit(1);
        }
    } else if(strcmp(argv[1], str2) == 0) { // if -u:
        if(kill(atoi(argv[2]), 10) == -1) { // execute kill() and check that it was successful
            printf("kill() was not successfully completed\n");
            exit(1);
        }
    }

    return 0;

}
