///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020 Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission for CS 354 Fall 2020, Deb Deppeler
//
////////////////////////////////////////////////////////////////////////////////
// Main File:        n_in_a_row.c
// This File:        n_in_a_row.c
// Other Files:
// Semester:         CS 354 Fall 2020
//
// Author:           Aiden Tepper
// Email:            ajtepper@wisc.edu
// CS Login:         tepper
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   Fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          N/A
//
//
// Online sources:   N/A
//
//
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *DELIM = ",";  // commas ',' are a common delimiter character for data strings

/*
* Retrieves from the first line of the input file,
* the size of the board (number of rows and columns).
*
* fp: file pointer for input file
* size: pointer to size
*/
void get_dimensions(FILE *fp, int *size) {
    char *line = NULL;
    size_t len = 0;
    if (getline(&line, &len, fp) == -1) {
        printf("Error in reading the file.\n");
        exit(1);
    }

    char *token = NULL;
    token = strtok(line, DELIM);
    *size = atoi(token);
}



/*
* Returns 1 if and only if the board is in a valid state.
* Otherwise returns 0.
*
* board: heap allocated 2D board
* size: number of rows and columns
*/
int n_in_a_row(int **board, int size) {

    //check that the size is odd
    if(size % 2 != 1) {
        return 0;
    }

    //check that the number of Xs and Os are valid
    int xCount = 0;
    int oCount = 0;
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            int space = *(*(board + i)+j);
            if(space == 1) {
                ++xCount;
            } else if(space == 2) {
                ++oCount;
            }
        }
    }
    if(xCount != oCount && xCount-oCount != 1) {
        return 0;
    }

    //counters for below loops that ensure either one winner or no winners
    int xWinsH = 0;
    int oWinsH = 0;
    int xWinsV = 0;
    int oWinsV = 0;
    int x;
    int o;

    for(int i = 0; i < size; i++) { //check for horizontal wins
        x = 0;
        o = 0;
        for(int j = 0; j < size; j++) {
            int space = *(*(board+i)+j);
            if(space == 1) {
                ++x;
            } else if(space == 2) {
                ++o;
            }
        }
        if(x == size) { //if whole row is x
            if(xWinsH == 1) { //if x has already had n in a row, then parallel rows (invalid)
                return 0;
            } else {
                xWinsH = 1;
            }
        }
        if(o == size) { //if whole row is o
            if(oWinsH == 1) { //if o has already had n in a row, then parallel rows (invalid)
                return 0;
            } else {
                oWinsH = 1;
            }
        }
    }

    for(int i = 0; i < size; i++) { //check for vertical wins
        x = 0;
        o = 0;
        for(int j = 0; j < size; j++) {
            int space = *(*(board+j)+i);
            if(space == 1) {
                ++x;
            } else if(space == 2) {
                ++o;
            }
        }
        if(x == size) { //if whole column is x
            if(xWinsV == 1) { //if x has already had n in a column, then parallel columns (invalid)
                return 0;
            } else {
                xWinsV = 1;
            }
        }
        if(o == size) { //if whole column is o
            if(oWinsV == 1) { //if o has already had n in a column, then parallel columns (invalid)
                return 0;
            } else {
                oWinsV = 1;
            }
        }
    }

    //check if the board has both x and o as winners
    int xTotalWins = xWinsV + xWinsH;
    int oTotalWins = oWinsV + oWinsH;
    if(xTotalWins > 0 && oTotalWins > 0) {
        return 0;
    }

    //if no test failed, then return 1 (valid)
    return 1;

}



/* PARTIALLY COMPLETED:
* This program prints Valid if the input file contains
* a game board with either 1 or no winners; and where
* there is at most 1 more X than O.
*
* argc: CLA count
* argv: CLA value
*/
int main(int argc, char *argv[]) {

    //TODO: Check if number of command-line arguments is correct.
    if(argc != 2) {
        printf("Incorrect number of command-line arguments");
        exit(1);
    }

    //Open the file and check if it opened successfully.
    FILE *fp = fopen(*(argv + 1), "r");
    if (fp == NULL) {
        printf("Can't open file for reading.\n");
        exit(1);
    }

    //Declare local variables.
    int size;
    int valid;

    //TODO: Call get_dimensions to retrieve the board size.
    int **board;
    get_dimensions(fp, &size);

    //TODO: Dynamically allocate a 2D array of dimensions retrieved above.
    board = malloc(sizeof(int*)*size);
    if(board == NULL) {
        printf("Error when calling malloc().\n");
        exit(1);
    }
    for(int i = 0; i < size; i++) {
        *(board+i) = malloc(sizeof(int)*size);
    }

    //Read the file line by line.
    //Tokenize each line wrt the delimiter character to store the values in your 2D array.
    char *line = NULL;
    size_t len = 0;
    char *token = NULL;
    for (int i = 0; i < size; i++) {

        if (getline(&line, &len, fp) == -1) {
            printf("Error while reading the file.\n");
            exit(1);
        }

        token = strtok(line, DELIM);
        for (int j = 0; j < size; j++) {
            //TODO: Complete the line of code below
            //to initialize your 2D array.
            *(*(board+i)+j) = atoi(token);
            token = strtok(NULL, DELIM);
        }
    }

    //TODO: Call the function n_in_a_row and print the appropriate
    //output depending on the function's return value.
    valid = n_in_a_row(board, size);
    if(valid == 1) {
        printf("valid\n");
    } else if(valid == 0) {
        printf("invalid\n");
    }

    //TODO: Free all dynamically allocated memory.
    for(int i = 0; i < size; i++) {
        free(*(board + i));
        *(board + i) = NULL;
    }

    //Close the file.
    if (fclose(fp) != 0) {
        printf("Error while closing the file.\n");
        exit(1);
    }

    return 0;
}



// FIN
