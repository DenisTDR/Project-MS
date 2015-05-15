#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mersenne.h"

int k=3, n=1000 * 10;
int *nrafis, *nrclicks;
double *pr, *theta;

void* allocArrayMemory(int sizeOfElement, int count);
void initializareMemorie();
double SimulBeta(int a, int b);
int indexMax (double *x, int L);
int Bernoulli(double p);
double SimulBeta(int a, int b);
double SimulExp(double theta);
void afisareSiScriere(void);

int main()
{
    int i, j;
    char clicked;
    init_genrand(time(NULL));
    initializareMemorie();
    pr[0] = 0.53;
    pr[1] = 0.2;
    pr[2] = 0.38;
    pr[3] = 0.13;

    for(i=0; i<n; i++){
        for(j=0; j<k; j++){
            theta[j] = SimulBeta(1 + nrclicks[j], 1+nrafis[j] - nrclicks[j]);
        }
        j = indexMax(theta, k);
        nrafis[j] ++;
        clicked = Bernoulli(pr[j]);
        nrclicks[j] += clicked;
    }
    afisareSiScriere();

    return 0;
}

void* allocArrayMemory(int sizeOfElement, int count){
	void *tmp;
	do{
		tmp = calloc(sizeOfElement, count);
	}while(!tmp);
	return tmp;
}

void initializareMemorie(){
	theta = (double*)allocArrayMemory(sizeof(double), k);
	pr = (double*)allocArrayMemory(sizeof(double), k);
	nrclicks = (int*)allocArrayMemory(sizeof(int), k);
	nrafis = (int*)allocArrayMemory(sizeof(int), k);
}

int indexMax (double *x, int L)  {
    int i, pozitie_max;
    pozitie_max = 0;
    for(i=0;i<=L;i++){
        if(x[pozitie_max] < x[i]) {
            pozitie_max=i;
        }
    }
    return pozitie_max;
}
int Bernoulli(double p)
{
    return genrand_real2()<p;
}

void afisareSiScriere(){
	int i;
	double m;
	FILE *f;

	if(!(f = fopen("date.txt", "wt"))){
        printf("nu pot sa deschid fisierul!");
        exit(0);
	}


	printf("Numarul de useri: %d\nNumarul de reclame: %d\n", n, k);

	printf("%10s %20s %20s %20s %15s %15s\n",
	 "Index Reclama", "Probabilitate", "Media Beta", "Diferenta", "last a", "last b");

	for (i=0; i!=k; i++){
        m  = (1.0+nrclicks[i])/(2.0+nrafis[i]);
		printf("%10d %20lf %20lf %20lf %15d %15d\n",
	 		i+1, pr[i], m, fabs(m-pr[i]), 1 + nrclicks[i], 1 + nrafis[i] - nrclicks[i]);
        fprintf(f, "%d %d\n",  1 + nrclicks[i], 1 + nrafis[i] - nrclicks[i]);

	}
	fclose(f);
}
double SimulExp(double theta)
{
    return -theta*log(1-genrand_real2());
}

double SimulBeta(int a, int b)
{
    double *x,G=0,B=0;
    int n,i;
    n=a+b+1;
    x=(double*)allocArrayMemory(sizeof(double), n+1);
    for(i=0;i<=n;i++)
    {
        x[i]=SimulExp(1);
        G=G+x[i];
    }
    for(i=0;i<a;i++)
    {
        B=B+x[i]/G;
    }
    free(x);
    return B;
}
