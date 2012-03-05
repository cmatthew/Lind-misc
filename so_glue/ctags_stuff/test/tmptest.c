#include <eti.h> 
#include <features.h>
#include <crypt.h>
//#include <openssl/rsa.h>
//#include <openssl/md5.h>  
/* #include <openssl/md4.h> /\* works *\/ */
/* #include <openssl/md2.h> /\* works *\/ */

#include "../output/tagstmp_mm.h"
int foo(int x) {
return serialize_foo(1, sizeof(x), x, 0);

}
unsigned int foo_add(int x, int y) {
return serialize_foo_add(2, sizeof(x), x, sizeof(y), y, 1);

}
char foo_char(int x, char y) {
return serialize_foo_char(2, sizeof(x), x, sizeof(y), y, 2);

}
short foo_short_add(int x, short y) {
return serialize_foo_short_add(2, sizeof(x), x, sizeof(y), y, 3);

}
long foo_long(long longx, long longy, int intx) {
return serialize_foo_long(3, sizeof(longx), longx, sizeof(longy), longy, sizeof(intx), intx, 4);

}
float foo_float(float x, int y) {
return serialize_foo_float(2, sizeof(x), x, sizeof(y), y, 5);

}
double foo_double(int intx, double doublex, double doubley) {
return serialize_foo_double(3, sizeof(intx), intx, sizeof(doublex), doublex, sizeof(doubley), doubley, 6);

}
_Bool foo_bool(_Bool boo) {
return serialize_foo_bool(1, sizeof(boo), boo, 7);

}
char* one_line(char * string) {
return serialize_one_line(1, strlen(string), string, 8);

}
char* two_strings(char * string1, char * string2) {
return serialize_two_strings(2, strlen(string1), string1, strlen(string2), string2, 9);

}
extern char *crypt (const char *__key, const char *__salt) {
return serialize_crypt(2, strlen(__key), __key, strlen(__salt), __salt, 10);

}
extern void setkey (const char *__key)  {
return serialize_setkey(1, strlen(__key), __key, 11);

}
extern void encrypt (char *__block, int __edflag)  {
return serialize_encrypt(2, strlen(__block), __block, sizeof(__edflag), __edflag, 12);

}
