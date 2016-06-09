/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "screen.h"
#include "tss.h"
#include "gdt.h"
#include "taskInfo.h"
extern unsigned short sched_proximo_indice();
void sched_lanzar_tareas(taskType tipo, unsigned int fisica );
extern void sched_init();

#endif	/* !__SCHED_H__ */
