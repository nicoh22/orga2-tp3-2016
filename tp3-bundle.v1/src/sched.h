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
#include "game.h"
#include "gdt.h"
#include "taskInfo.h"
extern unsigned short sched_proximo_indice();
void sched_lanzar_tareas(taskType tipo, unsigned short x, unsigned short y );
extern void sched_init();
extern void sched_set_enLaIdle();
task_info* sched_tarea_actual();
task_info tareasInfo[3][15];
unsigned short enLaIdle;
unsigned short task_max_index(short current);
unsigned int sched_desalojar_actual();
int sched_proximo_slot_tarea_libre(taskType tipo);
short sched_tipo_actual();
short sched_indice_actual();
#endif	/* !__SCHED_H__ */
