/* #include <eti.h> /\* works *\/ */
/* #include <crypt.h> /\* works *\/ */
/* #include <openssl/md5.h> /\* works *\/ */
/* #include <openssl/md4.h> /\* works *\/ */
/* #include <openssl/md2.h> /\* works *\/ */

#include "../output/tagstmp_mm.h"
int foo(int x) {
return serialize_foo(1, sizeof(x), x, 0);

}
unsigned int foo_add(int x, int y) {
return serialize_foo_add(2, sizeof(x), x, sizeof(y), y, 1);

}
char* one_line(char * string) {
return serialize_one_line(1, strlen(string), string, 2);

}
char foo_char(int x, char y) {
return serialize_foo_char(2, sizeof(x), x, sizeof(y), y, 3);

}
short foo_short_add(int x, short y) {
return serialize_foo_short_add(2, sizeof(x), x, sizeof(y), y, 4);

}
long foo_long(long longx, long longy, int intx) {
return serialize_foo_long(3, sizeof(longx), longx, sizeof(longy), longy, sizeof(intx), intx, 5);

}
float foo_float(float x, int y) {
return serialize_foo_float(2, sizeof(x), x, sizeof(y), y, 6);

}
double foo_double(int intx, double doublex, double doubley) {
return serialize_foo_double(3, sizeof(intx), intx, sizeof(doublex), doublex, sizeof(doubley), doubley, 7);

}
_Bool foo_bool(_Bool boo) {
return serialize_foo_bool(1, sizeof(boo), boo, 8);

}
