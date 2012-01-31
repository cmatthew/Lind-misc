typedef long int ptrdiff_t;
typedef long unsigned int size_t;
typedef struct MD5state_st{
unsigned int A;
unsigned int B;
unsigned int C;
unsigned int D;
unsigned int Nh;
unsigned int Nl;
unsigned int data;
unsigned int num;
} MD5_CTX;
typedef struct MD4state_st{
unsigned int A;
unsigned int B;
unsigned int C;
unsigned int D;
unsigned int Nh;
unsigned int Nl;
unsigned int data;
unsigned int num;
} MD4_CTX;
typedef struct MD2state_st{
unsigned int num;
unsigned char data;
unsigned int cksm;
unsigned int state;
} MD2_CTX;
extern char *crypt (const char *__key, const char *__salt) {
return 0;
}
extern void setkey (const char *__key)  {
return ;
}
extern void encrypt (char *__block, int __edflag)  {
return ;
}
int MD5_Init(MD5_CTX *c) {
return 0;
}
int MD5_Update(MD5_CTX *c, const void *data, size_t len) {
return 0;
}
int MD5_Final(unsigned char *md, MD5_CTX *c) {
return 0;
}
unsigned char *MD5(const unsigned char *d, size_t n, unsigned char *md) {
return 0;
}
void MD5_Transform(MD5_CTX *c, const unsigned char *b) {
return ;
}
int MD4_Init(MD4_CTX *c) {
return 0;
}
int MD4_Update(MD4_CTX *c, const void *data, size_t len) {
return 0;
}
int MD4_Final(unsigned char *md, MD4_CTX *c) {
return 0;
}
unsigned char *MD4(const unsigned char *d, size_t n, unsigned char *md) {
return 0;
}
void MD4_Transform(MD4_CTX *c, const unsigned char *b) {
return ;
}
const char *MD2_options(void) {
return 0;
}
int MD2_Init(MD2_CTX *c) {
return 0;
}
int MD2_Update(MD2_CTX *c, const unsigned char *data, size_t len) {
return 0;
}
int MD2_Final(unsigned char *md, MD2_CTX *c) {
return 0;
}
unsigned char *MD2(const unsigned char *d, size_t n,unsigned char *md) {
return 0;
}
void no_comments(char * string)  {
return ;
}
char* one_line(char * string) {
return 0;
}
