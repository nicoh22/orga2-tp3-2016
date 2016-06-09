#include "sched.h"
#include <stdio.h>
#include <stdlib.h>

int main(void){
	sched_lanzar_tareas(0,0x500000);
	unsigned int indiceGDT = sched_proximo_indice();
	printf("%d",indiceGDT);
}
