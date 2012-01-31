crypt            prototype    23 xxxtest.h        extern char *crypt (const char *__key, const char *__salt)
setkey           prototype    27 xxxtest.h        extern void setkey (const char *__key) ;
encrypt          prototype    31 xxxtest.h        extern void encrypt (char *__block, int __edflag) ;
ptrdiff_t        typedef      37 xxxtest.h        typedef long int ptrdiff_t;
size_t           typedef      38 xxxtest.h        typedef long unsigned int size_t;
MD5state_st      struct       40 xxxtest.h        typedef struct MD5state_st
A                member       42 xxxtest.h        unsigned int A,B,C,D;
B                member       42 xxxtest.h        unsigned int A,B,C,D;
C                member       42 xxxtest.h        unsigned int A,B,C,D;
D                member       42 xxxtest.h        unsigned int A,B,C,D;
Nh               member       43 xxxtest.h        unsigned int Nl,Nh;
Nl               member       43 xxxtest.h        unsigned int Nl,Nh;
data             member       44 xxxtest.h        unsigned int data[(64/4)];
num              member       45 xxxtest.h        unsigned int num;
MD5_CTX          typedef      46 xxxtest.h        } MD5_CTX;
MD5_Init         prototype    51 xxxtest.h        int MD5_Init(MD5_CTX *c);
MD5_Update       prototype    52 xxxtest.h        int MD5_Update(MD5_CTX *c, const void *data, size_t len);
MD5_Final        prototype    53 xxxtest.h        int MD5_Final(unsigned char *md, MD5_CTX *c);
MD5              prototype    54 xxxtest.h        unsigned char *MD5(const unsigned char *d, size_t n, unsigned char *md);
MD5_Transform    prototype    55 xxxtest.h        void MD5_Transform(MD5_CTX *c, const unsigned char *b);
MD4state_st      struct       60 xxxtest.h        typedef struct MD4state_st
A                member       62 xxxtest.h        unsigned int A,B,C,D;
B                member       62 xxxtest.h        unsigned int A,B,C,D;
C                member       62 xxxtest.h        unsigned int A,B,C,D;
D                member       62 xxxtest.h        unsigned int A,B,C,D;
Nh               member       63 xxxtest.h        unsigned int Nl,Nh;
Nl               member       63 xxxtest.h        unsigned int Nl,Nh;
data             member       64 xxxtest.h        unsigned int data[(64/4)];
num              member       65 xxxtest.h        unsigned int num;
MD4_CTX          typedef      66 xxxtest.h        } MD4_CTX;
MD4_Init         prototype    71 xxxtest.h        int MD4_Init(MD4_CTX *c);
MD4_Update       prototype    72 xxxtest.h        int MD4_Update(MD4_CTX *c, const void *data, size_t len);
MD4_Final        prototype    73 xxxtest.h        int MD4_Final(unsigned char *md, MD4_CTX *c);
MD4              prototype    74 xxxtest.h        unsigned char *MD4(const unsigned char *d, size_t n, unsigned char *md);
MD4_Transform    prototype    75 xxxtest.h        void MD4_Transform(MD4_CTX *c, const unsigned char *b);
MD2state_st      struct       82 xxxtest.h        typedef struct MD2state_st
num              member       84 xxxtest.h        unsigned int num;
data             member       85 xxxtest.h        unsigned char data[16];
cksm             member       86 xxxtest.h        unsigned int cksm[16];
state            member       87 xxxtest.h        unsigned int state[16];
MD2_CTX          typedef      88 xxxtest.h        } MD2_CTX;
MD2_options      prototype    90 xxxtest.h        const char *MD2_options(void);
MD2_Init         prototype    94 xxxtest.h        int MD2_Init(MD2_CTX *c);
MD2_Update       prototype    95 xxxtest.h        int MD2_Update(MD2_CTX *c, const unsigned char *data, size_t len);
MD2_Final        prototype    96 xxxtest.h        int MD2_Final(unsigned char *md, MD2_CTX *c);
MD2              prototype    97 xxxtest.h        unsigned char *MD2(const unsigned char *d, size_t n,unsigned char *md);
no_comments      prototype    98 xxxtest.h        void no_comments(char * string) ;
one_line         prototype    99 xxxtest.h        char* one_line(char * string);
