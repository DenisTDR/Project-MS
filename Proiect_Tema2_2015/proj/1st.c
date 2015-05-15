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
void calcVectRep(double *pr, Film *film, int n, double **pFt, double **pFexp);
int simulZipf(int n, double *F);
void simulareVizualizari(double *F, int n, int k);
void readInputFile(char *fileName, Film *film, int n, int *vTot);
void writeDebug(Film *film, int n);

int main(){
	Film *film;
	int n = 200, vTot, k;
	double alpha, *prt, *Ft, *Fexp;
	film = (Film*)allocArrayMemory(sizeof(Film), n);
	//citirea
	readInputFile("DateFilme.txt", film, n, &vTot);

	qsort(film, n, sizeof(Film), cmpFilm);//sortarea

	reRank(film, n, vTot);//calculul rangurilor

	writeDebug(film, n);

	scrierePuncte("NumeLeader2.txt", film, n);
	//partea de pana aici calculeaza alpha;
	alpha = 0.897675610478;

	prt = Zipf(alpha, n);

	writeOutputFiles(film, n, prt);

	calcVectRep(prt, film, n, &Ft, &Fexp);

	for(k=0;(k<n) && (Ft[k]<0.75);k++);// ";" nu e din greasala!!!!!!!!!
	
	printf("\nNumarul de file salvate in cache este k = %d\n", k);

	simulareVizualizari(Ft, n, k);
	simulareVizualizari(Fexp, n, k);

	if(prt)
		free(prt);
	free(film);
	getch();
	return 0;
}

void writeDebug(Film *film, int n){
	int i;
	for(i=0;i<n;i++)
		printf("%d %lf %d\n", film[i].cod, film[i].pre, film[i].nrv);
}

void writeOutputFiles(Film *film, int n, double *prt) {
	FILE *f1, *f2;
	int i;
	f1 = fopen("ProbsExp.txt", "wt");
	f2 = fopen("ProbsTheor.txt", "wt");
    if (!f1 || !f2)
    {perror("Eroare la deschiderea fisierului de iesire."); exit(-1);}
	for(i=0;i<n;i++){
		fprintf(f1, "%lf\n", film[i].pre);
		fprintf(f2, "%lf\n", prt[i]);
	}
	fclose(f1);
	fclose(f2);
}


double *Zipf(double alpha, int n){
	double *pr, s, c;
	int i;
	pr = (double *)allocArrayMemory(sizeof(double), n);
	s=0;
	for(i=0;i<n;i++)
		s+=(pr[i] = 1/pow(i+1, alpha));
	c=1/s;

	for(i=0;i<n;i++)
		pr[i]*=c;

	return pr;
}

void calcVectRep(double *pr, Film *film, int n, double **pFt, double **pFexp)
{
    int i;
    double *Ft, *Fexp;
    Ft = (double *)allocArrayMemory(sizeof(double), n+1);
    Fexp = (double *)allocArrayMemory(sizeof(double), n+1);

    Ft[0]=0;
    Fexp[0]=0;
    for(i=0;i<n;i++){
        Ft[i]=Ft[i-1]+pr[i-1];
        Fexp[i]=Fexp[i-1]+film[i-1].pre;

    }
    Ft[n]=1;
    Fexp[n]=1; //suma probabilitatilor 1
    (*pFt) = Ft;
    (*pFexp) = Fexp;
}

void simulareVizualizari(double *F, int n, int k){
	int i, tmp;
	int c1, t=20000;
	c1=0;

	for(i=0;i<t;i++){
		tmp = simulZipf(n, F);
		if(tmp<=k)
			c1++;
	}
	printf("\nc1=%d", c1);
	printf("\nt=%d", t);
	printf("\nc1/t=%lf", (double)c1/t);
}

int simulZipf(int n, double *F)
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
     		fprintf(f,"%.6lf %.6lf\n", 
     			log(film[i].rank),
     			-log(film[i].pre)
     			);
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

int cmpFilm(const void *pf1,const  void *pf2)
{
    Film *p1=(Film *)pf1, *p2=(Film *)pf2;
    return (p2->nrv)-(p1->nrv);
}
