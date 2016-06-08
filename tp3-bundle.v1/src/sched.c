/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"

// primer indice: tipo de tarea
//		0 : H
//		1 : A
//		2 : B
// segundo indice: nro de tarea
task_info tareasInfo[3][15];

unsigned short taskIndices[3];

unsigned short currentType;
unsigned short currentIndex;

unsigned short enLaIdle;

unsigned short getMaxIndex(unsigned short current){
	if(current == 0){
		return 14;
	}else{
		return 4;
	}
}

void sched_init(){
	enLaIdle = 0;
	currentType = 0;
	currentIndex = 0;
	int i, j;	
	for(i = 0; i<3; i++){
		for(j = 0; j<getMaxIndex(i); j++){
			tareasInfo[i][j].alive = 0;
			tareasInfo[i][j].owner = 0;
			tareasInfo[i][j].gdtIndex = 0;
		}
	}
	
	//TODO: Lanzar tareas sanas
	// rand x e y -> funcion que toma x,y : fisica
}

int getTypeGdtOffset(unsigned short tipo){
	if(tipo == 0){
		return 0;
	}else if(tipo==1){
		return 15;
	}else{
		return 20;
	}
}

void sched_lanzar_tareas(taskType tipo, unsigned int fisica ){
	// Busca el primer espacio en los arreglos de tareas
	int taskIndex = getNextFreeIndex(tipo-1); // -1 para ignorar la idle
	// ultimoGdtKernel + 15 sanas + 5 A + 5 B
	int gdtIndex = GDT_TASK_INDICES_START + getTypeGdtOffset(tipo-1) + taskIndex;
	tss_crear_tarea(tipo, gdtIndex, fisica);
	tareasInfo[tipo-1][taskIndex].alive = 1;
	tareasInfo[tipo-1][taskIndex].tipo = tipo-1;
	tareasInfo[tipo-1][taskIndex].gdtIndex = gdtIndex;
}

unsigned short sched_proximo_indice() {

	// Buscamos una tarea viva de alguno de los tipos siguientes	
	unsigned short nextType = currentType++; 
	if(nextType > 2){
		nextType = 0;
	}	

	while(nextType != currentType){

		// Para cada tipo loopeamos en los indices a partir del ultimo
		// en el que quedamos
		unsigned short currentTypeIndex = taskIndices[nextType];
		unsigned short nextIndex = taskIndices[nextType]++;
		if(nextIndex > maxIndex(nextType)){
			nextIndex = 0;
		}

		while(nextIndex != currentTypeIndex){
			task_info info = tareasInfo[nextType][nextIndex];
			if(info.alive){
				currentType = nextType;
				currentIndex = nextIndex;
				return info.gdtIndex;
			}
			nextIndex++;
			if(nextIndex > maxIndex(nextType)){
				nextIndex = 0;
			}
		}

		if(nextType > 2){
			nextType = 0;
		}
	}
	
	// Si no encontramos otra tarea viva a la que saltar, no saltamos
	// => devolvemos 0
	return 0;
}

