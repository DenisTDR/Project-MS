#include <stdio.h>
#include <stdlib.h>
#include "mersenne.c"


typedef struct Film{
	int cod; // codul filmului
	int rank;// popularitatea filmului
	int nrv;// nr de vizionari ale filmului avand codul cod
	double pre;// probabilitatea de vizionare a filmului
}Film;

void* allocArrayMemory(int, int);
double *Zipf(double alpha, int n);
void frecventa(int *fv, int L, int n, Film *film);
void ReRank(Film * f, int n);

int main(){
	Film *film;
	int n = 200;
	film = (Film*)allocArrayMemory(sizeof(Film), n);
	//citirea
	
	qsort(film, n, sizeof(Film), &cmpFilm);//sortarea

	ReRank(film, n);//calculul rangurilor

	getch();
	return 0;
}

double *Zipf(double alpha, int n){
	double *pr, s, c;
	char i;
	pr = (double *)allocArrayMemory(sizeof(double), n);
	s=0;
	for(i=0;i<=n;i++)
		s+=(pr[i] = 1/pow(i, alpha))
	c=1/s;

	for(i=0;i<=n;i++)
		pr[i]*=c;

	return pr;
}

void ReRank(Film * f, int n)
{
    int i;
    for (i=0; i!=n; i++)
        f[i].rank=i+1;
}

void* allocArrayMemory(int sizeOfElement, int count){
	void *tmp;
	do{
		tmp = calloc(sizeOfElement, count);
	}while(!tmp);
	return tmp;
}

int cmpFilm(void * pf1, void * pf2)
{
    Film *p1=(Film *)pf1, *p2=(Film *)pf2;

    return (p1->nrv)-(p2->nrv);
}
