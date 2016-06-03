/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"

tss tss_inicial;
tss tss_idle;

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
	tss_idle.eip = 0;
	tss_idle.eflags = 0x02;//despues cambiar por 0x202
	tss_idle.eax = 0;
	tss_idle.ecx = 0;
	tss_idle.edx = 0;
	tss_idle.ebx = 0;
	tss_idle.esp = STACK_BASE_KERN;
	tss_idle.ebp = STACK_BASE_KERN;
	tss_idle.esi = 0;
	tss_idle.edi = 0;
	tss_idle.es = 0;
	tss_idle.unused4 = 0;
	tss_idle.cs = 0x20;
	tss_idle.unused5 = 0;
	tss_idle.ss = 0x28;
	tss_idle.unused6 = 0;
	tss_idle.ds = 0;
	tss_idle.unused7 = 0;
	tss_idle.fs = 0;
	tss_idle.unused8 = 0;
	tss_idle.gs = 0;
	tss_idle.unused9 = 0;
	tss_idle.ldt = 0;
	tss_idle.unused10 = 0;
	tss_idle.dtrap = 0;
	tss_idle.iomap = 0;
}

void tss_cargar_en_gdt(){

}

tss* tss_definir_tarea(){}
