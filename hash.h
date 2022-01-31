//hash
//http://cgi.di.uoa.gr/~antoulas/k24/homeworks/h39jdk2.c
#ifndef _HASH__
#define _HASH__
#define HVALUE 137
//hashfuctions given by the instructor
unsigned long djb2(unsigned char *str);
unsigned long sdbm(unsigned char *str);
unsigned long hash_i(unsigned char *str, unsigned int i);

#endif
