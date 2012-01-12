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
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/statfs.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <dirent.h> 

#define FILE_NAME "3f23f9a0c6771d4e79fe0ea7"
#define FILE_NAME2 "SbdcO1HjzjFxn7qhtr"


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
 
  
  FILE* f = fopen(FILE_NAME, "w");
  assert(f != NULL);
  
  char a[512] = "this is a test string. zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz";
  char b[512];
 
  memset(b, 0, sizeof(b));

  int size = fwrite(a, 1, strlen(a)+1, f);
  int rc = fclose(f);
  assert(rc == 0);
  f = NULL;

  /* check we can do a read. */
  f = fopen(FILE_NAME, "r");
  assert(f != NULL);
  size = fread(b, 1, 512, f);
  int cmp = strcmp(a,b);
  assert(cmp==0);
 
  fclose(f);
  f = NULL;
  f = fopen(FILE_NAME, "w");
  assert(f != NULL);

  /* check we can do a simple seek from the start of the file one forward */
  int seeked = fseek(f, 1, SEEK_SET);
  printf("[output][glibc_test] LSeek got %d \n", seeked);

  printf("[output][glibc_test] ferror = %d\n",ferror(f));
  assert(seeked==0);


  fclose(f);

}

void check_ioctl(void) {

  FILE* f = fopen(FILE_NAME, "w");
  assert(f != NULL);

  int ioctl_rc = ioctl(fileno(f), 0);
  printf("[output][glibc_test] ioctl got %d \n", ioctl_rc);
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

   int access_test = access("afilethatdoesnotexist.txt", W_OK);
   printf("[output][glibc_test] Access 1 returned %d\n",access_test );
   assert(access_test == -1);

   access_test = access("somevvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvverylongname.txt", R_OK);
   printf("[output][glibc_test] Access 2 returned %d\n",access_test );
   assert(access_test == -1);

   access_test = access(FILE_NAME, R_OK);
   printf("[output][glibc_test] Access 2 returned %d\n",access_test );
   assert(access_test == 0);
   
}

#define LINK_FILENAME "foo3.txt"
#define LINK_DEST_1 "foo4.txt"
#define LINK_DEST_2 "foo5.txt"


void check_link(void) {
   printf("[output][glibc_test] Link\n");
   int link_test;
   link_test = link(FILE_NAME, LINK_FILENAME);
   assert(link_test == 0);
   link_test = link(FILE_NAME, "foo4.txt");
   assert(link_test == 0);
   link_test = link(LINK_FILENAME, "foo5.txt");
   assert(link_test == 0);
   
}


void check_unlink(void) {
   printf("[output][glibc_test] Unlink\n");
   int unlink_test;
   unlink_test = unlink(LINK_DEST_2);
   assert(unlink_test == 0);
   unlink_test = unlink(LINK_DEST_1);
   assert(unlink_test == 0);
   unlink_test = unlink(LINK_FILENAME);
   assert(unlink_test == 0);
   
}


void check_chdir(void) {
   printf("[output][glibc_test] Chdir\n");
   int chdir_test;
   /* Simple check */
   chdir_test = chdir("/usr/local/");
   assert(chdir_test == 0);
   chdir_test = chdir("/");
   assert(chdir_test == 0);
   chdir_test = chdir("/foo");
   assert(chdir_test == 0);
   /* Check you can go back to the same place twice */
   chdir_test = chdir("/usr/local");
   assert(chdir_test == 0);
   chdir_test = chdir("/usr/local");
   assert(chdir_test == 0);
   /* Test null string */
   /* chdir_test = chdir(""); */
   /* assert(chdir_test == -1); */
   
}

