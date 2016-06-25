/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCREEN_H__
#define __SCREEN_H__

/* Definicion de la pantalla */
#define VIDEO_FILS 50
#define VIDEO_COLS 80

#include "colors.h"
#include "defines.h"
#include "i386.h"
extern void print(const char * text, unsigned int x, unsigned int y, unsigned short attr);

/* Estructura de para acceder a memoria de video */
typedef struct ca_s {
    unsigned char c;
    unsigned char a;
} char_attr;

void print(const char * text, unsigned int x, unsigned int y, unsigned short attr);

void print_hex(unsigned int numero, int size, unsigned int x, unsigned int y, unsigned short attr);

void print_int(unsigned int n, unsigned int x, unsigned int y, unsigned short attr);

void screen_pintar_mapa();

void screen_pintar_tarea(taskType id, unsigned short x, unsigned short y);

void screen_pintar_mapeo_tarea(taskType id, unsigned short x, unsigned short y);

void screen_pintar_jugador(char id, unsigned short x, unsigned short y);

void screen_limpiar_posicion(unsigned short x, unsigned short y);

extern void inicializar_interfaz();

extern void print_alligned_right(const char * text);

extern void imprimir_log(unsigned int *esp);

void screen_actualizar_reloj_tarea( taskType tipo, unsigned short indice, unsigned char alive, taskType owner );
void screen_actualizar_puntos(unsigned short puntosA, unsigned short puntosB);
void screen_actualizar_vidas(unsigned short vidasA, unsigned short vidasB);

#endif  /* !__SCREEN_H__ */
