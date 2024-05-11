[extern isrHandler]
[extern irqHandler]

isr_common:
	;	saving the cpu state:
	pusha
	mov ax, ds	; mov ds into lower 16 bit of eax
	push eax
	mov ax, 0x10	; data segment descriptor
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	; calling C handler
	call isrHandler
	; restoring the sate of the cpu
	pop eax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	popa
	add esp, 8
	sti		; set interrupts again.
	iret	; pop eip, cs, eflgs, useresp, ss at the same time.

; common IQR handler:
irq_common:
	pusha
	mov ax, ds
	push eax
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	call irqHandler

	pop ebx
	mov ds, bx
	mov es, bx
	mov fs, bx
	mov gs, bx
	popa
	add esp, 8
	sti
	iret

; macro to define an isr that has no error code
%macro ISR_NOERRCODE 1
  [GLOBAL isr%1]
  isr%1:
    cli
    push byte 0
    push byte %1
    jmp isr_common
%endmacro
; macro to define an isr that has an error code
%macro ISR_ERRCODE 1
  [GLOBAL isr%1]
  isr%1:
    cli
    push byte %1
    jmp isr_common
%endmacro

%macro IRQ 2
	[GLOBAL irq%1]
	irq%1:
	cli
	push byte %1
	push byte %2
	jmp irq_common
%endmacro

; defining all the ISRs
ISR_NOERRCODE	0	; 0: Division Error
ISR_NOERRCODE	1	; 1: Debug
ISR_NOERRCODE	2	; 2: Non-Maskable Interrupt
ISR_NOERRCODE	3	; 3: Breakpoint
ISR_NOERRCODE	4	; 4: OverFlow
ISR_NOERRCODE	5	; 5: Bound range (out of bounds)
ISR_NOERRCODE	6	; 6: Invalid Opcode
ISR_NOERRCODE	7	; 7: Device Not Available
ISR_ERRCODE		8	; 8: Double Fault
ISR_NOERRCODE	9	; 9: Coprocessor Segment Overrun
ISR_ERRCODE		10	; 10: Invalid TSS
ISR_ERRCODE		11	; 11: Segment Not Present
ISR_ERRCODE		12	; 12: Stack-Segment Fault
ISR_ERRCODE		13	; 13: General Protection Fault
ISR_ERRCODE		14	; 14: Page Fault
ISR_NOERRCODE	15	; 15: Reserved
ISR_NOERRCODE	16	; 16: Floating Point Exception
ISR_ERRCODE		17	; 17: Alignment Check
ISR_NOERRCODE	18	; 18: Machine Check
ISR_NOERRCODE	19	; 19: SIMD Floating-Point Exception
ISR_NOERRCODE	20	; 20: Virtualization Exception
ISR_ERRCODE		21	; 21: Control Protection Exception
ISR_NOERRCODE	22	; 22: Reserved
ISR_NOERRCODE	23	; 23: Reserved
ISR_NOERRCODE	24	; 24: Reserved
ISR_NOERRCODE	25	; 25: Reserved
ISR_NOERRCODE	26	; 26: Reserved
ISR_NOERRCODE	27	; 27: Reserved
ISR_NOERRCODE	28	; 28: Hypervisor Injection Exception
ISR_ERRCODE		29	; 29: VMM Communication Exception
ISR_ERRCODE		30	; 30: Security Exception
ISR_NOERRCODE	31	; 31: Reserved

; defining all the IRQs
IRQ		0,		32	; 0:  System timer
IRQ		1,		33	; 1:  Keyboard Controller
IRQ		2,		34	; 2:  Receives signals from IRQs 8-15 (cascading)
IRQ		3,		35	; 3:  ??
IRQ		4,		36	; 4:  ??
IRQ		5,		37	; 5:  ??
IRQ		6,		38	; 6:  ??
IRQ		7,		39	; 7:  ??
IRQ		8,		40	; 8:  ??
IRQ		9,		41	; 9:  ??
IRQ		10,		42	; 10: ??
IRQ		11,		43	; 11: ??
IRQ		12,		44	; 12: ??
IRQ		13,		45	; 13: ??
IRQ		14,		46	; 14: ??
IRQ		15,		47	; 15: ??