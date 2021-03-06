; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "imprimir.mac"
%define GDT_SEL_TSS_IDLE	0x50

BITS 32

sched_tarea_offset:     dd 0x00
sched_tarea_selector:   dw 0x00

LA: db 'A', 0 
LS: db 'S', 0 
LD: db 'D', 0 
LW: db 'W', 0 
LJ: db 'J', 0 
LK: db 'K', 0 
LL: db 'L', 0 
LI: db 'I', 0 
LLESS: db '<', 0 
LGREATER: db '>', 0 

;; PIC
extern fin_intr_pic1

;; Sched
extern sched_proximo_indice
extern sched_desalojar_actual
extern sched_tarea_actual_owner

;; Screen
extern print
extern mensajesExcepcion
extern screen_imprimir_log

;; Rutinas de atencion de alto nivel
extern atender_teclado 
extern manejar_syscall
extern game_tick

;; Debug
extern debug_set_interrupted

;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

%macro ISR 1
global _isr%1

_isr%1:

    ; En la pila actualmente:
    ; ss
    ; esp
    ; eflags
    ; cs
    ; eip
    ; error code
    push eax
    call debug_set_interrupted 
    cmp eax, 1
    ; si se puede interrumpir por debug, continuamos
    jne _isr%1.debugDisable
    push ebx
    mov ebx, [esp + 24] ; esp nivel 3
    push ecx
    push edx
    push esi
    push edi
    push ebp
    xor ecx, ecx
    mov cx, ds
    push ecx ; ds = ss = fs = gs? gdt index user data 
    mov cx, es
    push ecx
    mov cx, fs
    push ecx
    mov cx, gs
    push ecx
    mov ecx, cr0
    push ecx
    mov ecx, cr2
    push ecx
    mov ecx, cr3
    push ecx
    mov ecx, cr4
    push ecx 
    call sched_tarea_actual_owner
    push eax
    mov ecx, [ebx] ; stack0
    push ecx
    mov ecx, [ebx + 4] ; stack1
    push ecx
    mov ecx, [ebx + 8] ; stack2
    push ecx
    mov ecx, [ebx + 12] ; stac3
    push ecx
    mov ecx, [ebx + 16] ; stack4
    push ecx
    push esp
    call screen_imprimir_log

_isr%1.debugDisable:

    	mov eax, %1
	shl eax, 2
	mov eax, [eax + mensajesExcepcion]
	
	push 0x7 ; color
	push 0   ; y
	push 0   ; x
	push eax;
	call print

	call sched_desalojar_actual
	jmp GDT_SEL_TSS_IDLE:0
	xchg bx, bx

	pop eax
	pop ecx
	pop ecx
	pop ecx
	xor eax, eax
	jmp $


%endmacro

;;
;; Datos
;; -------------------------------------------------------------------------- ;;
; Scheduler
isrnumero:           dd 0x00000000
isrClock:            db '|/-\'

;;
;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;
ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR 15
ISR 16
ISR 17
ISR 18
ISR 19
;;
;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;

global _isr32

_isr32: 
;El stackframe podria ser mas chico
;cambiar pushad y popad por los que sean necesarios
	pushad
	call proximo_reloj	
	
	call sched_proximo_indice
	shl ax, 3	
	cmp ax, 0
	je .noJump
		mov [sched_tarea_selector], ax

		call fin_intr_pic1

		jmp far [sched_tarea_offset]
		jmp .end
	.noJump:
		call fin_intr_pic1		

	.end:
	call game_tick
	popad
	iret

;;
;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;

%define A 0x1e
%define S 0x1f
%define D 0x20
%define W 0x11
%define J 0x24
%define K 0x25
%define L 0x26
%define I 0x17
%define L_SHIFT 0x2a
%define R_SHIFT 0x36

global _isr33

_isr33:
	pushad
	xor eax, eax
	in al, 0x60
	
	push eax
	call atender_teclado 
	pop eax

	call fin_intr_pic1	
	popad
	iret


;
;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;

%define DONDE  0x124
%define SOY    0xA6A
%define MAPEAR 0xFF3

%define VIRUS_ROJO 0x841
%define VIRUS_AZUL 0x325

%define GDT_SEL_TSS_IDLE	0x50

global _isr102

_isr102:

	pushad
	
	push ecx
	push ebx
	push eax
	
	call manejar_syscall
	
	pop ecx
	pop ecx
	pop eax
	; quiero conservar eax

	; saltamos a la idle hasta el proximo tick
	jmp GDT_SEL_TSS_IDLE:0

	popad
	iret


;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;
proximo_reloj:
        pushad
        inc DWORD [isrnumero]
        mov ebx, [isrnumero]
        cmp ebx, 0x4
        jl .ok
                mov DWORD [isrnumero], 0x0
                mov ebx, 0
        .ok:
                add ebx, isrClock
                imprimir_texto_mp ebx, 1, 0x0f, 49, 79
                popad
        ret

