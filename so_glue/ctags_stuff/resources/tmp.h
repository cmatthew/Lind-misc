#define __attribute__(x)
#define __extension__(x) 
#define __const const

/* these are working */
//int foo_empty();
int foo(int x);
unsigned int foo_add(int x, int y);
char foo_char(int x, char y);
short foo_short_add(int x, short y);

long foo_long(long longx, long longy, int intx);
float foo_float(float x, int y);
double foo_double(int intx, double doublex, double doubley);
_Bool foo_bool(_Bool boo);


char* one_line(char * string);
char* two_strings(char * string1, char * string2);
/* ^^ these are working ^^ */
/*
struct my_struct{
	int first;
	int second;
};
*/

#include <eti.h> 
 //\* works *\/
#include <features.h>
#include <crypt.h>
//#include <openssl/rsa.h>
#include <openssl/md5.h>  
/* #include <openssl/md4.h> /\* works *\/ */
/* #include <openssl/md2.h> /\* works *\/ */

//void no_comments(char * string) ;



