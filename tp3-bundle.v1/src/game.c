/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"

jugador jugadores[2];

void game_inicializar(){ 
	//inicializa estructuras del game
	//Jugadores, puntos, virus, etc

	jugadores[0].tareas_restantes = 20;
	jugadores[0].x = 1;// Totalmente arbitrario
	jugadores[0].y = 1;
	jugadores[0].id = 0;
	
	jugadores[1].tareas_restantes = 20;
	jugadores[1].x = 78;
	jugadores[1].y = 43;
	jugadores[1].id = 1;
}

void game_mover_cursor(int index_jugador, direccion dir) {
	// TODO: 
	// en lugar de limpiar el pixel donde estaba,
	// hacer otra cosa (Referirse al Readme.md)
	
	jugador* jugador_actual = &jugadores[index_jugador];
	
	screen_limpiar_pixel(jugador_actual->x, jugador_actual->y);
	
	switch(dir){
// La memoria de video labura asi:
// x crece de izquierda a derecha
// y crece de arriba hacia abajo (al reves de ejes cartesianos)
		case ARB:
			if( jugadores[index_jugador].y != 1 ){
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
	
	screen_pintar_jugador(
			jugador_actual->id, 
			jugador_actual->x, 
			jugador_actual->y
		);
	

}

void game_lanzar(unsigned int jugador) {
//TODO: completar	
	if ( jugadores[jugador].tareas_restantes > 0){ 
		jugadores[jugador].tareas_restantes--;
	} 
	else {
		return; // Jugador no puede lanzar mas tareas
	}
}

void game_soy(unsigned int yoSoy) {
}

void game_donde(unsigned int* pos) {
}

void game_mapear(int x, int y) {
}

