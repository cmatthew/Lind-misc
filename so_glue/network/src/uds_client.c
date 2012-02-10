#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "uds_helper.h"
#include "uds_client.h"


int UNIX_PATH_MAX = 100;

/* int cli_connect_buffer(char * buffer)
 * Receives a buffer of size 4096 and forwards it to the server
 * Arguments:
 *	buffer	char * containing a serialized version of the actual function call
 * Result:
 *	returns an int currently, should change this to return void * so that the
 *		caller can just return it without problem. The return value should be 
 *		specified somewhere in the buffer probably.
 */
int cli_connect_buffer (char *buffer) {
	/* debugging things for myself to see if buffer was filled correctly */
	int s;
	memcpy (&s, &buffer[0], 4);
	printf ("this worked like this: %d\n", s);
	
	/* overlaying struct message over buffer */
	message *andi;
	andi = (message *) buffer;

	/* preparing unix domain socket (uds) */
	struct sockaddr_un address;
	int socket_fd, nbytes;

	socket_fd = socket (PF_UNIX, SOCK_STREAM, 0);
	if (socket_fd < 0) {
		printf ("socket() failed\n");
		return 1;
	}
	memset (&address, 0, sizeof (struct sockaddr_un));

	/* populating the struct for the socket */
	address.sun_family = AF_UNIX;
	snprintf (address.sun_path, UNIX_PATH_MAX,
		"./../../network/output/demo_socket");

	/* make the connection */
  	if (connect (socket_fd,
			(struct sockaddr *) &address,
	       sizeof (struct sockaddr_un)) != 0)
	{
		printf ("connect() failed\n");
		return 1;	
	}

	/* just debugging stuff for me to experiment with and inspect the buffer */
  int t;
  memcpy (&t, &buffer[0], 4);
  printf ("message size: %d, %d, %d\n", &(andi->msg_size)[0], t, buffer);
  
	/* write to the socket and pass the buffer on to the server */
	write (socket_fd, andi, MSG_SIZE);

	/* read the server's response */
	nbytes = read (socket_fd, andi, MSG_SIZE);
	
	/* more debugging stuff */
	printf
    ("3333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333\n");
	int i;
	for (i = 0; i < 100; i++) {
		if (&andi[i] == '\0') {
			printf ("X");
		}
      printf ("%c ", &andi[i]);
	}
	printf ("<<\n");

	printf ("MESSAGE FROM ANDI: %d\n", &andi->msg_size);
	int tmp;
	memcpy (&tmp, &andi->data, 4);
	printf ("MESSAGE FROM SERVER: %d\n", andi->data);
	printf ("MESSAGE FROM SERVER: %d\n", tmp);
	memcpy (&tmp, &andi->num_of_args, 4);
	printf ("MESSAGE FROM SERVER: %d\n", tmp);
	memcpy (&tmp, &andi->call_num, 4);
	printf ("MESSAGE FROM SERVER: %d\n", tmp);
	memcpy (&tmp, &andi->data, 4);
	printf ("MESSAGE FROM SERVER: %d\n", tmp);
	
	/* close the socket's filedescriptor handle */
	close (socket_fd);

	/* I should return a void pointer so the caller can figure out what to do
	 * but this should come from the buffer somewhere
	 */
	return atoi (andi->call_num);

}


