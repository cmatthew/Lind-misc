#include <eti.h> 
#include <features.h>
#include <crypt.h>
//#include <openssl/rsa.h>
#include <openssl/md5.h>  
 #include <openssl/md4.h>
// #include <openssl/md2.h>

#include "../output/tagstmp_mm.h"
typedef long int ptrdiff_t;
typedef long unsigned int size_t;
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
int MD5_Init(MD5_CTX *c) {
return serialize_MD5_Init(1, sizeof(c), c, 13);

}
int MD5_Update(MD5_CTX *c, const void *data, size_t len) {
return serialize_MD5_Update(3, sizeof(c), c, sizeof(data), data, sizeof(len), len, 14);

}
int MD5_Final(unsigned char *md, MD5_CTX *c) {
return serialize_MD5_Final(2, strlen(md), md, sizeof(c), c, 15);

}
unsigned char *MD5(const unsigned char *d, size_t n, unsigned char *md) {
return serialize_MD5(3, strlen(d), d, sizeof(n), n, strlen(md), md, 16);

}
void MD5_Transform(MD5_CTX *c, const unsigned char *b) {
return serialize_MD5_Transform(2, sizeof(c), c, strlen(b), b, 17);

}
int MD4_Init(MD4_CTX *c) {
return serialize_MD4_Init(1, sizeof(c), c, 18);

}
int MD4_Update(MD4_CTX *c, const void *data, size_t len) {
return serialize_MD4_Update(3, sizeof(c), c, sizeof(data), data, sizeof(len), len, 19);

}
int MD4_Final(unsigned char *md, MD4_CTX *c) {
return serialize_MD4_Final(2, strlen(md), md, sizeof(c), c, 20);

}
unsigned char *MD4(const unsigned char *d, size_t n, unsigned char *md) {
return serialize_MD4(3, strlen(d), d, sizeof(n), n, strlen(md), md, 21);

}
void MD4_Transform(MD4_CTX *c, const unsigned char *b) {
return serialize_MD4_Transform(2, sizeof(c), c, strlen(b), b, 22);

}
