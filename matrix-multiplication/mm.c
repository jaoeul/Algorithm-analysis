#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mm.h"

matrix_t*
matrix_create(void)
{
    return calloc(1, sizeof(matrix_t));
}

void
matrix_destroy(matrix_t* m)
{
    free(m);
}

void
matrix_print(const float m[ROWS][COLS])
{
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            printf("%.0f ", m[row][col]);
        }
        printf("\n");
    }
}

void
matrix_fill(float m[ROWS][COLS], float f)
{
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            m[row][col] = f;
        }
    }
}

void
mm(const float a[ROWS][COLS], const float b[ROWS][COLS],
   float c[ROWS][COLS])
{
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            for (int x = 0; x < ROWS; x++) {
                c[row][col] += a[row][col] * b[x][col];
            }
        }
    }
}

void
mm_block(const float a[ROWS][COLS], const float b[ROWS][COLS],
         float c[ROWS][COLS])
{
    int blk_sz = MAX_INNER_BLOCK_SIDE_LENGTH;

    // The outer matrix loop.
    for (int row = 0; row < ROWS; row += blk_sz) {
        for (int col = 0; col < COLS; col += blk_sz) {
            for (int x = 0; x < ROWS; x += blk_sz) {

                // The `inner block` loop.
                for (int blk_row = row; blk_row < (row + blk_sz); blk_row++) {
                    for (int blk_col = col; blk_col < (col + blk_sz); blk_col++) {
                        for (int blk_x = x; blk_x < (x + blk_sz); blk_x++) {
                            c[blk_row][blk_col] += a[blk_row][blk_col] * b[blk_x][blk_col];
                        }
                    }
                }
            }
        }
    }
}

void
mm_block_vec(const float a[ROWS][COLS], const float b[ROWS][COLS],
             float c[ROWS][COLS])
{
    // 256-bit registers, each capable of holding 8 32-bit floats.
    __m256 _a   = _mm256_setzero_ps();
    __m256 _b   = _mm256_setzero_ps();
    __m256 _c   = _mm256_setzero_ps();
    __m256 _res = _mm256_setzero_ps();

    // The side length of the inner block.
    int blk_sz  = MAX_INNER_BLOCK_SIDE_LENGTH;

    // The outer matrix multiplication loops, incremented with a `block`.
    for (int row = 0; row < ROWS; row += blk_sz) {
        for (int col = 0; col < COLS; col += blk_sz) {
            for (int x = 0; x < ROWS; x += blk_sz) {

                // The cache optimizing, inner block loops.
                for (int blk_row = row; blk_row < (row + blk_sz); blk_row++) {
                    for (int blk_col = col; blk_col < (col + blk_sz); blk_col++) {

                        // Vectorizing loop. `blk_x` is used to index the
                        // columns acted upon in the inner block.
                        for (int blk_x = x; blk_x < (x + blk_sz); blk_x += 8) {

                            // We step through the inner block of the matrix
                            // in a row-by-row fashion. We multiply and add  all
                            // the columns in the first row before moving on to
                            // the second row. After we finish all the rows in
                            // the first block, we move on to the same rows in
                            // the second block, and so on. Since we can fit 8
                            // floats in the 256-bit avx2 registers, we can
                            // multiply and add 8 floats, aka. 8 columns, each
                            // iteration.

                            // Unaligned load of the first factor vector.
                            _a = _mm256_loadu_ps(&a[blk_row][blk_x]);

                            // Unaligned load of the second factor vector.
                            _b = _mm256_loadu_ps(&b[blk_row][blk_x]);

                            // Get the values we will add the result of the
                            // multiplication to.
                            _c = _mm256_loadu_ps(&c[blk_row][blk_x]);

                            // Fused multiply and add. `_a` and `_b` are
                            // multiplied and added to `_c`.
                            _res = _mm256_fmadd_ps(_a, _b, _c);

                            // Store the floats into the output matrix.
                            _mm256_storeu_ps(&c[blk_row][blk_x], _res);
                        }
                    }
                }
            }
        }
    }
}
