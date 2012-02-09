#include <eti.h> /* works */
#include <crypt.h> /* works */
#include <openssl/md5.h> /* works */
#include <openssl/md4.h> /* works */
#include <openssl/md2.h> /* works */
#include "../output/tagstmp_mm.h"
typedef long int ptrdiff_t;
typedef long unsigned int size_t;
int foo(int x) {
return serialize_foo(1, sizeof(x), x, 0);

}
unsigned int foo_add(int x, int y) {
return serialize_foo_add(2, sizeof(x), x, sizeof(y), y, 1);

}
extern char *crypt (const char *__key, const char *__salt) {
return *serialize_crypt(2, sizeof(__key), __key, sizeof(__salt), __salt, 2);

}
extern void setkey (const char *__key)  {
return serialize_setkey(1, sizeof(__key), __key, 3);

}
extern void encrypt (char *__block, int __edflag)  {
return serialize_encrypt(2, sizeof(__block), __block, sizeof(__edflag), __edflag, 4);

}
int MD5_Init(MD5_CTX *c) {
return serialize_MD5_Init(1, sizeof(c), c, 5);

}
int MD5_Update(MD5_CTX *c, const void *data, size_t len) {
return serialize_MD5_Update(3, sizeof(c), c, sizeof(data), data, sizeof(len), len, 6);

}
int MD5_Final(unsigned char *md, MD5_CTX *c) {
return serialize_MD5_Final(2, sizeof(md), md, sizeof(c), c, 7);

}
unsigned char *MD5(const unsigned char *d, size_t n, unsigned char *md) {
return *serialize_MD5(3, sizeof(d), d, sizeof(n), n, sizeof(md), md, 8);

}
void MD5_Transform(MD5_CTX *c, const unsigned char *b) {
return serialize_MD5_Transform(2, sizeof(c), c, sizeof(b), b, 9);

}
int MD4_Init(MD4_CTX *c) {
return serialize_MD4_Init(1, sizeof(c), c, 10);

}
int MD4_Update(MD4_CTX *c, const void *data, size_t len) {
return serialize_MD4_Update(3, sizeof(c), c, sizeof(data), data, sizeof(len), len, 11);

}
int MD4_Final(unsigned char *md, MD4_CTX *c) {
return serialize_MD4_Final(2, sizeof(md), md, sizeof(c), c, 12);

}
unsigned char *MD4(const unsigned char *d, size_t n, unsigned char *md) {
return *serialize_MD4(3, sizeof(d), d, sizeof(n), n, sizeof(md), md, 13);

}
void MD4_Transform(MD4_CTX *c, const unsigned char *b) {
return serialize_MD4_Transform(2, sizeof(c), c, sizeof(b), b, 14);

}
const char *MD2_options(void) {
return *serialize_MD2_options(1, 0/*FIX ME*/, 0/*FIX ME*/, 15);

}
int MD2_Init(MD2_CTX *c) {
return serialize_MD2_Init(1, sizeof(c), c, 16);

}
int MD2_Update(MD2_CTX *c, const unsigned char *data, size_t len) {
return serialize_MD2_Update(3, sizeof(c), c, sizeof(data), data, sizeof(len), len, 17);

}
int MD2_Final(unsigned char *md, MD2_CTX *c) {
return serialize_MD2_Final(2, sizeof(md), md, sizeof(c), c, 18);

}
unsigned char *MD2(const unsigned char *d, size_t n,unsigned char *md) {
return *serialize_MD2(3, sizeof(d), d, sizeof(n), n, sizeof(md), md, 19);

}
void no_comments(char * string)  {
return serialize_no_comments(1, sizeof(string), string, 20);

}
char* one_line(char * string) {
return serialize_one_line(1, sizeof(string), string, 21);

}
