#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    struct timeval start;
    struct timeval stop;
    gettimeofday(&start, NULL);
    sleep(5); // call algorithm. here: sleep for 5 sec
    gettimeofday(&stop, NULL);
    double elapsed_ms = (stop.tv_sec - start.tv_sec) * 1000.0;
    elapsed_ms += (stop.tv_usec - start.tv_usec) / 1000.0;
    printf("elapsed time = %.2f ms\n", elapsed_ms);
}