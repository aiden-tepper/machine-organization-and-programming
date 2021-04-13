////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020 Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission, CS 354 Fall 2020, Deb Deppeler
//
////////////////////////////////////////////////////////////////////////////////
// Main File:        myMagicSquare.c
// This File:        myMagicSquare.c
// Other Files:
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure that represents a magic square
typedef struct {
    int size;      // dimension of the square
    int **magic_square; // pointer to heap allocated magic square
} MagicSquare;

/*
 * Prompts the user for the magic square's size, reads it,
 * checks if it's an odd number >= 3 (if not display the required
 * error message and exit), and returns the valid number.
 */
int getSize() {
    printf("Enter magic square's size (odd integer >=3)\n");
    int size;
    scanf("%d", &size);
    if(size % 2 != 1) {
        printf("Size must be odd.");
        exit(1);
    }
    if(size < 3) {
        printf("Size must be >= 3.");
        exit(1);
    }
    return size;
}

/*
 * Makes a magic square of size n using the alternate
 * Siamese magic square algorithm from assignment and
 * returns a pointer to the completed MagicSquare struct.
 *
 * n the number of rows and columns
 */
MagicSquare *generateMagicSquare(int n) {

    //local variables
    int currentNum = 1;
    int currentRow = (n-1)/2;
    int currentCol = n-1;
    int nextRow = currentRow;
    int nextCol = currentCol;

    //initialize 2D array
    int **magicSquare;
    magicSquare = malloc(sizeof(int*)*n);
    if(magicSquare == NULL) {
        printf("Error when calling malloc().\n");
        exit(1);
    }
    for(int i = 0; i < n; i++) {
        *(magicSquare + i) = malloc(sizeof(int)*n);
        if(*(magicSquare + i) == NULL) {
            printf("Error when calling malloc().\n");
            exit(1);
        }
    }

    //fill the array with 0s
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            *(*(magicSquare+i)+j) = 0;
        }
    }

    while(1) {
        *(*(magicSquare+currentRow)+currentCol) = currentNum;
        if(currentRow == n-1) { // if at the last row,
            nextRow = 0; // reset to first row
        } else { // otherwise,
            nextRow++; // move to next row
        }
        if(currentCol == n-1) { // if at the last column,
            nextCol = 0; // reset to first column
        } else { // otherwise,
            nextCol++; // move to next column
        }
        if(*(*(magicSquare+nextRow)+nextCol) != 0) { // if next square is already filled,
            nextCol = currentCol - 1; // move one square left
            nextRow = currentRow;
            if(*(*(magicSquare+nextRow)+nextCol) != 0) { // if that square is also filled,
                break; // magic square is complete
            }
        }
        currentCol = nextCol;
        currentRow = nextRow;
        currentNum++;
    }

    //initialize MagicSquare struct and magic_square array on the heap
    MagicSquare *msStruct = malloc(sizeof(MagicSquare));
    if(msStruct == NULL) {
        printf("Error when calling malloc().\n");
        exit(1);
    }
    msStruct -> magic_square = malloc(sizeof(int*)*n);
    if(msStruct -> magic_square == NULL) {
        printf("Error when calling malloc().\n");
        exit(1);
    }
    for(int i = 0; i < n; i++) {
        *((msStruct -> magic_square)+ i) = malloc(sizeof(int)*n);
        if(*((msStruct -> magic_square)+ i) == NULL) {
            printf("Error when calling malloc().\n");
            exit(1);
        }
    }

    //copy 2D array to struct 2D array
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            *(*((msStruct -> magic_square)+i)+j) = *(*(magicSquare+i)+j);
        }
    }

    //write size to struct size variable
    msStruct -> size = n;

    return msStruct;

}

/*
 * Opens a new file (or overwrites the existing file)
 * and writes the square in the specified format.
 *
 * magic_square the magic square to write to a file
 * filename the name of the output file
 */
void fileOutputMagicSquare(MagicSquare *magic_square, char *filename) {

    FILE *file = fopen(filename, "w");
    if(file == NULL) {
        printf("Error when calling fopen().\n");
        exit(1);
    }

    fprintf(file, "%d\n", magic_square -> size);

    for(int i = 0; i < magic_square -> size; i++) {
        for(int j = 0; j < magic_square -> size; j++) {
            if(j == (magic_square -> size) - 1) {
                fprintf(file, "%d", *(*((magic_square -> magic_square)+ i)+j));
            } else {
                fprintf(file, "%d,", *(*((magic_square -> magic_square)+ i)+j));
            }
        }
        fprintf(file, "\n");
    }

    int err = fclose(file);
    if(err != 0) {
        printf("Error when calling fclose().\n");
        exit(1);
    }

}

/*
 * Generates a magic square of the user specified size and
 * output the quare to the output filename
 */
int main(int argc, char *argv[]) {

    // Check input arguments to get output filename
    char *filename = *(argv+1);

    // Get magic square's size from user
    int size = getSize();

    // Generate the magic square
    MagicSquare *square = generateMagicSquare(size);

    // Output the magic square
    fileOutputMagicSquare(square, filename);

    // Free all dynamically allocated memory
    for(int i = 0; i < size; i++) {
        free(*((square -> magic_square)+ i));
        *((square -> magic_square)+ i) = NULL;
    }
    free(square -> magic_square);
    square -> magic_square = NULL;

    return 0;
}



//				myMagicSquare.c
