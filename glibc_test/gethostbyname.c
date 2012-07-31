/*
 * gethostbyname.c - Example of using gethostbyname(3)
 * Martin Vidner <mvidner@suse.cz>
 */

#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>


struct hostent *he;
struct in_addr a;

#define SERVER "hypervisor-ilo.cs.uvic.ca"

int
main ()
{
    he = gethostbyname (SERVER);
    if (he)
    {
        printf("name: %s\n", he->h_name);
        while (*he->h_aliases)
            printf("alias: %s\n", *he->h_aliases++);
        while (*he->h_addr_list)
        {
            bcopy(*he->h_addr_list++, (char *) &a, sizeof(a));
            printf("address: %s\n", inet_ntoa(a));
        }
    }
    else
        herror(SERVER);
    return 0;
}
