/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "screen.h"

#define CLOCK_BASE_H 5
#define CLOCK_BASE_A 5
#define CLOCK_BASE_B 20


char clock[4] = { '|', '/', '-', '\\' };

unsigned short clockState[3][15];

void print(const char * text, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    int i;
    for (i = 0; text[i] != 0; i++) {
        p[y][x].c = (unsigned char) text[i];
        p[y][x].a = (unsigned char) attr;
        x++;
        if (x == VIDEO_COLS) {
            x = 0;
            y++;
        }
    }
}

void print_hex(unsigned int numero, int size, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    int i;
    char hexa[8];
    char letras[16] = "0123456789ABCDEF";
    hexa[0] = letras[ ( numero & 0x0000000F ) >> 0  ];
    hexa[1] = letras[ ( numero & 0x000000F0 ) >> 4  ];
    hexa[2] = letras[ ( numero & 0x00000F00 ) >> 8  ];
    hexa[3] = letras[ ( numero & 0x0000F000 ) >> 12 ];
    hexa[4] = letras[ ( numero & 0x000F0000 ) >> 16 ];
    hexa[5] = letras[ ( numero & 0x00F00000 ) >> 20 ];
    hexa[6] = letras[ ( numero & 0x0F000000 ) >> 24 ];
    hexa[7] = letras[ ( numero & 0xF0000000 ) >> 28 ];
    for(i = 0; i < size; i++) {
        p[y][x + size - i - 1].c = hexa[i];
        p[y][x + size - i - 1].a = attr;
    }
}

void print_int(unsigned int n, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
    if( n > 9 ) {
      int a = n / 10;
      n -= 10 * a;
      print_int(a,x-1,y,attr);
    }
    p[y][x].c = '0'+n;
    p[y][x].a = attr;
}

void inicializar_interfaz() {
	int x = 0;
	int y = 0;	
	while (x < 80) {
		print(" ", x, y, C_BG_BLACK);
		x++;
	}

	x = 0;
	y = 44;
	while (y < 50) {
		while (x < 80) {
			if(x <= 50 || x > 60) {
				print(" ", x, y, C_BG_BLACK);
			}else if (x > 55) {
				print(" ", x, y, C_BG_BLUE);
			}else {
				print(" ", x, y, C_BG_RED);
			}
			x++;
		}
		x = 0;
		y++;
	}
	//TODO: imprimir "vidas", puntos, indicadores de clocks
	

}

void print_alligned_right(const char * text){
	//toma un string terminado en 0 y lo imprime 
	//en la primera linea, alineado a derecha.
	int count = 0;

	while( text[count] != 0 ){
		count++;
	}
	int pos_inicial = VIDEO_COLS - count;
	print(text, pos_inicial, 0, C_FG_WHITE + C_BG_BLACK);
}

void screen_pintar_jugador(char id, unsigned short x, unsigned short y){
	char c, color; 
	if(id == 0){
		c = 'A';
		color = C_BG_RED | C_FG_WHITE;
	} 
	else{
		c = 'B';
		color = C_BG_BLUE | C_FG_WHITE;
	}
	ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
	p[y][x].c = c; 
	p[y][x].a = color;
}


void screen_pintar_tarea(taskType id, unsigned short x, unsigned short y){
	char c, color; 
	switch(id){
		case H_type:
			c = ' ';
			color = C_BG_GREEN | C_FG_WHITE;
			break;
		case A_type:
			c = ' ';
			color = C_BG_RED | C_FG_WHITE;
			break;
		case B_type:
			c = ' ';
			color = C_BG_BLUE | C_FG_WHITE;
			break;
		default:
			break;
	}
	ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
	p[y][x].c = c; 
	p[y][x].a = color;
}

void screen_limpiar_pixel(unsigned short x, unsigned short y){
	
	ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
	p[y][x].c = 0x00; 
	p[y][x].a = C_BG_LIGHT_GREY;


}


void screen_actualizar_reloj_tarea(tipo, indice){
	if(clockState[tipo][indice] < 4){
		clockState[tipo][indice]++;
	}else {
		clockState[tipo][indice] = 0;
	}

	char c = clockState[tipo][indice];
	short x, y, base;

   	switch(indice){
		case H_type: base = CLOCK_BASE_H; break;
		case A_type: base = CLOCK_BASE_A; break;
		case B_type: base = CLOCK_BASE_B; break;
	}
	
	x = base + indice * 2;
	y = (tipo == H_type) ? 48 : 46 ; 	
	
	ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO_SCREEN;
	p[y][x].c = c; 
	p[y][x].a = C_BG_BLACK | C_FG_WHITE;
}
