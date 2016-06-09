/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#ifndef __MMU_H__
#define __MMU_H__

#include "defines.h"
#include "i386.h"
#include "tss.h"
#include "game.h"

// Atributos 0x11 (read/write y present)
#define ATTR_KERN PG_READ_WRITE + PG_PRESENT
#define ATTR_USER PG_USER + PG_READ_WRITE + PG_PRESENT

//Macros para paginacion
#define PDE_INDEX(virtual) virtual >> 22
#define PTE_INDEX(virtual) (virtual << 10) >> 22
#define ALIGN(dir)         (dir >> 12) << 12
#define PG_READ_WRITE      0x02
#define PG_USER            0x04
#define PG_PRESENT         0x00000001
extern void mmu_inicializar();
extern void mmu_inicializar_dir_kernel();
extern unsigned int mmu_inicializar_dir_tarea(taskType tipo, unsigned int fisica);
extern void mmu_mapear_pagina( unsigned int virtual, unsigned int cr3, unsigned int fisica, short attr);
extern void mmu_unmapear_pagina(unsigned int virtual, unsigned int cr3);
extern void mmu_copiar_pagina(int* codigo_fuente, int* fisica);
int* mmu_proxima_pagina_fisica_libre();
#endif	/* !__MMU_H__ */




