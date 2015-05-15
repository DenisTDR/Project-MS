#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "mersenne.h"

int N = 1000, k = 3;
double alpha = 0.03;

int *nrafis;
double *pr, *nrmclicks;

void *allocArrayMemory(int sizeOfElement, int count);
void initializareMemorie();
void afisare(int N, int k, double * pr, double * nrmclicks);

int Bernoulli(double);
int SimulUnifD(int);
int indexMax(double*, int);
int selectAd(double, int, int);


int main(){
	int i, j;
	int reclamaNrMediuMax;
	char clicked;
	init_genrand(time(NULL));


	initializareMemorie();

	for(i=0; i<N; i++){
		reclamaNrMediuMax = indexMax(nrmclicks, k);
		j = selectAd(alpha, k, reclamaNrMediuMax);
		nrafis[j]++;
		clicked = Bernoulli(pr[j]);
		if(clicked){
			nrmclicks[j] = (nrmclicks[j] * (nrafis[j] - 1) + 1) / nrafis[j];
		}
	}
	afisare(N, k, pr, nrmclicks);

	free(pr);
	free(nrmclicks);
	free(nrafis);

	getch();
	return 0;
}

void initializareMemorie(){
	pr = (double*)allocArrayMemory(sizeof(double), k);
	nrmclicks = (double*)allocArrayMemory(sizeof(double), k);
	nrafis = (int*)allocArrayMemory(sizeof(int), k);
}

void* allocArrayMemory(int sizeOfElement, int count){
	void *tmp;
	do{
		tmp = calloc(sizeOfElement, count);
	}while(!tmp);
	return tmp;
}
int Bernoulli(double p)
{
    return genrand_real2()<p;
}

int SimulUnifD(int k)
{
    return (int)(k*genrand_real2())+1;
}
void afisare(int N, int k, double * pr, double * nrmclicks){
	int i;
	printf("Numarul de useri: %d\nNumarul de reclame: %d\n", N, k);
	printf("Probabilitati initiale:\n\n");
	printf("%10s %20s %20s %20s\n",
	 "Nr. Clicks", "Probabilitate", "Nr. Mediu Clicks", "Diferenta");
	for (i=0; i!=k; i++){
		
		printf("%10d %20lf %20lf %20lf\n",
	 		i+1, pr[i], nrmclicks[i], fabs(nrmclicks[i]-pr[i]));
	}	
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

int selectAd(double alpha, int k, int imax){
    double u;
    u = genrand_real2();
    if(u<alpha)
        return SimulUnifD(k);
    else
        return imax; 
}