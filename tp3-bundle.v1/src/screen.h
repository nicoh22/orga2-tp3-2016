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

extern void print(const char * text, unsigned int x, unsigned int y, unsigned short attr);

/* Estructura de para acceder a memoria de video */
typedef struct ca_s {
    unsigned char c;
    unsigned char a;
} ca;

void print(const char * text, unsigned int x, unsigned int y, unsigned short attr);

void print_hex(unsigned int numero, int size, unsigned int x, unsigned int y, unsigned short attr);

void print_int(unsigned int n, unsigned int x, unsigned int y, unsigned short attr);


void screen_pintar_jugador(char id, unsigned short x, unsigned short y);

void screen_limpiar_pixel(unsigned short x, unsigned short y);

extern void inicializar_interfaz();

extern void print_alligned_right(const char * text);

#endif  /* !__SCREEN_H__ */
