#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include "uds_helper.h"
#include "uds_client.h"


int UNIX_PATH_MAX = PATH_MAX;

int MSG_SIZE = 4096;

/* int cli_connect_buffer(char * buffer)
 * Receives a buffer of size 4096 and forwards it to the server
 * Arguments:
 *	buffer	char * containing a serialized version of the actual function call
 * Result:
 *	returns an int currently, should change this to return void * so that the
 *		caller can just return it without problem. The return value should be 
 *		specified somewhere in the buffer probably.
 */
int cli_connect_buffer (void *buffer) {
	
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
	snprintf (address.sun_path + 1, sizeof(address.sun_path) - 2, SOCK_PATH);

	/* make the connection */
  	if (connect (socket_fd,
			(struct sockaddr *) &address,
	       sizeof (struct sockaddr_un)) != 0)
	{
		printf ("connect() failed\n");
		return EXIT_FAILURE;	
	}

	/* just debugging stuff for me to experiment with and inspect the buffer */
	printf ("message size: %u\n", (unsigned int) andi->msg_size);
	printf ("call number: %u\n", (unsigned int) andi->call_num);
 
 	int bool = 2;
	if (andi->call_num == 2) {
		printf ("this is the STRING IN CLIENT going OUT: %s<<<\n", &andi->data[4]);
		bool = 0;
	}


	/* write to the socket and pass the buffer on to the server */
	int rc = write (socket_fd, andi, MSG_SIZE);
	assert(rc == MSG_SIZE);

	/* read the server's response */
	nbytes = read (socket_fd, buffer, MSG_SIZE);

	message * xxx = (message *) buffer;	
	if (bool == 0) {
		printf ("this is the STRING IN CLIENT going BACK: %s<<<\n", xxx->data);
	}
	
	assert(nbytes != -1);
/*
	int ret_size;
	int ret_val;
	memcpy(&ret_size, &buffer[0], 4);
	memcpy(&ret_val, &buffer[20], ret_size);*/
/*	memcpy(&ret_size, &(andi->msg_size), 4);
	memcpy(&ret_val, &(andi->data)[0], ret_size);
*/
	/* close the socket's filedescriptor handle */
	close (socket_fd);

	/* I should return a void pointer so the caller can figure out what to do
	 * but this should come from the buffer somewhere
	 */
	 return 1000;
//	return ret_val;

}


