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
	// TODO: check de limites del mapa.
	// en lugar de limpiar el pixel donde estaba,
	// hacer otra cosa (Referirse al Readme.md)
	
	jugador* jugador_actual = &jugadores[index_jugador];
	
	screen_limpiar_pixel(jugador_actual->x, jugador_actual->y);
	
	switch(dir){
// La memoria de video labura asi:
// x crece de izquierda a derecha
// y crece de arriba hacia abajo (al reves de ejes cartesianos)
		case ARB: jugadores[index_jugador].y--; break;
		case ABA: jugadores[index_jugador].y++; break;
		case IZQ: jugadores[index_jugador].x--; break;
		case DER: jugadores[index_jugador].x++; break;
	}
	
	screen_pintar_jugador(
			jugador_actual->id, 
			jugador_actual->x, 
			jugador_actual->y
		);
	

}

void game_lanzar(unsigned int jugador) {
}

void game_soy(unsigned int yoSoy) {
}

void game_donde(unsigned int* pos) {
}

void game_mapear(int x, int y) {
}

