/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#ifndef __GAME_H__
#define __GAME_H__

#include "defines.h"
#include "sched.h"
#include "screen.h"
#include "mmu.h"


typedef enum direccion_e { IZQ = 0xAAA, DER = 0x441, ARB = 0xA33, ABA = 0x883  } direccion;

typedef struct jugador_t{
	unsigned int tareas_restantes;
	unsigned short x;
	unsigned short y;
	unsigned short id;
	unsigned short puntos;
} jugador;

jugador jugadores[2];

void game_lanzar(unsigned int jugador);

void game_soy(unsigned int soy);

void game_donde(unsigned int* pos);

void game_mapear(int x, int y);

void game_mover_cursor(int jugador, direccion dir);

void game_inicializar();

unsigned int xytofisica( unsigned short x, unsigned short y );

#endif  /* !__GAME_H__ */
