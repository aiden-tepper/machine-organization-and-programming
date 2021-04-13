////////////////////////////////////////////////////////////////////////////////
// Main File:        cache2Dcols.c
// This File:        cache2Dcols.c
// Other Files:      N/A
// Semester:         CS 354 Fall 2020
// Instructor:       deppeler
//
// Discussion Group: DISC 623 Tues 5-7pm
// Author:           Aiden Tepper
// Email:            ajtepper@wisc.edu
// CS Login:         tepper
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          N/A
//
// Online sources:   N/A
//
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2019-2020 Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission Fall 2020, CS354-deppeler
//
///////////////////////////////////////////////////////////////////////////////

int arr2D[3000][500];

int main() {

    for(int col = 0; col < 500; col++) {
        for(int row = 0; row < 3000; row++) {
            arr2D[row][col] = row + col;
        }
    }

    return 1;

}
