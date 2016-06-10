#include "sched.h"
#include <stdio.h>
#include <stdlib.h>

int main(void){
	sched_init();
	unsigned int indiceGDT = sched_proximo_indice();
	printf("%d\n",indiceGDT);
	return 0;
}
