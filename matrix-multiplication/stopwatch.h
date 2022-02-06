#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <time.h>

typedef struct {
    struct timespec ts;
    double start;
    double end;
    double elapsed_s;
} stopwatch_t;

stopwatch_t*
stopwatch_create(void);

void
stopwatch_destroy(stopwatch_t* stopwatch);

void
stopwatch_start(stopwatch_t* stopwatch);

void
stopwatch_stop(stopwatch_t* stopwatch);

void
stopwatch_reset(stopwatch_t* stopwatch);

double
stopwatch_elapsed(stopwatch_t* stopwatch);

#endif
