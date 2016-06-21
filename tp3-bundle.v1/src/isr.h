/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de atencion de interrupciones
*/

#ifndef __ISR_H__
#define __ISR_H__

extern char* mensajesExcepcion[];

extern debugStateType debugState;

void _isr0();
void _isr1();
void _isr2();
void _isr3();
void _isr4();
void _isr5();
void _isr6();
void _isr7();
void _isr8();
void _isr9();
void _isr10();
void _isr11();
void _isr12();
void _isr13();
void _isr14();
void _isr15();
void _isr16();
void _isr17();
void _isr18();
void _isr19();
void _isr32();
void _isr33();
void _isr102();

char* mensajesExcepcion[20] = 
{ "Divide Error",
 "RESERVED",
 "NMI Interrupt",
 "Breakpoint",
 "Overflow",
 "BOUND Range Exceeded",
 "Invalid Opcode (Undefined Opcode)",
 "Device Not Available (No Math Coprocessor)",
 "Double Fault",
 "Coprocessor Segment Overrun (reserved)",
 "Invalid TSS",
 "Segment Not Present",
 "Stack-Segment Fault",
 "General Protection",
 "Page Fault",
 "(Intel reserved. Do not use.)",
 "x87 FPU Floating-Point Error (Math Fault)",
 "Alignment Check",
 "Machine Check",
 "SIMD Floating-Point Exception"
};


#endif  /* !__ISR_H__ */
