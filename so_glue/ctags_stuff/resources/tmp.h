#define __attribute__(x)
#define __extension__(x) 
#define __const const

/* these are working */
int foo(int x);
unsigned int foo_add(int x, int y);
char* one_line(char * string);
char foo_char(int x, char y);
short foo_short_add(int x, short y);
/* ^^ these are working ^^ */


long foo_long(long longx, long longy, int intx);
float foo_float(float x, int y);
double foo_double(int intx, double doublex, double doubley);
_Bool foo_bool(_Bool boo);

/* #include <eti.h> /\* works *\/ */
/* #include <crypt.h> /\* works *\/ */
/* #include <openssl/md5.h> /\* works *\/ */
/* #include <openssl/md4.h> /\* works *\/ */
/* #include <openssl/md2.h> /\* works *\/ */

//void no_comments(char * string) ;



