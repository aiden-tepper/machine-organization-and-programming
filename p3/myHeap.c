////////////////////////////////////////////////////////////////////////////////
// Main File:        myHeap.c
// This File:        myHeap.c
// Other Files:      myHeap.h
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

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include "myHeap.h"

/*
* This structure serves as the header for each allocated and free block.
* It also serves as the footer for each free block but only containing size.
*/
typedef struct blockHeader {
    int size_status;
    /*
    * Size of the block is always a multiple of 8.
    * Size is stored in all block headers and free block footers.
    *
    * Status is stored only in headers using the two least significant bits.
    *   Bit0 => least significant bit, last bit
    *   Bit0 == 0 => free block
    *   Bit0 == 1 => allocated block
    *
    *   Bit1 => second last bit
    *   Bit1 == 0 => previous block is free
    *   Bit1 == 1 => previous block is allocated
    *
    * End Mark:
    *  The end of the available memory is indicated using a size_status of 1.
    *
    * Examples:
    *
    * 1. Allocated block of size 24 bytes:
    *    Header:
    *      If the previous block is allocated, size_status should be 27
    *      If the previous block is free, size_status should be 25
    *
    * 2. Free block of size 24 bytes:
    *    Header:
    *      If the previous block is allocated, size_status should be 26
    *      If the previous block is free, size_status should be 24
    *    Footer:
    *      size_status should be 24
    */
} blockHeader;

/* Global variable - DO NOT CHANGE. It should always point to the first block,
* i.e., the block at the lowest address.
*/
blockHeader *heapStart = NULL;

/* Size of heap allocation padded to round to nearest page size.
*/
int allocsize;

/*
* Additional global variables may be added as needed below
*/
blockHeader *currentBlock = NULL;

/*
* Function for allocating 'size' bytes of heap memory.
* Argument size: requested size for the payload
* Returns address of allocated block on success.
* Returns NULL on failure.
* This function should:
* - Check size - Return NULL if not positive or if larger than heap space.
* - Determine block size rounding up to a multiple of 8 and possibly adding padding as a result.
* - Use NEXT-FIT PLACEMENT POLICY to chose a free block
* - Use SPLITTING to divide the chosen free block into two if it is too large.
* - Update header(s) and footer as needed.
* Tips: Be careful with pointer arithmetic and scale factors.
*/
void* myAlloc(int size) {

    if(size <= 0 || size > allocsize) {
        return NULL;
    }

    int padding = 0;

    if((size + sizeof(blockHeader)) % 8 != 0) {
        padding = 8 - ((size + sizeof(blockHeader)) % 8);
    }

    int newBlockSize = size + sizeof(blockHeader) + padding;

    // if this is the first time calling myAlloc and currentBlock hasn't been set yet
    if(currentBlock == NULL) {
        currentBlock = (void*)heapStart + sizeof(blockHeader); // start next fit placement policy from start
    }

    // local variables to simplify code
    int oldFreeBlockHeader = ((blockHeader*)((void*)currentBlock - sizeof(blockHeader)))->size_status;
    int oldFreeBlockSize = (oldFreeBlockHeader / 8) * 8;
    int counter = 0;

    while(1) { // loop to move find next freeblock
        if(((blockHeader*)((void*)currentBlock - sizeof(blockHeader)))->size_status == 1) { // if at end mark
            currentBlock = (void*)heapStart + sizeof(blockHeader); // loop to begnning
        }
        if(oldFreeBlockHeader % 8 == 0 || oldFreeBlockHeader % 8 == 2) { // if free block
            if(newBlockSize <= oldFreeBlockSize) { // if size is big enough
                break; // move on and allocate it
            }
        }
        currentBlock = (blockHeader*)((void*)currentBlock + oldFreeBlockSize); // otherwise, jump to next block and try again
        counter += oldFreeBlockSize; // add jump to counter
        oldFreeBlockHeader = ((blockHeader*)((void*)currentBlock - sizeof(blockHeader)))->size_status; // update variable
        oldFreeBlockSize = (oldFreeBlockHeader / 8) * 8; // update variable
        if(counter == allocsize) { // if we've already searched through every block
        return NULL; // alloc fails
    }
}

((blockHeader*)((void*)currentBlock - sizeof(blockHeader)))->size_status = newBlockSize + 3; // set new block header

if(!(newBlockSize == oldFreeBlockSize)) { // check if we need to split
    blockHeader *header = (blockHeader*) ((void*)currentBlock - sizeof(blockHeader) + newBlockSize); // create new free block header
    header->size_status = oldFreeBlockHeader - newBlockSize; // set header size_status
    if(((blockHeader*)((void*)currentBlock - sizeof(blockHeader) + oldFreeBlockSize - sizeof(blockHeader)*3))->size_status == 1) { // if next block is end mark
        ((blockHeader*)((void*)currentBlock - sizeof(blockHeader) + oldFreeBlockSize - sizeof(blockHeader)*4))->size_status = oldFreeBlockSize - newBlockSize; // update free block footer accordingly
    } else {
        ((blockHeader*)((void*)currentBlock - sizeof(blockHeader) + oldFreeBlockSize - sizeof(blockHeader)*3))->size_status = oldFreeBlockSize - newBlockSize; // update free block footer accordingly
    }
}

return currentBlock;

}

