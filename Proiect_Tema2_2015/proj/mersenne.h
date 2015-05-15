#ifndef MERSENNE_H_INCLUDED
#define MERSENNE_H_INCLUDED
//definitia functiilor din mersenne.c

void init_genrand(unsigned long s);
unsigned long genrand_int32(void);
double genrand_real1(void);
double genrand_real2(void);
double genrand_real3(void);

#endif // MERSENNE_H_INCLUDED
