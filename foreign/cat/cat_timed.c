/* Taken from the K&R C book */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

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


/* cat:  concatenate files, version 2 */
int main(int argc, char *argv[])
{
  struct timeval start,stop, result;
  gettimeofday(&start,NULL);
  FILE *fp;
  void filecopy(FILE *, FILE *);
  char *prog = argv[0];  /* program name for errors */
  
  if (argc == 1 ) {/* no args; copy standard input */
    filecopy(stdin, stdout);
  } else {
    while (--argc > 0)
      if ((fp = fopen(*++argv, "r")) == NULL) {
	fprintf(stderr, "%s: can't open %s\n",
		prog, *argv);
	goto exit;
      } else {
	filecopy(fp, stdout); 
	fclose(fp);
      }
  }
  if (ferror(stdout)) {
    fprintf(stderr, "%s: error writing stdout\n", prog);
    goto exit;
  }
 exit:
  gettimeofday(&stop,NULL);
  timeval_subtract(&result, &stop, &start);
  printf("%d.%06d\n",(unsigned int)result.tv_sec, (unsigned int)result.tv_usec);
  exit(0);
  return 0;
}

/* filecopy:  copy file ifp to file ofp */
void filecopy(FILE *ifp, FILE *ofp)  
{
  int c;
  
  while ((c = getc(ifp)) != EOF) {
    putc(c, ofp);
  }
}
