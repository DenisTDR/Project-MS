#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "mersenne.c"
double SimulExp(double theta)
{
    return -theta*log(1-genrand_real2());
}

double SimulBeta(int a, int b)
{
    double *x,G=0,B=0;
    int n,i;
    n=a+b+1;
    x=(double*)malloc(n * sizeof(double));

    for(i=0;i<n;i++)
    {
        x[i]=SimulExp(1);
        G=G+x[i];
    }
    for(i=0;i<a-1;i++)
    {
        B=B+x[i]/G;
    }
    return B;
}
int main()
{
    init_genrand(time(NULL));
    double y;
    y=SimulBeta(5,3);
    printf("%lf", y);

    getch();
    return 0;
}
