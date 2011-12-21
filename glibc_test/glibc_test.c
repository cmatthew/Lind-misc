/*
 * Copyright 2008 The Native Client Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can
 * be found in the LICENSE file.
 */

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/statfs.h>
#include <sys/types.h>
#include <sys/ioctl.h>

void exit_message(void) {
  puts("[output][Hello] Program done.\n");
  fflush(stdout);
}


void check_fstat(void) {
  /* code derived from http://codewiki.wikidot.com/c:system-calls:fstat */
  const char * f_name = "foo.txt";
  int file=0;
  if((file=open(f_name,O_RDONLY)) < -1)
    return;
 
  struct stat fileStat;
  if(fstat(file,&fileStat) < 0)    
    return;
 
  printf("Information for %s\n",f_name);
  printf("---------------------------\n");
  printf("File Size: \t\t%d bytes\n",(int)fileStat.st_size);
  printf("Number of Links: \t%d\n",(int)fileStat.st_nlink);
  printf("File inode: \t\t%d\n",(int)fileStat.st_ino);
 
  printf("File Permissions: \t");
  printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
  printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
  printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
  printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
  printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
  printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
  printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
  printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
  printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
  printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
  printf("\n\n");
 
  printf("The file %s a symbolic link\n\n", (S_ISLNK(fileStat.st_mode)) ? "is" : "is not");
 

}

void check_getpid(void) {
  int i;
  const int MAX_ITTER_CHECK = 10; /* Run a bunch of times, make sure the code is robust for many runs. */
  const int EXPECT_PID = 1000; 		/* 1000 is a hardcoded PID right now. */
  for (i = 0; i < MAX_ITTER_CHECK; i++) {
    int tmp = getpid();
    assert(tmp==EXPECT_PID);
  }
}

void check_file_ops(void) {
  printf("[output][glibc_test] About to open\n");
  fflush(stdout);
 
  
  FILE* f = fopen("foo.txt", "w");
  assert(f != NULL);
  
  /* check double opens work */
  f = fopen("foo.txt", "w");
  assert(f != NULL);

  char a[512] = "this is a test string.";
  memset(a, 0, sizeof(a));

  /* check we can do a write. */
  int size = fwrite(a, 1, 512, f);
  printf("[output][glibc_test]Fread read %d and got: %s\n", size, a);
  fflush(stdout);
  printf("[output][glibc_test] ferror = %d\n",ferror(f));

  /* check we can do a read. */
  size = fread(a, 1, 512, f);
  printf("[output][glibc_test]Fread read %d and got: %s\n", size, a);
  fflush(stdout);
  printf("[output][glibc_test] ferror = %d\n",ferror(f));


  /* check we can do a simple seek from the start of the file one forward */
  int seeked = fseek(f, 1, SEEK_SET);
  printf("[output][glibc_test] LSeek got %d \n", seeked);

  printf("[output][glibc_test] ferror = %d\n",ferror(f));
  assert(seeked==0);

  int ioctl_rc = ioctl(f->_fileno, 0);
  printf("[output][glibc_test] ioctl got %d \n", ioctl_rc);
  printf("[output][glibc_test] ferror = %d\n",ferror(f));
  assert(ioctl_rc==0);

  fclose(f);

}


void check_file_write(void) {
  printf("[output][glibc_test] About to write\n");
  fflush(stdout);

 

  FILE* f = fopen("foo2.txt", "w");
  assert(f != NULL);
  
  char * a = "This is a test string.";
  int size = fwrite(a, 1, strlen(a)+1, f);
  printf("[output][glibc_test] ferror = %d\n",ferror(f));
  

  printf("[output][glibc_test]Fwrite wrote %d\n", size);
  fflush(stdout);

  fclose(f);

}



void check_access(void) {
   printf("[output][glibc_test] Access\n");
   int access_test = access("foo2.txt", W_OK);
   printf("[output][glibc_test] Access returned %d\n",access_test );
   assert(access_test == 0);
   
}


void check_unlink(void) {
   printf("[output][glibc_test] Unlink\n");
   int unlink_test = unlink("foo2.txt");
   printf("[output][glibc_test] Unlink returned %d\n",unlink_test );
   assert(unlink_test == -1);
   
}

void check_link(void) {
   printf("[output][glibc_test] Link\n");
   int link_test = link("foo2.txt", "foo3.txt");
   printf("[output][glibc_test] Link returned %d\n",link_test );
   assert(link_test == -1);
   
}

void check_chdir(void) {
   printf("[output][glibc_test] Chdir\n");
   int chdir_test = chdir("/usr/local/");
   printf("[output][glibc_test] Chdir returned %d\n",chdir_test );
   assert(chdir_test == -1);
   
}

void check_mkdir(void) {
   printf("[output][glibc_test] Mkdir\n");
   int mkdir_test = mkdir("/usr/local/", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
   printf("[output][glibc_test] Mkdir returned %d\n",mkdir_test );
   assert(mkdir_test == -1);
   
}

void check_rmdir(void) {
   printf("[output][glibc_test] Rmdir\n");
   int rmdir_test = rmdir("/usr/local/");
   printf("[output][glibc_test] Rmdir returned %d\n",rmdir_test );
   perror("rmdir");
   assert(rmdir_test == -1);
   
}


void hello_world(void) {
  printf("unsigned long int %lu", sizeof(unsigned long int));
  printf("Hello Files\n");
  fflush(stdout);

  printf("Sizeof(struct statfs)=%lu\n", sizeof(struct statfs));
  
}

int main() {
  hello_world();
  check_access();
  /* check_getpid(); */
  check_file_ops();
  check_fstat();
  check_file_write();
  check_link();
  check_unlink();
  check_mkdir();
  check_chdir();
  check_rmdir();
  exit_message();
  return 0;
}
