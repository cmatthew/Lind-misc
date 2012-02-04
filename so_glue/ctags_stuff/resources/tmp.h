#define __attribute__(x)
#define __extension__(x) 
#define __const const


int foo(int x);


#include <eti.h> /* works */
#include <crypt.h> /* works */
#include <openssl/md5.h> /* works */
#include <openssl/md4.h> /* works */
#include <openssl/md2.h> /* works */


void no_comments(char * string) ;
char* one_line(char * string);


