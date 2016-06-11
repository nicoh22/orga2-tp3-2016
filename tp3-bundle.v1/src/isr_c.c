#include "game.h"
#include "mmu.h"
#include "tss.h"
#include "screen.h"
#include "defines.h"

extern unsigned int globalDebug;

unsigned int globalDebug = FALSE;



uint game_syscall_manejar(uint syscall, uint param1)
{
   	uint res;	
 // ~ completar llamando a las funciones que haga falta ~
   	switch(syscall){
		case 1: res = game_perro_mover(game_perro_actual, param1); break;
		case 2: res = game_perro_cavar(game_perro_actual); break;
		case 3: res = game_perro_olfatear(game_perro_actual); break;
		case 4: res = (uint) game_perro_recibirOrden(game_perro_actual); break;
		default: return -1;
	}
	 return res;
}

// ~~~ debe atender la interrupción de reloj para actualizar la pantalla y terminar si es hora,
// ~~~ recibe el perro que está corriendo actualmente

/*
void game_atender_tick(perro_t *perro){
	//pintar? capaz todas las actualizaciones(perro, jugador, se hacen aca)
	screen_actualizar_reloj_perro(perro);
	game_terminar_si_es_hora();
}
*/



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

#define KB_y        0x15 // ?



// ~~~ debe atender la interrupción de teclado, se le pasa la tecla presionada
void game_atender_teclado(unsigned char tecla)
{
	switch (tecla)
	{
		// ~~~ completar ~~~
		
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

