; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "imprimir.mac"

extern GDT_DESC
extern IDT_DESC

extern idt_inicializar
extern mmu_inicializar
extern mmu_inicializar_dir_kernel
extern mmu_inicializar_dir_tarea
extern inicializar_interfaz
extern print_alligned_right
extern resetear_pic
extern habilitar_pic
extern tss_inicializar
extern sched_init
extern game_inicializar

%define PAGE_DIRECTORY 0X27000
%define PAGE_TABLE 0X28000

%define stackBasePointerKern 0x27000
%define dataSegmentSelectorKern 0x28

%define GDT_SEL_TSS_INICIAL	0x48 
%define GDT_SEL_TSS_IDLE	0x50

%define INT_SOFT 0x66

global start


;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
iniciando_mr_msg db	 'Iniciando kernel (Modo Real)...'
iniciando_mr_len equ	$ - iniciando_mr_msg

iniciando_mp_msg db	 'Iniciando kernel (Modo Protegido)...'
iniciando_mp_len equ	$ - iniciando_mp_msg

nombre_grupo db 'A PC regalado no se le mira procesador', 0
;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
	; Deshabilitar interrupciones
	cli

	; Cambiar modo de video a 80 X 50
	mov ax, 0003h
	int 10h ; set mode 03h
	xor bx, bx
	mov ax, 1112h
	int 10h ; load 8x8 font

	; Imprimir mensaje de bienvenida
	imprimir_texto_mr iniciando_mr_msg, iniciando_mr_len, 0x07, 0, 0

	; Habilitar A20 - no hay que salver registros
	call habilitar_A20
	; Cargar la GDT
	lgdt [GDT_DESC]

	; Setear el bit PE del registro CR0
	mov eax, cr0
	or al, 1 ; set PE (Protection Enable) bit in CR0 (Control Register 0)
	mov cr0, eax

	; Saltar a modo protegido
	; 20h es el selector de segmento de codigo de kernel
	JMP 20h:protectedMode

protectedMode:
	; Establecer selectores de segmentos
BITS 32

	mov ax, dataSegmentSelectorKern
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
	; Establecer la base de la pila
	mov ebp, stackBasePointerKern
	mov esp, ebp
		
	
	; Imprimir mensaje de bienvenida
	imprimir_texto_mp iniciando_mp_msg, iniciando_mp_len, 0x07, 0, 3

	; Inicializar pantalla
	mov ecx, 0
	.inicializarPantalla:
	; 80x50 * 2 bytes: 	8000 bytes
	; Espacio con bg gris : 0x0720
	; Segmento de video (8) 0000 0000 0100 0000
	mov eax, 0x40
	mov gs, ax
	mov eax, 0x7020
	mov [gs:ecx], ax
	add ecx, 2
	cmp ecx, 8000
	JL .inicializarPantalla
	
	; Inicializar el manejador de memoria
	call mmu_inicializar
	; Inicializar el directorio de paginas
	; Cargar directorio de paginas

	call mmu_inicializar_dir_kernel ; inicializa y carga el dir

	; Habilitar paginacion

	mov eax, PAGE_DIRECTORY
	mov cr3, eax

	mov eax, cr0
	or eax, 0x80000000
	mov cr0, eax
 
	; Inicializar interfaz de usuario
	call inicializar_interfaz


	; xchg bx, bx
	mov ebx, nombre_grupo
	push ebx
	call print_alligned_right	
	pop ebx

	; Inicializar tss
	; Inicializar tss de la tarea Idle
	call tss_inicializar

	; Inicializar el scheduler

	; Inicializar la IDT
	call idt_inicializar
  
	; Cargar IDT
	lidt [IDT_DESC]		

;test interrupciones

;	mov eax, 0
;	div eax

;fin test

;test mmu

;	xor ebx, ebx
;	mov ebx, 1
;	mov ecx, 0x400000 ; direccion base del mapa 
;	push ecx
;	push ebx
	
;	xchg bx, bx
;	call mmu_inicializar_dir_tarea 
;	mov cr3, eax ; la tlb se flushea sola aca
	
;	mov ebx, 0xb8000 ; memoria de video
;	mov byte [ebx + 1], 0x4F
;	mov byte [ebx], 100
;	mov eax, PAGE_DIRECTORY	
;	mov cr3, eax

;fin test

	; Configurar controlador de interrupciones
	
	call resetear_pic
	call habilitar_pic
	
	
	;inicializar el scheduler
	call sched_init
	call game_inicializar 

	; Cargar tarea inicial
  
	; xchg bx, bx
	; Se carga el tr con el selector de segmento de la tarea inicial
	mov ax, GDT_SEL_TSS_INICIAL
	ltr ax
	; xchg bx, bx
	; Se intercambian las tareas de la inicial a la idle
	; El procesador pisa el contexto de la tarea inicial con fruta
	  
	; Habilitar interrupciones
	sti
	; Saltar a la primera tarea: Idle
	jmp GDT_SEL_TSS_IDLE:0
	
	
;test interrupcion software

;	int INT_SOFT

;fin test
   

	; Ciclar infinitamente (por si algo sale mal...)
	mov eax, 0xFFFF
	mov ebx, 0xFFFF
	mov ecx, 0xFFFF
	mov edx, 0xFFFF
	jmp $
	jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"
