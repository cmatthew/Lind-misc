#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define ITTER 1000

int main() {
  //extern int __lind_noop(void);
  struct timeval start[ITTER],stop[ITTER];
  int i;
  for (i = 0; i < ITTER; i++) {
    gettimeofday(&start[i],NULL);
    
    int fd = open("input_files/1mb", O_RDWR);
        
    close(fd);
    
    gettimeofday(&stop[i],NULL);
  }

  printf("\n");
  for (i = 0; i < ITTER; i++) {
    printf(">> %ld.%06ld, %ld.%06ld\n",(long int)start[i].tv_sec, (long int)start[i].tv_usec, (long int)stop[i].tv_sec, (long int)stop[i].tv_usec);
  }


}
