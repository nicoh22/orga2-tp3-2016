/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"



unsigned int proxima_pagina_libre;

int inMemoryCodeOrder(taskType tipo);

void mmu_inicializar() {
	proxima_pagina_libre = INICIO_PAGINAS_LIBRES;
}

int* mmu_proxima_pagina_fisica_libre() {
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

unsigned int mmu_inicializar_dir_tarea(taskType tipo, unsigned int fisica){
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

	unsigned int cr3Tarea = (int) page_directory;

	mmu_mapear_pagina(fisica, rcr3(), fisica, ATTR_KERN);

	//tipo de tarea
	int* codigo_fuente = (int*) (IDLE_TASK + (inMemoryCodeOrder(tipo) * PAGE_SIZE));
	mmu_copiar_pagina(codigo_fuente, (int*) fisica);

	mmu_unmapear_pagina(fisica, rcr3());
	

	
	mmu_mapear_pagina(TASK_CODE, cr3Tarea, fisica, ATTR_USER);

	//En principio se mapea a si misma
	//No esta atacando a nadie
	mmu_mapear_pagina(TASK_CODE + PAGE_SIZE, cr3Tarea, fisica, ATTR_USER);
	return cr3Tarea;
}

int inMemoryCodeOrder(taskType tipo){
//	idle_type, A_type, B_type, H_type
	switch(tipo){
		case idle_type:
			return 0;
		case A_type:
			return 1;
		case B_type:
			return 2;
		case H_type:
			return 3;
	}
	return 0;
}

void mmu_mapear_pagina( unsigned int virtual, unsigned int cr3, unsigned int fisica, short attr){
	unsigned int pageDirIndex = PDE_INDEX(virtual);	//ebp-12
	unsigned int pageDirTableIndex = PTE_INDEX(virtual); //ebp-16
	unsigned int page_table_entry = fisica | attr; //ebp-20
	unsigned int coso = 0;
	int* page_directory = (int*) (ALIGN(cr3));
	int* page_table = 0;

	int presente = (page_directory[pageDirIndex] & 0x01);
	if(!presente){ // chequeo presente! 
		page_table = mmu_proxima_pagina_fisica_libre();
		page_directory[pageDirIndex] = ( (unsigned int) page_table ) | attr; 
		int i = 0;
		while(i < 1024) {// Limpiamos la tabla nueva
			page_table[i] = 0; 
			i++;
		}
	}else{
		coso = (page_directory[pageDirIndex]) & 0xFFFFF000;
		page_table = ((int*) (page_directory[pageDirIndex] & 0xFFFFF000));
		if(coso){
		}		
	}
	
	page_table[pageDirTableIndex] = page_table_entry;

	tlbflush();
}

void mmu_unmapear_pagina(unsigned int virtual, unsigned int cr3){
	
	int *page_directory = (int*) (ALIGN(cr3));
	int *page_table = (int*) (page_directory[PDE_INDEX(virtual)] & 0xFFFFF000);	
	int presente = (page_directory[PDE_INDEX(virtual)] & 0x01);
	int directorioVacio = 1;
	if(presente){
		page_table[PTE_INDEX(virtual)] = 0;
		int i = 0;
		while(directorioVacio && i < 1024){
			int tablaVacia = (page_table[i] & 0x01);
			// Aca usamos and logico. El directorio esta vacio
			// si ya estaba vacio y la i-esima tabla esta vacia
			directorioVacio = directorioVacio && tablaVacia;
			i++;
		}
		if(directorioVacio){
			page_directory[PDE_INDEX(virtual)] = 0;
		}
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

