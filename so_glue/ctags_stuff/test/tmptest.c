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
