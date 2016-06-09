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

short currentType;
short currentIndex;

unsigned short enLaIdle;

void sched_lanzar_tareas(taskType tipo, unsigned int fisica );

unsigned short getMaxIndex(short current){
	if(current == 0){
		return 14;
	}else{
		return 4;
	}
}

void sched_init(){
	enLaIdle = 1;
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
	
	sched_lanzar_tareas(0,0x500000);
	//TODO: Lanzar tareas sanas
	// rand x e y -> funcion que toma x,y : fisica
}

int getTypeGdtOffset(taskType tipo){
	if(tipo == 0){
		return 0;
	}else if(tipo==1){
		return 15;
	}else{
		return 20;
	}
}

int getNextFreeIndex(taskType tipo){
	int freeTaskIndex = -1;
	int i = 0;
	for(i = 0; i<getMaxIndex(tipo); i++){
		if(!tareasInfo[tipo][i].alive){
			freeTaskIndex = i;
		}
	}
	return freeTaskIndex;
}

void sched_lanzar_tareas(taskType tipo, unsigned int fisica ){
	// Busca el primer espacio en los arreglos de tareas
	int taskIndex = getNextFreeIndex(tipo); 
	// Si no encontramos lugar para poner la nueva tarea no hacemos nada
	if(taskIndex < 0){
		// ultimoGdtKernel + 15 sanas + 5 A + 5 B
		int gdtIndex = GDT_TASK_INDICES_START + getTypeGdtOffset(tipo) + taskIndex;
		tss_crear_tarea(tipo, gdtIndex, fisica);
		tareasInfo[tipo][taskIndex].alive = 1;
		tareasInfo[tipo][taskIndex].owner = tipo;
		tareasInfo[tipo][taskIndex].gdtIndex = gdtIndex;
	}
}

unsigned short sched_proximo_indice() {
	// Buscamos una tarea viva de alguno de los tipos siguientes	
	unsigned short nextType = currentType + 1; 
	if(nextType > 2){
		nextType = 0;
	}	
	
	if(enLaIdle){
		// Si estamos en la idle *siempre* vamos a querer saltar a otra
		// tarea, aun si es la misma que estabamos corriendo
		// (caso de syscall desde la unica tarea viva)
		currentType = -1;
		enLaIdle = 0;
	}


	while(nextType != currentType){

		// Para cada tipo loopeamos en los indices a partir del ultimo
		// en el que quedamos
		unsigned short currentIndexForType = taskIndices[nextType];
		unsigned short nextIndex = taskIndices[nextType] + 1;
		if(nextIndex > getMaxIndex(nextType)){
			nextIndex = 0;
		}

		while(nextIndex != currentIndexForType){
			task_info info = tareasInfo[nextType][nextIndex];
			if(info.alive){
				taskIndices[nextType] = nextIndex;
				currentType = nextType;
				currentIndex = nextIndex;
				return info.gdtIndex;
			}
			nextIndex++;
			if(nextIndex > getMaxIndex(nextType)){
				nextIndex = 0;
			}
		}
		
		nextType++;
		if(nextType > 2){
			nextType = 0;
		}
	}
	
	// Si no encontramos otra tarea viva a la que saltar, no saltamos
	// => devolvemos 0
	return 0;
}

