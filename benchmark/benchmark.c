#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

int
timeval_subtract (result, x, y)
     struct timeval *result, *x, *y;
{
  /* Perform the carry for the later subtraction by updating y. */
  if (x->tv_usec < y->tv_usec) {
    int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
    y->tv_usec -= 1000000 * nsec;
    y->tv_sec += nsec;
  }
  if (x->tv_usec - y->tv_usec > 1000000) {
    int nsec = (x->tv_usec - y->tv_usec) / 1000000;
    y->tv_usec += 1000000 * nsec;
    y->tv_sec -= nsec;
  }

  /* Compute the time remaining to wait.
     tv_usec is certainly positive. */
  result->tv_sec = x->tv_sec - y->tv_sec;
  result->tv_usec = x->tv_usec - y->tv_usec;

  /* Return 1 if result is negative. */
  return x->tv_sec < y->tv_sec;
}

#define ITTER 500

int main() {
  extern int __lind_noop(void);
  struct timeval start[ITTER],stop[ITTER];
  int i;
  for (i = 0; i < ITTER; i++) {
    gettimeofday(&start[i],NULL);
    getpid();
    gettimeofday(&stop[i],NULL);
  }
  printf("\n");
  for (i = 0; i < ITTER; i++) {
    printf(">> %ld.%06ld, %ld.%06ld\n",(long int)start[i].tv_sec, (long int)start[i].tv_usec, (long int)stop[i].tv_sec, (long int)stop[i].tv_usec);
  }


}
