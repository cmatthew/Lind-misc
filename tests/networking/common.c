#include <stdio.h>                /* perror() */
#include <stdlib.h>               /* atoi() */
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>               /* read() */
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <assert.h>
#include <errno.h>

#define perror(x) fprintf(stderr, x);
#define debug(x) printf(x);fflush(stdout);

/* The name lookup subsystem needs files to direct it on how to do DNS and hostnames.
This function writes some fake files into root, which then program can then read.

*/
void make_files(void) {

  const char * dns = "nameserver 142.104.71.64\nnameserver 142.104.96.2\nnameserver 142.104.6.1\ndomain cs.uvic.ca\nsearch cs.uvic.c";
  mkdir("/etc/", 0);

  FILE* f = fopen("/etc/resolv.conf", "w");
  assert(f != NULL);
  fwrite(dns, strlen(dns), 1, f);
  fclose(f);

  const char * nss = "# /etc/nsswitch.conf\n passwd:         compat\n group:          compat\n shadow:         compat\n hosts:          files\n networks:       files\n protocols:      db files\n services:       db files\n ethers:         db files\n rpc:            db files\n netgroup:       nis\n";
  f = fopen("/etc/nsswitch.conf", "w");
  assert(f != NULL);
  fwrite(nss, strlen(nss), 1, f);
  fclose(f);


  const char * hosts = "127.0.0.1\tlocalhost\n";
  f = fopen("/etc/hosts", "w");
  assert(f != NULL);
  fwrite(hosts, strlen(hosts), 1, f);
  fclose(f);



}
