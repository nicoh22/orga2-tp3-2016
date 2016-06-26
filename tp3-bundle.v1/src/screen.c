/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "screen.h"

#define CLOCK_BASE_H 5
#define CLOCK_BASE_A 5
#define CLOCK_BASE_B 22


char clock[4] = { '|', '/', '-', '\\' };

unsigned short clock_State[3][15];

void print(const char * text, unsigned int x, unsigned int y, unsigned short attr) {
    char_attr (*p)[VIDEO_COLS] = (char_attr (*)[VIDEO_COLS]) VIDEO_SCREEN;
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
    char_attr (*p)[VIDEO_COLS] = (char_attr (*)[VIDEO_COLS]) VIDEO_SCREEN;
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
    char_attr (*p)[VIDEO_COLS] = (char_attr (*)[VIDEO_COLS]) VIDEO_SCREEN;
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
	y = 45; 
	// y = 0 linea de status
	// 1 <= y <= 44 las 44 filas del mapa 
	// y > 45 status del game
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
	

	print("<A", 17, 46, C_BG_BLACK | C_FG_WHITE);
	print("B>", 20, 46, C_BG_BLACK | C_FG_WHITE);

	print("vidas", 44, 45, C_BG_BLACK | C_FG_WHITE);
	print("vidas", 63, 45, C_BG_BLACK | C_FG_WHITE);

	int i, j;
	for( i = 0; i < 3; i++ ){
		for( j = 0; j < 15; j++ ){
			clock_State[i][j] = 0;
		}
	}

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
	char_attr (*p)[VIDEO_COLS] = (char_attr (*)[VIDEO_COLS]) VIDEO_SCREEN;
	p[y+1][x].c = c;
	p[y+1][x].a = color;
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
	char_attr (*p)[VIDEO_COLS] = (char_attr (*)[VIDEO_COLS]) VIDEO_SCREEN;
	p[y+1][x].c = c;
	p[y+1][x].a = color;
}

void screen_pintar_mapeo_tarea(taskType id, unsigned short x, unsigned short y){
	char c, color;
	switch(id){
		case A_type:
			c = 'A';
			color = C_BG_RED | C_FG_LIGHT_GREY;
			break;
		case B_type:
			c = 'B';
			color = C_BG_BLUE | C_FG_LIGHT_GREY;
			break;
		default:
			break;
	}
	char_attr (*p)[VIDEO_COLS] = (char_attr (*)[VIDEO_COLS]) VIDEO_SCREEN;
	p[y+1][x].c = c;
	p[y+1][x].a = color;
}


void screen_limpiar_posicion(unsigned short x, unsigned short y){
	
	char_attr (*p)[VIDEO_COLS] = (char_attr (*)[VIDEO_COLS]) VIDEO_SCREEN;
	p[y+1][x].c = 0x00;
	p[y+1][x].a = C_BG_LIGHT_GREY;


}


void screen_actualizar_reloj_tarea( taskType tipo,  unsigned short indice, unsigned char alive, taskType owner){
	char c = ' ';
	if(alive){
		if(clock_State[tipo][indice] < 3){
			clock_State[tipo][indice]++;
		}else {
			clock_State[tipo][indice] = 0;
		}

		c = clock[ clock_State[tipo][indice] ];
	}else{
		clock_State[tipo][indice] = 0;
	}

	short x, y, base;

	unsigned char fg_color = C_FG_WHITE;
   	switch(tipo){
		case H_type: base = CLOCK_BASE_H; break;
		case A_type: base = CLOCK_BASE_A; break;
		case B_type: base = CLOCK_BASE_B; break;
		default: return;
	}

   	switch(owner){
   			case H_type: fg_color = C_FG_WHITE; break;
   			case A_type: fg_color = C_FG_LIGHT_RED; break;
   			case B_type: fg_color = C_FG_LIGHT_BLUE; break;
   			default: return;
   		}
	
	x = base + indice * 2;
	y = (tipo == H_type) ? 48 : 46 ; 	
	
	char_attr (*p)[VIDEO_COLS] = (char_attr (*)[VIDEO_COLS]) VIDEO_SCREEN;
	p[y][x].c = c; 


	p[y][x].a = C_BG_BLACK | fg_color;
}


void screen_actualizar_puntos(unsigned short puntosA, unsigned short puntosB){
	print_int(puntosA, 53, 47, C_BG_RED | C_FG_WHITE);
	print_int(puntosB, 58, 47, C_BG_BLUE  | C_FG_WHITE);
}

void screen_actualizar_vidas(unsigned short vidasA, unsigned short vidasB){
	print_int(vidasA, 46, 47, C_BG_BLACK | C_FG_WHITE);
	print_int(vidasB, 65, 47, C_BG_BLACK | C_FG_WHITE);
}

