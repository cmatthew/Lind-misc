#include <assert.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <sys/poll.h>

#include <netinet/in.h>
#include <arpa/nameser.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <resolv.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main() {
    int s = socket(PF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in saddr;
    struct sockaddr_in them;
    socklen_t them_t = sizeof(struct sockaddr_in);
    memset(&saddr, 0, sizeof(struct sockaddr_in));
    
    saddr.sin_family = AF_INET;    /* Internet socket */
    saddr.sin_port = htons(10000); /* Port number */
    inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr);

    if (connect (s, &saddr, sizeof(struct sockaddr_in)) < 0) {
        perror("connect");
        return -1;
    }
    char * message = "Hello there";
    
    send(s, message, strlen(message) + 1, 0);

    recvfrom(s, message, strlen(message) + 1, 0, &them, &them_t);

    return 0;
}
