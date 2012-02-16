#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "uds_helper.h"
#include "../../ctags_stuff/output/deserializer.h"

int MSG_SIZE = _MSG_SIZE;


int connection_handler (int connection_fd) {
	int nbytes = -1;
	char * buffer = malloc(MSG_SIZE);

	/* message *andi; */
	/* int rc = -1;  /\* return codes for system calls. *\/ */
	nbytes = read (connection_fd, buffer, MSG_SIZE);
	buffer = (void *) des(buffer);
	
	message * reply = (message *) buffer;

//	if(&buffer[4] == 2) {
		printf("this is it in the SERVER ON WAY OUT: %s\n", &reply[20]);
//	}

	int rc = write(connection_fd, reply, MSG_SIZE);
	assert(rc != -1);
	return 0;

	/* andi = (message *) buffer; */
		
	/* rc = write(connection_fd, andi, MSG_SIZE); */
	/* assert(rc == MSG_SIZE); */

	/* close (connection_fd); */

	/* int ret_val, ret_size; */
	/* memcpy(&ret_size, &buffer[0], 4); */
	/* memcpy(&ret_val, &buffer[4], ret_size); */
	/* printf("deserialize result: %d\n", ret_val); */
  /* return 0; */
}


#define MAX_CONNECTIONS 5


int
main (void)
{
  struct sockaddr_un address;
  int socket_fd = -1, connection_fd = -1;
  socklen_t address_length = 0;
  int rc = 0;
  assert(_MSG_SIZE == sizeof(message));

  /* start with a clean address structure */
  memset (&address, 0, sizeof (struct sockaddr_un));

  socket_fd = socket (PF_UNIX, SOCK_STREAM, 0);
  if (socket_fd < 0)
    {
      printf ("socket() failed\n");
      return EXIT_FAILURE;
    }

  address.sun_family = AF_UNIX;
  rc = snprintf(address.sun_path + 1, sizeof(address.sun_path) -2, SOCK_PATH);
  assert(rc == strlen(SOCK_PATH));

  if (bind(socket_fd, (struct sockaddr *) &address, sizeof(struct sockaddr_un) ) != 0)
    {
      printf ("bind() failed\n");
      return EXIT_FAILURE;
    }

  if (listen (socket_fd, MAX_CONNECTIONS) != 0)
    {
      printf ("listen() failed\n");
      return EXIT_FAILURE;
    }

  while ((connection_fd = accept (socket_fd,
				  (struct sockaddr *) &address,
				  &address_length)) > -1)
    {
  
	  /* now inside newly created connection handling process */
	  connection_handler (connection_fd);

      /* still inside server process */
      close (connection_fd);
    }

  close (socket_fd);
  return 0;
}