void imprimir_log(unsigned int *espPtr) {
	
	unsigned int stack4 = espPtr[0];
	unsigned int stack3 = espPtr[1];
	unsigned int stack2 = espPtr[2];
	unsigned int stack1 = espPtr[3];
	unsigned int stack0 = espPtr[4];

    char *virus  = (char*)espPtr[5]; //ocupa 8 chares
	
	unsigned int cr4 = espPtr[6];
	unsigned int cr3 = espPtr[7];
	unsigned int cr2 = espPtr[8];
	unsigned int cr0 = espPtr[9];

	unsigned short *espPtrShort = (unsigned short *)espPtr;
	unsigned short ds = espPtrShort[20];
	unsigned short es = espPtrShort[22];
	unsigned short fs = espPtrShort[24];
	unsigned short gs = espPtrShort[26];

	unsigned int ebp = espPtr[14]; 
	unsigned int edi = espPtr[15]; 
	unsigned int esi = espPtr[16]; 
	unsigned int edx = espPtr[17]; 
	unsigned int ecx = espPtr[18]; 
	unsigned int ebx = espPtr[19]; 
	unsigned int eax = espPtr[20]; 	

	unsigned int eip = espPtr[22]; // espPtr[17]; = error code
    unsigned short cs = espPtrShort[46];
	unsigned short eflags = espPtrShort[48];
    unsigned int esp = espPtr[25]; 
	unsigned short ss = espPtrShort[52];
	
	// desde la posicion x: 25 a 54 y: 7 -> negro una fila 
	// para x: 25 y x: 54, desde y:7 hasta 42 negro una columna
	// desde la posicion x: 25 a 54 y: 42 -> negro una fila
	// desde la posicion x: 26 a 53 y: 8 -> rojo una fila
	// limpiar el resto
	
	char bg_color = C_BG_LIGHT_GREY | C_FG_LIGHT_GREY;
	char border_color = C_BG_BLACK | C_FG_BLACK;
	char bg_title_color = C_BG_RED | C_FG_RED;
	int y,x;
	for(y = 7; y < 43; y++) {
		for(x = 25; x < 55; x++) {
			if (y == 7 || y == 42 || x == 25 || x == 54) {
				print(" ", x, y, border_color);			
			}else if (y == 8) {
				print(" ", x, y, bg_title_color);
			}else {
				print(" ", x, y, bg_color);
			}
		}
	}
	
	char titleAttr = C_BG_LIGHT_GREY | C_FG_BLACK;
	char textAttr = C_BG_LIGHT_GREY | C_FG_WHITE;
	print(virus, 26, 8, C_BG_RED | C_FG_WHITE);
	print("eax ", 27, 10, titleAttr);
	print_hex(eax, 8, 31, 10, textAttr);
	print("ebx ", 27, 12, titleAttr);
	print_hex(ebx, 8, 31, 12, textAttr); 	
	print("ecx ", 27, 14, titleAttr);
	print_hex(ecx, 8, 31, 14, textAttr); 
	print("edx ", 27, 16, titleAttr);
	print_hex(edx, 8, 31, 16, textAttr); 
	print("esi ", 27, 18, titleAttr);
	print_hex(esi, 8, 31, 18, textAttr); 
	print("edi ", 27, 20, titleAttr);
	print_hex(edi, 8, 31, 20, textAttr); 
	print("ebp ", 27, 22, titleAttr);
	print_hex(ebp, 8, 31, 22, textAttr); 
	print("esp ", 27, 24, titleAttr);
	print_hex(esp, 8, 31, 24, textAttr); 
	print("eip ", 27, 26, titleAttr);
	print_hex(eip, 8, 31, 26, textAttr); 
	print("cs ", 28, 28, titleAttr);
	print_hex((unsigned int)cs, 4, 32, 28, textAttr); 
	print("ds ", 28, 30, titleAttr);
	print_hex((unsigned int)ds, 4, 32, 30, textAttr); 
	print("es ", 28, 32, titleAttr);
	print_hex((unsigned int)es, 4, 32, 32, textAttr); 
	print("fs ", 28, 34, titleAttr);
	print_hex((unsigned int)fs, 4, 32, 34, textAttr); 
	print("gs ", 28, 36, titleAttr);
	print_hex((unsigned int)gs, 4, 32, 36, textAttr); 
	print("ss ", 28, 38, titleAttr);
	print_hex((unsigned int)ss, 4, 32, 38, textAttr); 
	print("eflags ", 28, 40, titleAttr);
	print_hex((int)eflags, 4, 36, 40, textAttr); 
	print("cr0 ", 41, 10, titleAttr);
	print_hex(cr0, 8, 45, 10, textAttr); 
	print("cr1 ", 41, 12, titleAttr);
	print_hex(cr2, 8, 45, 12, textAttr); 
	print("cr3 ", 41, 14, titleAttr);
	print_hex(cr3, 8, 45, 14, textAttr); 
	print("cr4 ", 41, 16, titleAttr);
	print_hex(cr4, 8, 45, 16, textAttr); 
	print("stack ", 41, 22, titleAttr);
	print_hex(stack0, 8, 45, 26, textAttr); 
	print_hex(stack1, 8, 45, 28, textAttr); 
	print_hex(stack2, 8, 45, 30, textAttr); 
	print_hex(stack3, 8, 45, 32, textAttr); 
	print_hex(stack4, 8, 45, 34, textAttr); 

}
