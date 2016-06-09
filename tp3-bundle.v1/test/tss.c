/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"

tss tss_inicial;
tss tss_idle;

unsigned int proxima_tss_index = 0;
tss* tss_directory[TSS_COUNT];

void tss_cargar_en_gdt(tss* tss_pointer, int gdt_index);

void tss_inicializar() {
	//llena idle e inicial 
	tss_inicial.ptl = 0;
	tss_inicial.unused0 = 0;
	tss_inicial.esp0 = 0;
	tss_inicial.ss0 = 0;
	tss_inicial.unused1 = 0;
	tss_inicial.esp1 = 0;
	tss_inicial.ss1 = 0;
	tss_inicial.unused2 = 0;
	tss_inicial.esp2 = 0;
	tss_inicial.ss2 = 0;
	tss_inicial.unused3 = 0;
	tss_inicial.cr3 = 0;
	tss_inicial.eip = 0;
	tss_inicial.eflags = 0;
	tss_inicial.eax = 0;
	tss_inicial.ecx = 0;
	tss_inicial.edx = 0;
	tss_inicial.ebx = 0;
	tss_inicial.esp = 0;
	tss_inicial.ebp = 0;
	tss_inicial.esi = 0;
	tss_inicial.edi = 0;
	tss_inicial.es = 0;
	tss_inicial.unused4 = 0;
	tss_inicial.cs = 0;
	tss_inicial.unused5 = 0;
	tss_inicial.ss = 0;
	tss_inicial.unused6 = 0;
	tss_inicial.ds = 0;
	tss_inicial.unused7 = 0;
	tss_inicial.fs = 0;
	tss_inicial.unused8 = 0;
	tss_inicial.gs = 0;
	tss_inicial.unused9 = 0;
	tss_inicial.ldt = 0;
	tss_inicial.unused10 = 0;
	tss_inicial.dtrap = 0;
	tss_inicial.iomap = 0;

	tss_idle.ptl = 0;
	tss_idle.unused0 = 0;
	tss_idle.esp0 = STACK_BASE_KERN;
	tss_idle.ss0 = 0x28;
	tss_idle.unused1 = 0;
	tss_idle.esp1 = 0;
	tss_idle.ss1 = 0;
	tss_idle.unused2 = 0;
	tss_idle.esp2 = 0;
	tss_idle.ss2 = 0;
	tss_idle.unused3 = 0;
	tss_idle.cr3 = PAGE_DIRECTORY_KERN;
	tss_idle.eip = 0x10000;
	tss_idle.eflags = 0x00000202;//TODO: despues cambiar por 0x00000202
	tss_idle.eax = 0;
	tss_idle.ecx = 0;
	tss_idle.edx = 0;
	tss_idle.ebx = 0;
	tss_idle.esp = STACK_BASE_KERN;
	tss_idle.ebp = STACK_BASE_KERN;
	tss_idle.esi = 0;
	tss_idle.edi = 0;
	tss_idle.es = 0x28;
	tss_idle.unused4 = 0;
	tss_idle.cs = 0x20;
	tss_idle.unused5 = 0;
	tss_idle.ss = 0x28;
	tss_idle.unused6 = 0;
	tss_idle.ds = 0x28;
	tss_idle.unused7 = 0;
	tss_idle.fs = 0x28;
	tss_idle.unused8 = 0;
	tss_idle.gs = 0x28;
	tss_idle.unused9 = 0;
	tss_idle.ldt = 0;
	tss_idle.unused10 = 0;
	tss_idle.dtrap = 0;
	tss_idle.iomap = 0;
	
	//Cargar las entradas de la gdt
	//Por eso en tss.h esta el include de gdt.h y no al reves

	
	tss_cargar_en_gdt(&tss_inicial, GDT_IDX_TSS_INICIAL);
	tss_cargar_en_gdt(&tss_idle, GDT_IDX_TSS_IDLE);

}

void tss_cargar_en_gdt(tss* tss_pointer, int gdt_index){
	
	unsigned int tss = (unsigned int) tss_pointer;
	unsigned int limite = 103;
	
	gdt[gdt_index].base_0_15 = (tss & 0x0000FFFF);
	gdt[gdt_index].base_23_16 = ((tss >> 16) & 0x000000FF);
	gdt[gdt_index].base_31_24 = (tss >> 24);
	gdt[gdt_index].limit_0_15 = limite & 0x0000FFFF;
	gdt[gdt_index].limit_16_19 = (limite >> 16) & 0x0000000F;
	gdt[gdt_index].type = 0x9;
	gdt[gdt_index].s = 0x0;
	gdt[gdt_index].dpl = 0x00;
	gdt[gdt_index].p = 0x1;
	gdt[gdt_index].avl = 0x1;
	gdt[gdt_index].db = 0x0;
	gdt[gdt_index].l = 0x0;
	gdt[gdt_index].g = 0x0;
	
}

tss* tss_proxima(){
	return tss_directory[proxima_tss_index++];
}

/**
 * Tipo = tipo de tarea (A, B o Idle)
 * fisica = posicion de memoria fisica 
 * donde vamos a mapear la tarea (en el mapa)
*/
tss* tss_crear_tarea(taskType tipo, int gdt_index, unsigned int fisica){

	tss* new_tss = tss_proxima();
	
	tss_cargar_en_gdt(new_tss, gdt_index);

	unsigned int cr3_task =  mmu_inicializar_dir_tarea(tipo, fisica);
	
	unsigned int kernStack = (int) mmu_proxima_pagina_fisica_libre();
	
	//esta linea no compila (no esta incluido mmu.h), 
	//entonces asumo que el scheduler me pasa el cr3
	//pero tengo que pedir la pagina para la pila del kernel

	new_tss->ptl = 0;
	new_tss->unused0 = 0;
	new_tss->esp0 = kernStack + PAGE_SIZE;
	new_tss->ss0 = ( GDT_IDX_KERN_DATA << 3 );
	new_tss->unused1 = 0;
	new_tss->esp1 = 0;
	new_tss->ss1 = 0;
	new_tss->unused2 = 0;
	new_tss->esp2 = 0;
	new_tss->ss2 = 0;
	new_tss->unused3 = 0;
	new_tss->cr3 = cr3_task;
	new_tss->eip = TASK_CODE;
	new_tss->eflags = 0x00000202;
	new_tss->eax = 0;
	new_tss->ecx = 0;
	new_tss->edx = 0;
	new_tss->ebx = 0;
	new_tss->esp = TASK_CODE + PAGE_SIZE;
	new_tss->ebp = TASK_CODE + PAGE_SIZE;
	new_tss->esi = 0;
	new_tss->edi = 0;
	new_tss->es = 0;
	new_tss->unused4 = 0;
	new_tss->cs = ( GDT_IDX_USER_CODE << 3 );
	new_tss->unused5 = 0;
	new_tss->ss = ( GDT_IDX_USER_DATA << 3 );
	new_tss->unused6 = 0;
	new_tss->ds = ( GDT_IDX_USER_DATA << 3 );
	new_tss->unused7 = 0;
	new_tss->fs = 0;
	new_tss->unused8 = 0;
	new_tss->gs = 0;
	new_tss->unused9 = 0;
	new_tss->ldt = 0;
	new_tss->unused10 = 0;
	new_tss->dtrap = 0;
	new_tss->iomap = 0;


	return new_tss;
}

