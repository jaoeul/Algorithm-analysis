#include <stdint.h>

#include "mm.h"
#include "stopwatch.h"

int
main(void)
{
    if (MATRIX_SIDE_LENGTH % MAX_INNER_BLOCK_SIDE_LENGTH != 0) {
        printf("Error! The matrices side lengths must be divisable by the side "
                "length of the inner block.\n");
        abort();
    }

    if (ROWS != COLS) {
        printf("Error! Matrix is not a square!\n");
        abort();
    }

    __builtin_cpu_init();
    if (!__builtin_cpu_supports("avx2")) {
        printf("Error! This program uses the avx2 extension, which is not "
                "supported by your processor!\n");
        abort();
    }

    // Matrix full of 1's.
    matrix_t* a = matrix_create();
    matrix_fill(a->data, (float)1);

    // Matrix full of 2's.
    matrix_t* b = matrix_create();
    matrix_fill(b->data, (float)2);

    // Result matrices.
    matrix_t* c = matrix_create();
    matrix_t* d = matrix_create();
    matrix_t* e = matrix_create();

    printf("~* Running some matrix multiplications! *~\n");
    printf("Matrix size: %d rows, %d cols\n", ROWS, COLS);

    stopwatch_t* stopwatch = stopwatch_create();

    // Naive vanilla.
    stopwatch_start(stopwatch);
    mm(a->data, b->data, c->data);
    stopwatch_stop(stopwatch);
    printf("Navie: %lf s\n", stopwatch_elapsed(stopwatch));

    // Cache blocking.
    stopwatch_reset(stopwatch);
    mm_block(a->data, b->data, d->data);
    stopwatch_stop(stopwatch);
    printf("Blocking: %lf s\n", stopwatch_elapsed(stopwatch));

    // Cache blocking and vectorizing.
    stopwatch_reset(stopwatch);
    mm_block_vec(a->data, b->data, e->data);
    stopwatch_stop(stopwatch);
    printf("Blocking and vectorizing %lf s\n", stopwatch_elapsed(stopwatch));

    //matrix_print(c->data);

    // Clean up.
    stopwatch_destroy(stopwatch);
    matrix_destroy(a);
    matrix_destroy(b);
    matrix_destroy(c);
    matrix_destroy(d);
    matrix_destroy(e);

    return 0;
}
