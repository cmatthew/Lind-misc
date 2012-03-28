





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




















extern char *crypt (const char *__key, const char *__salt)
     ;


extern void setkey (const char *__key) ;



extern void encrypt (char *__block, int __edflag) ;





typedef long int ptrdiff_t;
typedef long unsigned int size_t;
typedef int wchar_t;
typedef struct MD5state_st
 {
 unsigned int A,B,C,D;
 unsigned int Nl,Nh;
 unsigned int data[(64/4)];
 unsigned int num;
 } MD5_CTX;




int MD5_Init(MD5_CTX *c);
int MD5_Update(MD5_CTX *c, const void *data, size_t len);
int MD5_Final(unsigned char *md, MD5_CTX *c);
unsigned char *MD5(const unsigned char *d, size_t n, unsigned char *md);
void MD5_Transform(MD5_CTX *c, const unsigned char *b);




typedef struct MD4state_st
 {
 unsigned int A,B,C,D;
 unsigned int Nl,Nh;
 unsigned int data[(64/4)];
 unsigned int num;
 } MD4_CTX;




int MD4_Init(MD4_CTX *c);
int MD4_Update(MD4_CTX *c, const void *data, size_t len);
int MD4_Final(unsigned char *md, MD4_CTX *c);
unsigned char *MD4(const unsigned char *d, size_t n, unsigned char *md);
void MD4_Transform(MD4_CTX *c, const unsigned char *b);
