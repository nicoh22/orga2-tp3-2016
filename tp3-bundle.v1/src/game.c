/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"

void game_inicializar(){ 
	//inicializa estructuras del game
	//Jugadores, puntos, virus, etc

	jugadores[0].tareas_restantes = 20;
	jugadores[0].x = 0;// Totalmente arbitrario
	jugadores[0].y = 0;
	jugadores[0].id = 0;
	jugadores[0].puntos = 0;
	
	jugadores[1].tareas_restantes = 20;
	jugadores[1].x = 79;
	jugadores[1].y = 43;
	jugadores[1].id = 1;
	jugadores[1].puntos = 0;
	
}

void game_mover_cursor(int index_jugador, direccion dir) {
	jugador* jugador_actual = &jugadores[index_jugador];
	
	screen_limpiar_posicion(jugador_actual->x, jugador_actual->y); 
	//Esta funcion implica control directo de un pixel 
	//por eso el ajuste respecto a las coordenadas del mapa
	
	switch(dir){
// La memoria de video labura asi:
// x crece de izquierda a derecha
// y crece de arriba hacia abajo (al reves de ejes cartesianos)
		case ARB:
			if( jugadores[index_jugador].y != 0 ){
				jugadores[index_jugador].y--; 
			}
			break;
		case ABA: 
			if( jugadores[index_jugador].y != 43 ){
				jugadores[index_jugador].y++; 
			}
			break;
		case IZQ: 
			if( jugadores[index_jugador].x != 0 ){ 
				jugadores[index_jugador].x--; 
			}
			break;
		case DER: 
			if ( jugadores[index_jugador].x != 79 ){ 
				jugadores[index_jugador].x++; 
			}
			break;
	}
	
}

void game_lanzar(unsigned int index_jugador) {
	jugador* jugador_actual = &jugadores[index_jugador];
	if ( jugador_actual->tareas_restantes > 0 &&
		 (sched_proximo_slot_tarea_libre(index_jugador + 1)  != -1) )
	{ 
		jugador_actual->tareas_restantes--;
		sched_lanzar_tareas(index_jugador + 1, 
							jugador_actual->x, 
							jugador_actual->y);
	} 
	else {
		return; // Jugador no puede lanzar mas tareas
	}
}
#define VIRUS_ROJO 0x841
#define VIRUS_AZUL 0x325

void game_soy(unsigned int yoSoy) {
	task_info* tarea_actual = sched_tarea_actual();
	switch(yoSoy){
		case VIRUS_ROJO: tarea_actual->owner = A_type; break;
		case VIRUS_AZUL: tarea_actual->owner = B_type; break;
		default: tarea_actual->owner = H_type; break;
	}
}

void game_donde(unsigned int* pos) {
	task_info* tarea_actual = sched_tarea_actual();
	pos[0] = (int) tarea_actual->x;
	pos[1] = (int) tarea_actual->y;
}

void game_mapear(int x, int y) {
	unsigned int fisica = xytofisica(x, y);
	if(fisica == NULL){	
		//error en los checks del mapa
		sched_desalojar_actual();
		return;
	}
	unsigned int cr3Tarea = rcr3();
	mmu_mapear_pagina(EXTRA_PAGE, cr3Tarea, fisica, ATTR_USER);
	task_info* tareaActual = sched_tarea_actual();
	screen_limpiar_posicion(tareaActual->mapped_x, tareaActual->mapped_y);
	tareaActual->mapped_x = x;
	tareaActual->mapped_y = y;

}

unsigned int xytofisica( unsigned short x, unsigned short y ){
	//Cito enunciado:
	//Cada tarea tendra asignada inicialmente una sola pagina de memoria de 4kb,
	//esta pagina estara ubicada dentro del area de memoria denominada el mapa.
	//Este correspondera a un area de memoria fısica de 80 × 44 paginas de 4kb

	if( ( x > 79 ) | ( y > 43 ) ){
		//el par (x, y) no corresponde a una posicion valida del mapa
		return NULL; 
	}	
	
	return BASE_MAP + ( 80*y  + x ) * PAGE_SIZE;
}
