





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
