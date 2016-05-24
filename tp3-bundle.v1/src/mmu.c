/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"
// Atributos 0x11 (read/write y present)
#define ATTR 3
#define PAGE_DIRECTORY 0X27000
#define PAGE_TABLE 0X28000


void mmu_inicializar() {
}

void mmu_inicializar_dir_kernel() {
	// Usamos punteros a int para direccionar 32bits, que es el tamaño
	// de las entradas de la tabla el directorio.
	int *page_directory = (int *)PAGE_DIRECTORY;

	// Limpiamos el directorio
	int i = 0;
	while (i < 1024) {
		page_directory[i] = 0;
		i++;
	}
	
	// Registramos la primer pagina en el directorio
	int *page_table = (int *)PAGE_TABLE;
	page_directory[0] = (int)page_table + ATTR; 


	// En la tabla de paginas registramos las primeras 1024 con bloques
	// de 4kb
	int base_page_addr = 0;
	i = 0;
	while(i < 1024) {
		page_table[i] = base_page_addr + ATTR;
		base_page_addr += 4096;						
		i++;
	}
}







