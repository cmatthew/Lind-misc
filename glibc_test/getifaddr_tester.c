#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <errno.h>
#include <assert.h>
#include <string.h>

int main() {
    struct ifaddrs *myaddrs, *ifa;
    struct sockaddr_in *s4;
    int status;
    /* buf must be big enough for an IPv6 address (e.g. 3ffe:2fa0:1010:ca22:020a:95ff:fe8a:1cf8) */
    char buf[64];

    status = getifaddrs(&myaddrs);
    if (status != 0) {
        printf("Getifaddrs returned %d\n", status);
        fflush(stdout);
        exit(1);
        
    } else {
        printf("Getifaddrs returned %d\n", status);
        fflush(stdout);
    }        
    
    for (ifa = myaddrs; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL) continue;
        if ((ifa->ifa_flags & IFF_UP) == 0) continue;
        
        if (ifa->ifa_addr->sa_family == AF_INET) {
            s4 = (struct sockaddr_in *)(ifa->ifa_addr);
            if (inet_ntop(ifa->ifa_addr->sa_family, (void *)&(s4->sin_addr), buf, sizeof(buf)) == NULL) {
                printf("%s: inet_ntop failed!\n", ifa->ifa_name);
                assert(0); // inet_top should never fail in lind.
            } else {
                printf("%s: %s\n", ifa->ifa_name, buf);
                assert(strlen(buf) > 8);// IP addresses are at least 8
                // characters long.
                assert(strlen(ifa->ifa_name) >= 3); // Adapter names should always
                // be at least three characters long
                
            }
        }
    }
    
    freeifaddrs(myaddrs);
    exit(0);
}

