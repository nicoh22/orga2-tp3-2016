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

unsigned short tareasIndices[3];

short currentType;
short currentIndex;

unsigned short en_idle;

char *tareaH = "Tarea H";
char *tareaA = "Tarea A";
char *tareaB = "Tarea B";

unsigned int rand_in_range(unsigned int min, unsigned int max) {
	return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

unsigned short task_type_max(short current) {
	if (current == 0) {
		return 15;
	} else {
		return 5;
	}
}

void print_saltando() {
	unsigned short attr;
	switch (currentType) {
	case A_type:
		attr = C_FG_RED | C_BG_BLACK;
		break;
	case B_type:
		attr = C_FG_BLUE | C_BG_BLACK;
		break;
	default:
		attr = C_FG_WHITE | C_BG_BLACK;
		break;
	}
	print_int(currentType, 11, 0, attr);
	print(":  ", 12, 0, attr);
	print_int(currentIndex, 14, 0, attr);
}

void sched_init() {
	debugState = disableDebug;

	en_idle = 1;
	currentType = 0;
	currentIndex = 0;
	int i, j;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < task_type_max(i); j++) {
			tareasInfo[i][j].alive = 0;
			tareasInfo[i][j].owner = 0;
			tareasInfo[i][j].gdtIndex = 0;
			tareasInfo[i][j].x = 0;
			tareasInfo[i][j].y = 0;
			tareasInfo[i][j].mapped_x = 0;
			tareasInfo[i][j].mapped_y = 0;
		}
		tareasIndices[i] = 0;
	}

	srand(0);
	for (i = 0; i < 15; i++) {
		unsigned short y = rand_in_range(0, 43);
		unsigned short x = rand_in_range(0, 79);
		sched_lanzar_tareas(0, x, y);
	}
}

int task_type_gdt_offset(taskType tipo) {
	if (tipo == 0) {
		return 0;
	} else if (tipo == 1) {
		return 15;
	} else {
		return 20;
	}
}

int sched_proximo_slot_tarea_libre(taskType tipo) {
	int freeTaskIndex = -1;
	int i = 0;
	for (i = 0; i < task_type_max(tipo); i++) {
		if (!tareasInfo[tipo][i].alive) {
			freeTaskIndex = i;
			break;
		}
	}
	return freeTaskIndex;
}

void sched_lanzar_tareas(taskType tipo, unsigned short x, unsigned short y) {
	unsigned int fisica = xytofisica(x, y);
	// Busca el primer espacio en los arreglos de tareas
	int taskIndex = sched_proximo_slot_tarea_libre(tipo);
	// Si no encontramos lugar para poner la nueva tarea no hacemos nada
	if (taskIndex >= 0) {
		// ultimoGdtKernel + 15 sanas + 5 A + 5 B
		int gdtIndex = GDT_TASK_INDICES_START + task_type_gdt_offset(tipo) + taskIndex;
		tss_crear_tarea(tipo, taskIndex, gdtIndex, fisica);
		tareasInfo[tipo][taskIndex].alive = 1;
		tareasInfo[tipo][taskIndex].x = x;
		tareasInfo[tipo][taskIndex].y = y;
		tareasInfo[tipo][taskIndex].mapped_x = x;
		tareasInfo[tipo][taskIndex].mapped_y = y;
		tareasInfo[tipo][taskIndex].owner = tipo;
		tareasInfo[tipo][taskIndex].gdtIndex = gdtIndex;
	}
}

unsigned short sched_proximo_indice() {

	// Estoy en la idle y previamente habia ocurrido una excepcion. No salto.
	if (en_idle && debugState == enableDebugIntr) {
		return 0;
	}

	// Buscamos una tarea viva de alguno de los tipos siguientes	
	unsigned short nextType = ((currentType + 1) % 3);

	// Iteramos 3 veces para volver a considerar el tipo actual si no
	// encontramos tareas de otros tipos
	int i = 0;
	while (i < 3) {
		// Para cada tipo loopeamos en los indices a partir del ultimo
		// en el que quedamos
		unsigned short currentIndexForType = tareasIndices[nextType];
		unsigned short nextIndex = (tareasIndices[nextType] + 1) % task_type_max(nextType) ;
		while (nextIndex != currentIndexForType) {
			task_info info = tareasInfo[nextType][nextIndex];
			if (info.alive) {
				en_idle = 0;
				tareasIndices[nextType] = nextIndex;
				currentType = nextType;
				currentIndex = nextIndex;
				return info.gdtIndex;
			}
			nextIndex = (nextIndex + 1) % task_type_max(nextType) ;
		}

		// Si la única tarea del tipo siguiente es la última que se ejecuto,
		// saltamos a esa de todas formas.
		// Como iteramos 3 veces para considerar el tipo actual: si hay solo 1 tarea de 1 tipo
		// no saltamos.
		if (tareasInfo[nextType][currentIndexForType].alive && nextType != currentType) {
			en_idle = 0;
			currentType = nextType;
			currentIndex = currentIndexForType;
			return tareasInfo[nextType][currentIndexForType].gdtIndex;
		}

		nextType = ((nextType + 1) % 3);
		i++;
	}

	// Si no encontramos otra tarea viva a la que saltar, no saltamos
	// => devolvemos 0
	if (en_idle) {
		task_info info = tareasInfo[currentType][currentIndex];
		if (info.alive) {
			en_idle = 0;
			return info.gdtIndex;
		}
	}
	return 0;
}

unsigned int sched_desalojar_actual() {
	task_info* actual = sched_tarea_actual();

	///// DEBUG //////
	unsigned short attr = C_FG_WHITE | C_BG_BLACK;
	switch (currentType) {
	case A_type:
		attr = C_FG_RED | C_BG_BLACK;
		break;
	case B_type:
		attr = C_FG_BLUE | C_BG_BLACK;
		break;
	default:
		attr = C_FG_WHITE | C_BG_BLACK;
		break;
	}
	print("DES:", 20, 0, attr);
	print_int(currentIndex, 26, 0, attr);
	///// DEBUG //////

	// Solo bajamos el flag alive
	// Podemos usar el resto de la informacion para limpiar la pantalla
	actual->alive = 0;
	en_idle = 1;
	return 0;
}

void sched_set_enLaIdle() {
	en_idle = 1;
}

task_info* sched_tarea_actual() {
	return &tareasInfo[currentType][currentIndex];
}

char* sched_tarea_actual_owner() {
	short type = sched_tipo_actual();

	if (type == H_type) {
		return tareaH;
	} else if (type == A_type) {
		return tareaA;
	}

	return tareaB;

}

short sched_tipo_actual() {
	return currentType;
}

short sched_indice_actual() {
	return currentIndex;
}

