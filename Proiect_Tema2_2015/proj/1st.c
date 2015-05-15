#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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
void reRank(Film * f, int n, int vTot);
int cmpFilm(const void * pf1,const  void * pf2);
void scrierePuncte(char *out, Film *film, int n);
double *VectRep(double *pr, int n);
int SimulZipf(int n, double *F);
void readInputFile(char *fileName, Film *film, int n, int *vTot);

int main(){
	Film *film;
	int n = 200, vTot;
	film = (Film*)allocArrayMemory(sizeof(Film), n);
	//citirea
	readInputFile("DateFilme.txt", film, n, &vTot);

	qsort(film, n, sizeof(Film), cmpFilm);//sortarea

	reRank(film, n, vTot);//calculul rangurilor

	writeDebug(film, n);

	scrierePuncte("NumeLeader2.txt", film, n);

	free(film);
	getch();
	return 0;
}

void writeDebug(Film *film, int n){
	int i;
	for(i=0;i<n;i++)
		printf("%d %lf %d\n", film[i].cod, film[i].pre, film[i].nrv);
}

// int cod; // codul filmului
// 	int rank;// popularitatea filmului
// 	int nrv;// nr de vizionari ale filmului avand codul cod
// 	double pre;/

double *Zipf(double alpha, int n){
	double *pr, s, c;
	char i;
	pr = (double *)allocArrayMemory(sizeof(double), n);
	s=0;
	for(i=0;i<n;i++)
		s+=(pr[i] = 1/pow(i+1, alpha));
	c=1/s;

	for(i=0;i<n;i++)
		pr[i]*=c;

	return pr;
}

double *VectRep(double *pr, int n)
{
    int i;
    double *rep;
    rep = (double *)allocArrayMemory(sizeof(double), n+1);
    rep[0]=0;
    for(i=0;i<n;i++)
        rep[i]=rep[i-1]+pr[i-1];
    rep[n]=1; //suma probabilitatilor 1
    return rep;
}

int SimulZipf(int n, double *F)
{
    int L=0,R=n,m;
    double u;
    u=genrand_real1();
    while(L<R){
        m=(L+R)/2;
        if(u<F[m])
            R=m;
        else
            L=m+1;
    }
    return L;
}
void reRank(Film *film, int n, int vTot)
{
    int i;
    for (i=0; i!=n; i++){
        film[i].rank=i+1;
        film[i].pre = (double)film[i].nrv/vTot;
    }
}

void scrierePuncte(char *out, Film *film, int n)
{
    FILE *f;
    int i=0;

    if (!(f=fopen(out,"wt")))
    	{perror("Eroare la deschiderea fisierului de iesire."); exit(-1);}
    for (i=0; i<n; i++)
     	if (film[i].pre!=0) // afisam doar punctele care exista 
     		fprintf(f,"%.6lf %.6lf\n",log((double)(film[i].rank+1)),-log(film[i].pre));
    fclose(f);
}


void readInputFile(char *fileName, Film *film, int n, int *vTot){

    FILE *f;
    int x, i;
    for(i=0;i<n;i++){
    	film[i].cod = i+1;
    	film[i].pre = 0;
    	film[i].nrv = 0;
    	film[i].rank = i+1;
    }

    if (!(f=fopen(fileName,"rt")))
    {
        perror("Eroare la deschiderea fisierului.");

        exit(-1);
    }
	*vTot = 0;
    while(fscanf(f,"%d",&x)==1){
        film[x-1].nrv++;
    	(*vTot)++;
    }


    fclose(f);
}

void* allocArrayMemory(int sizeOfElement, int count){
	void *tmp;
	do{
		tmp = calloc(sizeOfElement, count);
	}while(!tmp);
	return tmp;
}

int cmpFilm(const void * pf1,const  void * pf2)
{
    Film *p1=(Film *)pf1, *p2=(Film *)pf2;
    return (p1->nrv)-(p2->nrv);
}
