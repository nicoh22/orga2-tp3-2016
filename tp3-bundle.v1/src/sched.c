/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"
#include "rand.c"

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
unsigned short indicesInicializados;

unsigned int rand_in_range(unsigned int min, unsigned int max){
	return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

void sched_lanzar_tareas(taskType tipo, unsigned int fisica );

unsigned short task_max_index(short current){
	if(current == 0){
		return 14;
	}else{
		return 4;
	}
}

void sched_init(){
	enLaIdle = 1;
	indicesInicializados = 0;
	currentType = 0;
	currentIndex = 0;
	int i, j;	
	for(i = 0; i<3; i++){
		for(j = 0; j<task_max_index(i); j++){
			tareasInfo[i][j].alive = 0;
			tareasInfo[i][j].owner = 0;
			tareasInfo[i][j].gdtIndex = 0;
		}
		taskIndices[i] = 0;
	}
	// Area de la pantalla: desde 0x400000 a 0x11BF000
	// Hacemos un random entre 0x400 y 0x11BF
	// Y luego shifteamos 12 bits
	srand(0);
	for(i = 0; i<15; i++){
		unsigned int fisica = rand_in_range(0x400,0x11BF);
		fisica = fisica << 12;
		sched_lanzar_tareas(0,fisica);
	}
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
	for(i = 0; i<task_max_index(tipo); i++){
		if(!tareasInfo[tipo][i].alive){
			freeTaskIndex = i;
			break;
		}
	}
	return freeTaskIndex;
}

void sched_lanzar_tareas(taskType tipo, unsigned int fisica ){
	// Busca el primer espacio en los arreglos de tareas
	int taskIndex = getNextFreeIndex(tipo); 
	// Si no encontramos lugar para poner la nueva tarea no hacemos nada
	if(taskIndex >= 0){
		// ultimoGdtKernel + 15 sanas + 5 A + 5 B
		int gdtIndex = GDT_TASK_INDICES_START + getTypeGdtOffset(tipo) + taskIndex;
		tss_crear_tarea(tipo, taskIndex, gdtIndex, fisica);
		tareasInfo[tipo][taskIndex].alive = 1;
		tareasInfo[tipo][taskIndex].owner = tipo;
		tareasInfo[tipo][taskIndex].gdtIndex = gdtIndex;
		tareasInfo[tipo][taskIndex].index = taskIndex;
	}
}



unsigned short sched_proximo_indice() {
	// Manejamos el caso borde de que tenemos que saltar a la primer tarea
	if(!indicesInicializados){
		indicesInicializados = 1;
		// Saltamos a la primer tarea sana al empezar una partida
		// siempre que exista. Sino nos quedamos en la idle
		task_info info = tareasInfo[0][0];
		if(info.alive){
			enLaIdle = 0;
			return info.gdtIndex;
		}else{
			return 0;
		}
	}

	// Buscamos una tarea viva de alguno de los tipos siguientes	
	unsigned short nextType = currentType + 1; 
	if(nextType > 2){
		nextType = 0;
	}	
	

	// Iteramos 3 veces para volver a considerar el tipo actual si no
	// encontramos tareas de otros tipos
	int i = 0;
	while(i < 3){

		// Para cada tipo loopeamos en los indices a partir del ultimo
		// en el que quedamos
		unsigned short currentIndexForType = taskIndices[nextType];
		unsigned short nextIndex = taskIndices[nextType] + 1;

		if(nextIndex > task_max_index(nextType)){
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
			if(nextIndex > task_max_index(nextType)){
				nextIndex = 0;
			}
		}
		
		nextType++;
		if(nextType > 2){
			nextType = 0;
		}

		i++;
	}
	
	// Si no encontramos otra tarea viva a la que saltar, no saltamos
	// => devolvemos 0
	if(enLaIdle){
		task_info info = tareasInfo[currentType][currentIndex];
		if(info.alive){
			enLaIdle = 0;
			return info.gdtIndex;
		}
	}
	return 0;
}

void sched_set_enLaIdle(){
	enLaIdle = 1;
}

task_info* sched_tarea_actual(){
	return &tareasInfo[currentType][currentIndex];
}

void sched_desalojar_tarea(){
	//TODO
	//Se le pasa como parametro la tarea
	//o asumimos que queremos desalojar la actual?
}
