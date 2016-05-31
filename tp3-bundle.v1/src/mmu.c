/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"



unsigned int proxima_pagina_libre;

void mmu_inicializar() {
	proxima_pagina_libre = INICIO_PAGINAS_LIBRES;
}
int *mmu_proxima_pagina_fisica_libre() {
	unsigned int pagina_libre = proxima_pagina_libre;
	proxima_pagina_libre += PAGE_SIZE;
	return (int*) pagina_libre;
}




void mmu_inicializar_dir_kernel() {
	// Usamos punteros a int para direccionar 32bits, que es el tamaño
	// de las entradas de la tabla el directorio.
	int *page_directory = (int *)PAGE_DIRECTORY_KERN;

	// Limpiamos el directorio
	int i = 0;
	while (i < 1024) {
		page_directory[i] = 0;
		i++;
	}
	
	// Registramos la primer pagina en el directorio
	int *page_table = (int *)PAGE_TABLE_KERN;
	page_directory[0] = (int)page_table + ATTR_KERN; 


	// En la tabla de paginas registramos las primeras 1024 con bloques
	// de 4kb
	int base_page_addr = 0;
	i = 0;
	while(i < 1024) {
		page_table[i] = base_page_addr + ATTR_KERN;
		base_page_addr += 4096;						
		i++;
	}
}

unsigned int mmu_inicializar_dir_tarea(unsigned int tipo, int fisica){
	int *page_directory = mmu_proxima_pagina_fisica_libre();

	// Limpiamos el directorio
	int i = 0;
	while (i < 1024) {
		page_directory[i] = 0;
		i++;
	}
	
	// Registramos la primer pagina en el directorio
	int *page_table = mmu_proxima_pagina_fisica_libre(); 
	page_directory[0] = (int)page_table + ATTR_KERN; 

	// En la tabla de paginas registramos las primeras 1024 con bloques
	// de 4kb
	
	int base_page_addr = 0;
	i = 0;
	while(i < 1024) {
		page_table[i] = base_page_addr + ATTR_KERN;
		base_page_addr += 4096;						
		i++;
	}
	// logica para determinar direccion fisica (en el mapa)
	// TODO en vez del parametro fisica podemos recibir el X y el Y
	// y calcular su direccion aca o en un funcion "xyaFisica"
	// Mapeamos a la estructura de paginacion del kernel
	// la direccion fisica donde vamos a copiar el codigo de la tarea
	mmu_mapear_pagina(fisica, PAGE_DIRECTORY_KERN, fisica, ATTR_KERN);
	//tipo de tarea
	int* codigo_fuente = (int*) (IDLE_TASK + (tipo * PAGE_SIZE));
	mmu_copiar_pagina(codigo_fuente, (int*) fisica);
	
	unsigned int cr3Tarea = (int) page_directory;
	

	//En principio se mapea a si misma
	//No esta atacando a nadie
	mmu_mapear_pagina(TASK_CODE + PAGE_SIZE, cr3Tarea, fisica, ATTR_USER);
	return cr3Tarea;
}

void mmu_mapear_pagina( unsigned int virtual, unsigned int cr3, unsigned int fisica, short attr){
	int *page_directory = (int*) (ALIGN(cr3));
	int *page_table = (int*) (page_directory[PDE_INDEX(virtual)] && 0xFFFFF000);	
	
	if(page_table == 0){
		page_table = mmu_proxima_pagina_fisica_libre();
		page_directory[PDE_INDEX(virtual)] = (int) page_table + attr; 
		int i = 0;
		while(i < 1024) {// Limpiamos la tabla nueva
			page_table[i] = 0; 
			i++;
		}
		
	}
	page_table[PTE_INDEX(virtual)] = fisica + attr;
	tlbflush();	
}
void mmu_unmapear_pagina(unsigned int virtual, unsigned int cr3){
	
	int *page_directory = (int*) (ALIGN(cr3));
	int *page_table = (int*) (page_directory[PDE_INDEX(virtual)] && 0xFFFFF000);	
	if(page_table != 0){
		page_table[PTE_INDEX(virtual)] = 0;
	}
	tlbflush();
}



void mmu_copiar_pagina(int* codigo_fuente, int* fisica){
	int i = 0; 
	while(i < 1024){
		fisica[i] = codigo_fuente[i];
		i++;
	}
};

