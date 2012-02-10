#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "uds_helper.h"
#include "uds_client.h"


int UNIX_PATH_MAX = 100;

int cli_connect_buffer(char * buffer) {

	int s;
	memcpy(&s, &buffer[0], 4);
	printf("this worked like this: %d\n", s);
	message *andi;
	andi = (message *) buffer;
	struct sockaddr_un address;
  int socket_fd, nbytes;

  socket_fd = socket (PF_UNIX, SOCK_STREAM, 0);
  if (socket_fd < 0)
    {
      printf ("socket() failed\n");
      return 1;
    }

  
  memset (&address, 0, sizeof (struct sockaddr_un));

  address.sun_family = AF_UNIX;
  snprintf (address.sun_path, UNIX_PATH_MAX, "./../../network/output/demo_socket");

  if (connect (socket_fd,
	       (struct sockaddr *) &address,
	       sizeof (struct sockaddr_un)) != 0)
    {
      printf ("connect() failed\n");
      return 1;
    }
	 int t;
	 memcpy(&t, &buffer[0], 4);
	printf("message size: %d, %d, %d\n", &(andi->msg_size)[0], t, buffer);
	write (socket_fd, andi , MSG_SIZE);
  nbytes = read (socket_fd, andi, MSG_SIZE);
	printf("3333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333\n");
	int i;
	for (i = 0; i < 100; i++) {
		if (&andi[i] == '\0') {
			printf("X");
		}
		printf("%c ", &andi[i]);
	}
	printf ("<<\n");

  

	printf ("MESSAGE FROM ANDI: %d\n", &andi->msg_size);
	int tmp;
	memcpy(&tmp, &andi->data, 4);
  printf ("MESSAGE FROM SERVER: %d\n", andi->data);
  printf ("MESSAGE FROM SERVER: %d\n", tmp);
	memcpy(&tmp, &andi->num_of_args, 4);
	printf("MESSAGE FROM SERVER: %d\n", tmp);
	memcpy(&tmp, &andi->call_num, 4);
	printf("MESSAGE FROM SERVER: %d\n", tmp);
	memcpy(&tmp, &andi->data, 4);
	printf("MESSAGE FROM SERVER: %d\n", tmp);
close (socket_fd);

  return atoi(andi->call_num);

	
	printf("printing this here: %d\n", atoi(andi->msg_size));
	return 12345;
}


int cli_connect_int(x) {

  struct sockaddr_un address;
  int socket_fd, nbytes;
  char buffer[MSG_SIZE];
	memset(buffer, 'a', MSG_SIZE);

  socket_fd = socket (PF_UNIX, SOCK_STREAM, 0);
  if (socket_fd < 0)
    {
      printf ("socket() failed\n");
      return 1;
    }

  
  memset (&address, 0, sizeof (struct sockaddr_un));

  address.sun_family = AF_UNIX;
  snprintf (address.sun_path, UNIX_PATH_MAX, "./../../network/output/demo_socket");

  if (connect (socket_fd,
	       (struct sockaddr *) &address,
	       sizeof (struct sockaddr_un)) != 0)
    {
      printf ("connect() failed\n");
      return 1;
    }

  //nbytes = snprintf (buffer, MSG_SIZE, "hello from a client");
  //write (socket_fd, buffer, nbytes);
//	sprintf(buffer, "%d", x);
	sprintf(buffer, "%d", x);
	int j;
	for (j = 0; j <50; j++) {
		printf("%c", buffer[j]);
	}
	printf("\n");


	message *andi = (message *) buffer;
	sprintf(andi->num_of_args, "%d", 2);
	write (socket_fd, andi , MSG_SIZE);
  nbytes = read (socket_fd, andi, MSG_SIZE);
	printf("3333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333\n");
	int i;
	for (i = 0; i < 60; i++) {
		printf("%c ", buffer[i]);
	}
	printf ("\n");

  buffer[nbytes] = 0;

	printf ("MESSAGE FROM ANDI: %s\n", andi->call_num);
  printf ("MESSAGE FROM SERVER: %s\n", buffer);

  close (socket_fd);

  return atoi(andi->call_num);


}

/*int
main (void)
{
  struct sockaddr_un address;
  int socket_fd, nbytes;
  char buffer[256];

  socket_fd = socket (PF_UNIX, SOCK_STREAM, 0);
  if (socket_fd < 0)
    {
      printf ("socket() failed\n");
      return 1;
    }

  
  memset (&address, 0, sizeof (struct sockaddr_un));

  address.sun_family = AF_UNIX;
  snprintf (address.sun_path, UNIX_PATH_MAX, "./demo_socket");

  if (connect (socket_fd,
	       (struct sockaddr *) &address,
	       sizeof (struct sockaddr_un)) != 0)
    {
      printf ("connect() failed\n");
      return 1;
    }

  nbytes = snprintf (buffer, 256, "hello from a client");
  write (socket_fd, buffer, nbytes);

  nbytes = read (socket_fd, buffer, 256);
  buffer[nbytes] = 0;

  printf ("MESSAGE FROM SERVER: %s\n", buffer);

  close (socket_fd);

  return 0;
}*/
