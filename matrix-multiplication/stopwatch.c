#include <stdlib.h>

#include "stopwatch.h"

stopwatch_t*
stopwatch_create(void)
{
     return calloc(1, sizeof(stopwatch_t));
}

void
stopwatch_destroy(stopwatch_t* stopwatch)
{
    free(stopwatch);
}

void
stopwatch_start(stopwatch_t* stopwatch)
{
    clock_gettime(CLOCK_MONOTONIC, &stopwatch->ts);
    stopwatch->start  = stopwatch->ts.tv_sec;
    stopwatch->start += stopwatch->ts.tv_nsec / 1e9;
}

void
stopwatch_stop(stopwatch_t* stopwatch)
{
    clock_gettime(CLOCK_MONOTONIC, &stopwatch->ts);
    stopwatch->end       = stopwatch->ts.tv_sec;
    stopwatch->end      += stopwatch->ts.tv_nsec / 1e9;
    stopwatch->elapsed_s = stopwatch->end - stopwatch->start;
}

void
stopwatch_reset(stopwatch_t* stopwatch)
{
    clock_gettime(CLOCK_MONOTONIC, &stopwatch->ts);
    stopwatch->start  = stopwatch->ts.tv_sec;
    stopwatch->start += stopwatch->ts.tv_nsec / 1e9;
}

double
stopwatch_elapsed(stopwatch_t* stopwatch)
{
    return stopwatch->elapsed_s;
}

