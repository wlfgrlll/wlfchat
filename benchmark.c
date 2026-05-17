#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define TEXT_BUFSIZ 64
#define HISTORY_BUFSIZ 256

__attribute__((noinline)) void manual_copy(uint32_t *historyBuf, uint32_t *buf, int historyBufCount) {
    for (int i = 0; i < TEXT_BUFSIZ; i++) historyBuf[historyBufCount * TEXT_BUFSIZ + i] = buf[i];
}

__attribute__((noinline)) void memcpy_copy(uint32_t *historyBuf, uint32_t *buf, int historyBufCount) {
    memcpy(&historyBuf[historyBufCount * TEXT_BUFSIZ], buf, sizeof(uint32_t) * TEXT_BUFSIZ);
}

int main() {
    uint32_t buf[TEXT_BUFSIZ];
    uint32_t* historyBuf = malloc(sizeof(uint32_t) * TEXT_BUFSIZ * HISTORY_BUFSIZ);
    for (int i=0; i<TEXT_BUFSIZ; i++) buf[i] = i;

    int ITERS = 10000000;

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int iter=0; iter<ITERS; iter++) {
        manual_copy(historyBuf, buf, iter % HISTORY_BUFSIZ);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    double time_loop = end.tv_sec - start.tv_sec + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Loop: %f s\n", time_loop);

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int iter=0; iter<ITERS; iter++) {
        memcpy_copy(historyBuf, buf, iter % HISTORY_BUFSIZ);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    double time_memcpy = end.tv_sec - start.tv_sec + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Memcpy: %f s\n", time_memcpy);

    printf("Improvement: %.2fx\n", time_loop / time_memcpy);
    free(historyBuf);
    return 0;
}