/*
* Function for freeing up a previously allocated block.
* Argument ptr: address of the block to be freed up.
* Returns 0 on success.
* Returns -1 on failure.
* This function should:
* - Return -1 if ptr is NULL.
* - Return -1 if ptr is not a multiple of 8.
* - Return -1 if ptr is outside of the heap space.
* - Return -1 if ptr block is already freed.
* - USE IMMEDIATE COALESCING if one or both of the adjacent neighbors are free.
* - Update header(s) and footer as needed.
*/
int myFree(void *ptr) {

    if(ptr == NULL) { // check if ptr is NULL
        return -1;
    }

    if(((int)ptr) % 8 != 0) { // check if ptr is multiple of 8
        return -1;
    }

    if(((int)ptr) < (int)heapStart || ((int)ptr) > (int)heapStart + allocsize) { // check if ptr is outside of the heap space
        return -1;
    }

    if(((blockHeader*)((void*)ptr - sizeof(blockHeader))) -> size_status % 2 == 0) { // check if ptr is already freed
        return -1;
    }

    // local variables to simplify code
    int counter = 0;
    int currentBlockHeader = ((blockHeader*)((void*)ptr - sizeof(blockHeader)))->size_status;
    int currentBlockSize = (currentBlockHeader / 8) * 8;
    blockHeader *nextBlock = (blockHeader*)((void*)ptr - sizeof(blockHeader) + currentBlockSize);
    int nextBlockHeader = ((blockHeader*)((void*)nextBlock))->size_status;
    int nextBlockSize = (nextBlockHeader / 8) * 8;

    // coalescing with next block
    if(nextBlockHeader % 8 == 0 || nextBlockHeader % 8 == 2) { // if next block is free
        ((blockHeader*)((void*)ptr - sizeof(blockHeader)))->size_status -= 1; // subtract 1 from header
        ((blockHeader*)((void*)ptr - sizeof(blockHeader)))->size_status += nextBlockSize; // update size of header
        ((blockHeader*)((void*)ptr - sizeof(blockHeader)*2 + currentBlockSize + nextBlockSize))->size_status += currentBlockSize; // update footer
        counter++;
    }

    // update local variables
    currentBlockHeader = ((blockHeader*)((void*)ptr - sizeof(blockHeader)))->size_status;
    currentBlockSize = (currentBlockHeader / 8) * 8;

    // coalescing with previous block
    if(currentBlockHeader % 8 == 0 || currentBlockHeader % 8 == 1) { // if previous block is free
        int prevBlockSize = ((blockHeader*)((void*)ptr - sizeof(blockHeader)*2))->size_status;
        ((blockHeader*)((void*)ptr - sizeof(blockHeader) - prevBlockSize))->size_status += currentBlockSize; // update previous block header
        blockHeader *footer = (blockHeader*)((void*)ptr - sizeof(blockHeader)*2 + currentBlockSize); // create footer
        footer->size_status = currentBlockSize + prevBlockSize; // set footer size_status
        if(((blockHeader*)((void*)ptr - sizeof(blockHeader) + currentBlockSize))->size_status != 1) { // if next block isn't end mark
        ((blockHeader*)((void*)ptr - sizeof(blockHeader) + currentBlockSize))->size_status -= 2; // update p-bit of next block
    }
    currentBlock = (blockHeader*)((void*)ptr - prevBlockSize);
    counter++;
}

// if no coalescing needed
if(counter == 0) {
    ((blockHeader*)((void*)ptr - sizeof(blockHeader)))->size_status -= 1; // update block header
    blockHeader *footer = (blockHeader*)((void*)ptr - sizeof(blockHeader)*2 + currentBlockSize); // create footer
    footer->size_status = currentBlockSize; // set footer size_status
    if(((blockHeader*)((void*)ptr - sizeof(blockHeader) + currentBlockSize))->size_status != 1) { // if next block isn't end mark
    ((blockHeader*)((void*)ptr - sizeof(blockHeader) + currentBlockSize))->size_status -= 2; // update p-bit of next block
}
}

return 0;

}

