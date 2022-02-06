// My attempt at implementing fast matrix multiplication.
//
// How to find the optimal size of inner block:
//
// Laptop L1 cache size: 32KiB
//
// We are working on three matrices. Two which we are multiplying and one which
// we store the result in. Biggest possible size of a square matrices which we
// can fit three of in L1 cache:
//
// 10922 = floor(32KiB / 3)
//
// Now we need to find the side length of a block of this size. As we are only
// using square matrices, we can achieve this by finding the square root of
// the block size. This is the largest side length of the inner block we can
// use, without ever having to step out of L1 cache.
//
// 104 = floor(sqrt(10922))
//
// We will use this value to step through the matrices in both X and Y
// direction. Because of this, the side length of the matrix block needs to be
// divisable by the side length of the main matrices.
//
// In this program, a side length of 1024 is used for the matrices. The biggest
// divisor of 1024, which is below our maximum inner block length (104) is 64.

#ifndef MM_H
#define MM_H

#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MATRIX_SIDE_LENGTH          1024
#define ROWS                        MATRIX_SIDE_LENGTH
#define COLS                        ROWS
#define REG_SIZE                    64
#define MAX_INNER_BLOCK_SIDE_LENGTH 64

typedef struct {
    float data[ROWS][COLS];
} matrix_t;

matrix_t*
matrix_create(void);

void
matrix_destroy(matrix_t* m);

void
matrix_print(const float m[ROWS][COLS]);

void
matrix_fill(float m[ROWS][COLS], float f);

// Naive implementation.
void
mm(const float a[ROWS][COLS], const float b[ROWS][COLS],
   float c[ROWS][COLS]);

// Cache efficient implementation, breaking multiplication into inner `blocks`.
// The inner block side length has to be smaller than the side of the matrices,
// since a too big block size will be used to increment the indecies to outside
// of the main matrices.
void
mm_block(const float a[ROWS][COLS], const float b[ROWS][COLS],
         float c[ROWS][COLS]);

// Cache efficient implementation combined with vectiorization of the innermost
// loop. This is the fastest one so far.
void
mm_block_vec(const float a[ROWS][COLS], const float b[ROWS][COLS],
             float c[ROWS][COLS]);

#endif
