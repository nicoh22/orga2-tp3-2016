#include "game.h"
#include "mmu.h"
#include "tss.h"
#include "screen.h"
#include "sched.h"
#include "defines.h"

extern unsigned int globalDebug;
void controlDebugMode();

unsigned short manejar_syscall(unsigned int syscall, unsigned int param1, unsigned int param2){
	enLaIdle = 1;
   	switch(syscall){
		case 0x124: game_donde( (unsigned int*) param1); break;
		case 0xA6A: game_soy(param1); break;
		case 0XFF3: game_mapear(param1, param2); break;
		default:
			//ERROR, deberia desalojarse la tarea por atrevida
			sched_desalojar_actual();
			return 1;
	}
	 return 0; //a lo unix, esta todo bien
}


void actualizar_puntos(){
	jugadores[0].puntos = 0;
	jugadores[1].puntos = 0;
	int i,j;
		for(i = 0; i<3; i++){
			for(j = 0; j<=task_max_index(i); j++){
				task_info* info = &tareasInfo[i][j];
				if(info->alive){
					// Usamos el offset -1 porque owner es task_type,
					// Que incluye el a las tareas sanas como indice 0
					jugadores[info->owner-1].puntos++;
				}
			}
		}
}

void game_tick(){
	// El ciclo de actualización de la pantalla sigue este orden:
	// Actualizar reloj tarea
	// Pintar tareas
	// Pintar paginas mapeadas
	// Pintar jugadores
	// Mostrar pantalla de debug
	// Actualizar puntos
	// Actualizar vidas
	
	task_info* tarea_actual = sched_tarea_actual();

	//En el ciclo de clock actual solo pintamos el clock que corresponde
	screen_actualizar_reloj_tarea(sched_tipo_actual(), sched_indice_actual(), tarea_actual->alive, tarea_actual->owner);
	short i,j;

	for(i = 1; i<3; i++){
		for(j = 0; j<=task_max_index(i); j++){
			task_info* info = &tareasInfo[i][j];
			if(info->alive){
				screen_pintar_mapeo_tarea(info->owner,
					info->mapped_x,
					info->mapped_y);
			}else{
				screen_limpiar_pixel(info->mapped_x,
					info->mapped_y);
			}
		}
	}



	for(i = 0; i<3; i++){
		for(j = 0; j<=task_max_index(i); j++){
			task_info* info = &tareasInfo[i][j];
			if(info->alive){
				screen_pintar_tarea(info->owner,
					info->x,
					info->y);
			}else{
				screen_limpiar_pixel(info->x,
									info->y);
				screen_actualizar_reloj_tarea(i, j, 0, info->owner);
			}
		}
	}





	actualizar_puntos();

	screen_pintar_jugador(0, jugadores[0].x, jugadores[0].y);
	screen_pintar_jugador(1, jugadores[1].x, jugadores[1].y);

	screen_actualizar_puntos( jugadores[0].puntos, jugadores[1].puntos);
	screen_actualizar_vidas( jugadores[0].tareas_restantes, jugadores[1].tareas_restantes);
	
//	screen no tiene que conocer nada del juego, las tareas
//	se le tiene que pasar todo como parametro-
//	Si no, rompemos encapsulamiento.

}

#define KB_w        0x11 // 0x91
#define KB_s        0x1f // 0x9f
#define KB_a        0x1e // 0x9e
#define KB_d        0x20 // 0xa0

#define KB_i        0x17 // 0x97
#define KB_k        0x25 // 0xa5
#define KB_j        0x24 // 0xa4
#define KB_l        0x26 // 0xa6

#define KB_shiftL   0x2a // 0xaa
#define KB_shiftR   0x36 // 0xb6

#define KB_y        0x15 // 

// ~~~ debe atender la interrupción de teclado, se le pasa la tecla presionada
void atender_teclado(unsigned char tecla){
	
	switch (tecla){
		// 0 para jugadorA, 1 para jugadorB
		
		case KB_w: game_mover_cursor(0, ARB); break;
		case KB_a: game_mover_cursor(0, IZQ); break;
		case KB_s: game_mover_cursor(0, ABA); break;
		case KB_d: game_mover_cursor(0, DER); break;

		case KB_i: game_mover_cursor(1, ARB); break;
		case KB_j: game_mover_cursor(1, IZQ); break;
		case KB_k: game_mover_cursor(1, ABA); break;
		case KB_l: game_mover_cursor(1, DER); break;

		case KB_shiftL: game_lanzar(0); break;
		case KB_shiftR: game_lanzar(1); break;
		
		case KB_y: controlDebugMode(); break;
		default: break;
	}
}

void controlDebugMode() {
/*
	if (debugState == enableDebug) {
		debugState = disableDebug;
	}else if (debugState == disableDebug) {
		debugState = enableDebug;
	}
*/
}