/*
* Function used to initialize the memory allocator.
* Intended to be called ONLY once by a program.
* Argument sizeOfRegion: the size of the heap space to be allocated.
* Returns 0 on success.
* Returns -1 on failure.
*/
int myInit(int sizeOfRegion) {

    static int allocated_once = 0; //prevent multiple myInit calls

    int pagesize;  // page size
    int padsize;   // size of padding when heap size not a multiple of page size
    void* mmap_ptr; // pointer to memory mapped area
    int fd;

    blockHeader* endMark;

    if (0 != allocated_once) {
        fprintf(stderr,
            "Error:mem.c: InitHeap has allocated space during a previous call\n");
            return -1;
        }
        if (sizeOfRegion <= 0) {
            fprintf(stderr, "Error:mem.c: Requested block size is not positive\n");
            return -1;
        }

        // Get the pagesize
        pagesize = getpagesize();

        // Calculate padsize as the padding required to round up sizeOfRegion
        // to a multiple of pagesize
        padsize = sizeOfRegion % pagesize;
        padsize = (pagesize - padsize) % pagesize;

        allocsize = sizeOfRegion + padsize;

        // Using mmap to allocate memory
        fd = open("/dev/zero", O_RDWR);
        if (-1 == fd) {
            fprintf(stderr, "Error:mem.c: Cannot open /dev/zero\n");
            return -1;
        }
        mmap_ptr = mmap(NULL, allocsize, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
        if (MAP_FAILED == mmap_ptr) {
            fprintf(stderr, "Error:mem.c: mmap cannot allocate space\n");
            allocated_once = 0;
            return -1;
        }

        allocated_once = 1;

        // for double word alignment and end mark
        allocsize -= 8;

        // Initially there is only one big free block in the heap.
        // Skip first 4 bytes for double word alignment requirement.
        heapStart = (blockHeader*) mmap_ptr + 1;

        // Set the end mark
        endMark = (blockHeader*)((void*)heapStart + allocsize);
        endMark->size_status = 1;

        // Set size in header
        heapStart->size_status = allocsize;

        // Set p-bit as allocated in header
        // note a-bit left at 0 for free
        heapStart->size_status += 2;

        // Set the footer
        blockHeader *footer = (blockHeader*) ((void*)heapStart + allocsize - 4);
        footer->size_status = allocsize;

        return 0;
    }

    /*
    * Function to be used for DEBUGGING to help you visualize your heap structure.
    * Prints out a list of all the blocks including this information:
    * No.      : serial number of the block
    * Status   : free/used (allocated)
    * Prev     : status of previous block free/used (allocated)
    * t_Begin  : address of the first byte in the block (where the header starts)
    * t_End    : address of the last byte in the block
    * t_Size   : size of the block as stored in the block header
    */
    void dispMem() {

        int counter;
        char status[5];
        char p_status[5];
        char *t_begin = NULL;
        char *t_end   = NULL;
        int t_size;

        blockHeader *current = heapStart;
        counter = 1;

        int used_size = 0;
        int free_size = 0;
        int is_used   = -1;

        fprintf(stdout, "************************************Block list***\
        ********************************\n");
        fprintf(stdout, "No.\tStatus\tPrev\tt_Begin\t\tt_End\t\tt_Size\n");
        fprintf(stdout, "-------------------------------------------------\
        --------------------------------\n");

        while (current->size_status != 1) {
            t_begin = (char*)current;
            t_size = current->size_status;

            if (t_size & 1) {
                // LSB = 1 => used block
                strcpy(status, "used");
                is_used = 1;
                t_size = t_size - 1;
            } else {
                strcpy(status, "Free");
                is_used = 0;
            }

            if (t_size & 2) {
                strcpy(p_status, "used");
                t_size = t_size - 2;
            } else {
                strcpy(p_status, "Free");
            }

            if (is_used)
            used_size += t_size;
            else
            free_size += t_size;

            t_end = t_begin + t_size - 1;

            fprintf(stdout, "%d\t%s\t%s\t0x%08lx\t0x%08lx\t%d\n", counter, status,
            p_status, (unsigned long int)t_begin, (unsigned long int)t_end, t_size);

            current = (blockHeader*)((char*)current + t_size);
            counter = counter + 1;
        }

        fprintf(stdout, "---------------------------------------------------\
        ------------------------------\n");
        fprintf(stdout, "***************************************************\
        ******************************\n");
        fprintf(stdout, "Total used size = %d\n", used_size);
        fprintf(stdout, "Total free size = %d\n", free_size);
        fprintf(stdout, "Total size = %d\n", used_size + free_size);
        fprintf(stdout, "***************************************************\
        ******************************\n");
        fflush(stdout);

        return;
    }


    // end of myHeap.c (fall 2020)
