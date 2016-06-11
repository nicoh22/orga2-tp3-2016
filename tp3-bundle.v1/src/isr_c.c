#include "game.h"
#include "mmu.h"
#include "tss.h"
#include "screen.h"
#include "defines.h"

extern unsigned int globalDebug;

unsigned int globalDebug = FALSE;



unsigned short manejar_syscall(unsigned int syscall, unsigned int param1, unsigned int param2){
   	switch(syscall){
		case 0x124: game_donde( (unsigned int*) param1); break;
		case 0xA6A: game_soy(param1); break;
		case 0XFF3: game_mapear(param1, param2); break;
		default: return 1; //ERROR, deberia desalojarse la tarea por atrevida
	}
	 return 0; //a lo unix, esta todo bien
}


void game_atender_tick(){

//	screen_actualizar_reloj_tarea(tarea_actual);
	
	//actualizar cosas del game si corresponde
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
void game_atender_teclado(unsigned char tecla){
	
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

		case KB_y: globalDebug = !globalDebug; break;
		default: break;
	}

}