void check_mkdir(void) {
   printf("[output][glibc_test] Mkdir\n");
   int mkdir_test = mkdir("/usr/", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
   assert(mkdir_test == 0);
   mkdir_test = mkdir("/usr/local", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
   assert(mkdir_test == 0);
   mkdir_test = mkdir("/foo", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
   assert(mkdir_test == 0);
   mkdir_test = mkdir("/bar", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
   assert(mkdir_test == 0);
   mkdir_test = mkdir("/foobar", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
   assert(mkdir_test == 0);
   mkdir_test = mkdir("/foobar/bar", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
   assert(mkdir_test == 0);

}

void check_rmdir(void) {
   printf("[output][glibc_test] Rmdir\n");
   int rmdir_test;
   rmdir_test = rmdir("/usr/local/");
   assert(rmdir_test == 0);
   rmdir_test = rmdir("/usr/");
   assert(rmdir_test == 0);
   rmdir_test = rmdir("/");
   assert(rmdir_test == -1);
}


void hello_world(void) {
  /* printf("unsigned long int %u", sizeof(unsigned long int)); */
  /* printf("Hello Files\n"); */
  /* fflush(stdout); */

  /* printf("Sizeof(struct statfs)=%u\n", sizeof(struct statfs)); */
  /* printf("O_CREAT=%o\nO_ASYNC=%o\nO_WRONLY=%o\n",O_CREAT, O_ASYNC, O_WRONLY); */
}

#define DOUBLE_OPEN_FILE_NAME "double_open_file"

void check_double_open(void) {
  printf("Doing Double Open Test:\n");
  char * test = "abcd1234\n";
  char * test2 = "foo";
   struct stat fileStat;
   int f = open(DOUBLE_OPEN_FILE_NAME, O_WRONLY|O_CREAT|O_TRUNC, 0666);
   if(fstat(f, &fileStat) < 0)    
    return;
 
  printf("Information for %s\n",DOUBLE_OPEN_FILE_NAME);
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


  int rc = write(f, test, strlen(test)+1);
  printf("RC=%d\n",rc);
  /* check double opens work */
  int f2 = open(DOUBLE_OPEN_FILE_NAME, O_WRONLY|O_CREAT|O_TRUNC, 0666);

  /* rc = write(f2, test, strlen(test)+1); */
  printf("RC=%d\n",rc);

 
  rc = write(f, test2, strlen(test2)+1);
  printf("RC=%d\n",rc);


  if(fstat(f, &fileStat) < 0)    
    return;
 
  printf("Information for %s\n",DOUBLE_OPEN_FILE_NAME);
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

  if(fstat(f2, &fileStat) < 0)    
    return;
 
  printf("Information for %s\n",DOUBLE_OPEN_FILE_NAME);
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





  close(f2);
  close(f);

  printf("Done Double Open Test.\n");


}

void check_readdir(void) {
  printf("Doing chdir\n");
  assert (chdir("/") == 0);
  printf("Doing Opendir\n");
  DIR* dirp = opendir(".");
  assert(dirp != NULL);
  struct dirent * dp = NULL;
  printf("Doing Readdir\n");

  while ((dp = readdir(dirp)) != NULL ) {
    printf("One ent:\n");
    printf("%s\n", dp->d_name);
    
  }
  printf("Closedir\n");
  closedir(dirp);
}

void check_statfs(void) {
    FILE* f = fopen(".", "r");
    struct statfs buf;
    int rc = fstatfs(fileno(f), &buf);

    printf("fstatfs: type: %x, bsize %d, blocks %ld, bfree %ld rc=%d\n", buf.f_type, buf.f_bsize,
	   (long int)buf.f_blocks, (long int)buf.f_bfree, rc);
    assert(rc==0);

    rc = statfs(".", &buf);

    printf("statfs: type: %x, bsize %d, blocks %ld, bfree %ld rc=%d\n", buf.f_type, buf.f_bsize,
	   (long int)buf.f_blocks, (long int)buf.f_bfree, rc);
    assert(rc==0);
    
}


void check_dups(void) {
  dup(12);
  dup2(15, 16);
  

}



void check_creat(void) {
  int rc = creat(FILE_NAME2, O_WRONLY);
  assert(rc != 0);
  close(rc);
  

}




int main() {

  /* hello_world(); */
  /* check_double_open(); */

  /* check_file_ops(); */
  check_statfs();
  check_dups();
  check_creat();
  /* check_readdir(); */

  /* exit(0); */
  /* check_access(); */
  /* check_getpid();  */
  /* check_fstat(); */
  /* check_file_write(); */
  /* check_mkdir(); */
  /* check_link(); */
  /* check_unlink(); */

  /* check_chdir(); */
  /* check_rmdir(); */

  /* check_ioctl(); */
  /* exit_message(); */

  return 0;

}
