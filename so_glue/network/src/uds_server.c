#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "uds_helper.h"
#include "../../ctags_stuff/output/deserializer.h"
int UNIX_PATH_MAX = 40;


int connection_handler (int connection_fd) {
	int nbytes;
	char buffer[MSG_SIZE];
	message *andi;
	nbytes = read (connection_fd, buffer, MSG_SIZE);
	andi = (message *) buffer;
		
	
	write(connection_fd, andi, MSG_SIZE);
	close (connection_fd);
	buffer = (char *) des(buffer);
	int ret_val, ret_size;
	memcpy(&ret_size, &buffer[0], 4);
	memcpy(&ret_val, &buffer[4], ret_size);
	printf("deserialize result: %d\n", ret_val);
  return 0;
}

int
main (void)
{
  struct sockaddr_un address;
  int socket_fd, connection_fd;
  socklen_t address_length;
  pid_t child;

  socket_fd = socket (PF_UNIX, SOCK_STREAM, 0);
  if (socket_fd < 0)
    {
      printf ("socket() failed\n");
      return 1;
    }

  unlink ("./../output/demo_socket");

  /* start with a clean address structure */
  memset (&address, 0, sizeof (struct sockaddr_un));

  address.sun_family = AF_UNIX;
  snprintf (address.sun_path, UNIX_PATH_MAX, "./../output/demo_socket");

  if (bind (socket_fd,
	    (struct sockaddr *) &address, sizeof (struct sockaddr_un)) != 0)
    {
      printf ("bind() failed\n");
      return 1;
    }

  if (listen (socket_fd, 5) != 0)
    {
      printf ("listen() failed\n");
      return 1;
    }

  while ((connection_fd = accept (socket_fd,
				  (struct sockaddr *) &address,
				  &address_length)) > -1)
    {
      child = fork ();
      if (child == 0)
	{
	  /* now inside newly created connection handling process */
	  return connection_handler (connection_fd);
	}

      /* still inside server process */
      close (connection_fd);
    }

  close (socket_fd);
  unlink ("./../output/demo_socket");
  return 0;
}
