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

char *tareaH = "Tarea H";
char *tareaA = "Tarea A";
char *tareaB = "Tarea B";

unsigned int rand_in_range(unsigned int min, unsigned int max){
	return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

unsigned short task_max_index(short current){
	if(current == 0){
		return 14;
	}else{
		return 4;
	}
}

void print_saltando(){
	unsigned short attr;
	switch(currentType){
				case A_type: attr = C_FG_RED | C_BG_BLACK; break;
				case B_type: attr = C_FG_BLUE | C_BG_BLACK; break;
				default: attr = C_FG_WHITE | C_BG_BLACK; break;
			}
				print_int(currentType,11,0, attr);
				print(":  ",12,0, attr);
				print_int(currentIndex,14,0, attr);
}

void sched_init(){
    debugState = disableDebug;

	enLaIdle = 1;
	indicesInicializados = 0;
	currentType = 0;
	currentIndex = 0;
	int i, j;	
	for(i = 0; i<3; i++){
		for(j = 0; j<=task_max_index(i); j++){
			tareasInfo[i][j].alive = 0;
			tareasInfo[i][j].owner = 0;
			tareasInfo[i][j].gdtIndex = 0;
			tareasInfo[i][j].x = 0;
			tareasInfo[i][j].y = 0;
			tareasInfo[i][j].mapped_x = 0;
			tareasInfo[i][j].mapped_y = 0;
		}
		taskIndices[i] = 0;
	}

	srand(0);
	for(i = 0; i<15; i++){
		unsigned short y = rand_in_range(0,43);
		unsigned short x = rand_in_range(0,79);
		sched_lanzar_tareas(0,x,y);
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

int sched_proximo_slot_tarea_libre(taskType tipo){
	int freeTaskIndex = -1;
	int i = 0;
	for(i = 0; i<=task_max_index(tipo); i++){
		if(!tareasInfo[tipo][i].alive){
			freeTaskIndex = i;
			break;
		}
	}
	return freeTaskIndex;
}

void sched_lanzar_tareas(taskType tipo, unsigned short x, unsigned short y ){
	unsigned int fisica = xytofisica(x,y);
	// Busca el primer espacio en los arreglos de tareas
	int taskIndex = sched_proximo_slot_tarea_libre(tipo); 
	// Si no encontramos lugar para poner la nueva tarea no hacemos nada
	if(taskIndex >= 0){
		// ultimoGdtKernel + 15 sanas + 5 A + 5 B
		int gdtIndex = GDT_TASK_INDICES_START + getTypeGdtOffset(tipo) + taskIndex;
		tss_crear_tarea(tipo, taskIndex, gdtIndex, fisica);
		tareasInfo[tipo][taskIndex].alive = 1;
		tareasInfo[tipo][taskIndex].x = x;
		tareasInfo[tipo][taskIndex].y = y;
		tareasInfo[tipo][taskIndex].mapped_x = x;
		tareasInfo[tipo][taskIndex].mapped_y = y;
		tareasInfo[tipo][taskIndex].owner = tipo;
		tareasInfo[tipo][taskIndex].gdtIndex = gdtIndex;
		tareasInfo[tipo][taskIndex].index = taskIndex;
	}
}



unsigned short sched_proximo_indice() {

    // Estoy en la idle y previamente habia ocurrido una excepcion. No salto.
    if (enLaIdle && debugState == enableDebugIntr) {
        return 0;
    }

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

				////////////////// DEBUG /////////////////////
				
				print_saltando();

				////////////////// DEBUG /////////////////////


				return info.gdtIndex;
			}
			nextIndex++;
			if(nextIndex > task_max_index(nextType)){
				nextIndex = 0;
			}
		}
		
		if( tareasInfo[nextType][currentIndexForType].alive ){
			currentType = nextType;
			currentIndex = currentIndexForType;
			print_saltando();
			if( nextType == A_type){
			//	breakpoint();	
				
			}
			return tareasInfo[nextType][currentIndexForType].gdtIndex;
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


unsigned int sched_desalojar_actual(){
	task_info* actual = sched_tarea_actual();

	///// DEBUG //////
	unsigned short attr = C_FG_WHITE | C_BG_BLACK;
	switch(currentType){
		case A_type: attr = C_FG_RED | C_BG_BLACK; break;
		case B_type: attr = C_FG_BLUE | C_BG_BLACK; break;
		default: attr = C_FG_WHITE | C_BG_BLACK; break;
	}
	print("DES:",20,0, attr);
	print_int(currentIndex,26,0, attr);
	///// DEBUG //////

	// Solo bajamos el flag alive
	// Podemos usar el resto de la informacion para limpiar la pantalla
	actual->alive = 0;
	enLaIdle = 1;
	return 0;
}

void sched_set_enLaIdle(){
	enLaIdle = 1;
}

task_info* sched_tarea_actual(){
	return &tareasInfo[currentType][currentIndex];
}

char* sched_tarea_actual_owner(){
	short type = sched_tipo_actual();
	
	if (type == H_type){
	    return tareaH;
	}else if(type == A_type) {
	    return tareaA;
	}
	    
	return tareaB;
	
}

short sched_tipo_actual(){
	return currentType;
}

short sched_indice_actual(){
	return currentIndex;
}

