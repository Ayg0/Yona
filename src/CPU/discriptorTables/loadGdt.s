[GLOBAL loadGdt]

; 0->7 null segment; 8->0xF code Segment and 0x10->0x17 data Segment
CODE_S equ 0x08		; where our code segment is located
DATA_S equ 0x10		; where our data segment is located
STACK_S equ 0x18	; where our stack segment is located

loadGdt:
	mov	eax, [esp + 4]	; load eax with our gdt-Ptr
	lgdt [eax]			; flush the gdt with new vals

	mov ax, DATA_S				; set the segment registers with the new data segment addr
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ax, STACK_S
	mov ss, ax
	
	jmp CODE_S:reloadCS	; do a far jump to flush the instruction prefetch queue and set CS to the new code segment

reloadCS:
	ret