#include <stdio.h>

void fct(){
	static int tmp;
	printf("%d\n", tmp);
	tmp = 12;
}

int main(){
	char c;

	while(1){
		c=getche();
		//printf("%c", c);
		fct();
	}

	return 0;
}