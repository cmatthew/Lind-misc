/* This code is in public domain. Use for whatever purpose at your own risk. */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/time.h> 
 
 
#define MAXN  1000000000  /* maximum value of N */
#define P1    15625000   /* = ceil(MAXN/64) */
#define P2    500000000   /* = ceil(MAXN/2) */
#define P3    15812       /* = ceil(ceil(sqrt(MAXN))/2) */
 
uint64_t sieve[P1];
 
#define GET(b) ((sieve[(b)>>5]>>((b)&31))&1)
 
void make()
{
    uint64_t i, j, k;
    memset(sieve, 0, sizeof(sieve));
    for (k = 1; k <= P3; k++)
        if (GET(k)==0) for(j=2*k+1,i=2*k*(k+1);i<P2;i+=j) sieve[i>>5]|=1<<(i&31);
}
 
int isprime(uint64_t p) { return p==2 || (p>2 && (p&1)==1 && (GET((p-1)>>1)==0)); }
 
int main()
{
    struct timeval start,stop;
  
    gettimeofday(&start,NULL);

    uint64_t i, n;
    make();
    for (n = 0, i = 0; i <= MAXN; i++)
        if (isprime(i)) n++;
    printf("The number of primes below 10^9 is %lld.\n", n);
  gettimeofday(&stop,NULL);
  printf(">> %ld.%06ld, %ld.%06ld\n",(long int)start.tv_sec, (long int)start.tv_usec, (long int)stop.tv_sec, (long int)stop.tv_usec);

    return 0;
}
