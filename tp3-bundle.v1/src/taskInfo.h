/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
	Informacion sobre tareas
*/

#ifndef __TASK_INFO_H__
#define __TASK_INFO_H__

#include "tss.h"
#include "gdt.h"

typedef struct str_task_info {
    unsigned char alive;
    task_type owner;
    int gdtIndex;
} __attribute__((__packed__)) task_info;

#endif	/* !__TASK_INFO__ */
