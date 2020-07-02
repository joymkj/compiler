SECTION .data
SECTION .bss
b3:	resb	8
b2:	resb	8
c2:	resb	8
d2:	resb	8

SECTION .text
global  _start
_start:
	mov 	edx, 8 
	mov 	ecx, b2
	mov 	ebx, 0 
	mov 	eax, 3 
	int 	80h
	mov 	eax,b2
	call	atoi
	mov 	[b2],eax

	mov		edx,0

	mov		eax, 20
	mov		[c2], eax

	mov 	edx, 8 
	mov 	ecx, d2
	mov 	ebx, 0 
	mov 	eax, 3 
	int 	80h
	mov 	eax,d2
	call	atoi
	mov 	[d2],eax

	mov		edx,0

	push	eax
	push	eax
	mov		eax, [d2]
	mov		edx, eax
	pop		eax
	push	edx
	mov		eax, [c2]
	pop		edx
	add		eax,edx

	mov		edx, eax
	pop		eax
	push	edx
	mov		eax, [b2]
	pop		edx
	add		eax,edx

	mov		[b3], eax

	mov		eax, [b3]
	call	iprintLF

	call	exit

atoi:
    push    ebx
    push    ecx
    push    edx
    push    esi
    mov     esi, eax
    mov     eax, 0  
    mov     ecx, 0  
    ;check minus
    mov     edi,0
    xor     ebx,ebx
    mov     bl,[esi+ecx]
    cmp     bl,45       ;45 is minus symbol
    jne     .multiplyLoop
    mov     edi,1
    inc     ecx   

.multiplyLoop:
    xor     ebx, ebx
    mov     bl, [esi+ecx]
    cmp     bl, 48
    jl      .finished
    cmp     bl, 57
    jg      .finished
    cmp     bl, 10
    je      .finished
    cmp     bl, 0
    jz      .finished    

    sub     bl, 48
    add     eax, ebx
    mov     ebx, 10 
    mul     ebx
    inc     ecx
    jmp     .multiplyLoop

.finished:
    mov     ebx, 10
    div     ebx
    cmp     edi,1
    jne     .notNeg
    not     eax
    add     eax,1

.notNeg:
    pop     esi
    pop     edx
    pop     ecx
    pop     ebx
    ret

iprint:
    push    eax
    push    ecx
    push    edx
    push    esi
    mov     ecx, 0
    cmp     eax, 0
    jge     divideLoop
    push    eax
    mov     eax, 2Dh
    push    eax
    mov     eax, esp
    call    printHelper
    pop     eax
    pop     eax
    not     eax
    add     eax, 1  

divideLoop:
    inc     ecx    
    mov     edx, 0 
    mov     esi, 10
    idiv    esi    
    add     edx, 48
    push    edx    
    cmp     eax, 0 
    jnz     divideLoop

printLoop:
    dec     ecx
    mov     eax, esp
    call    printHelper
    pop     eax
    cmp     ecx, 0
    jnz     printLoop
    pop     esi
    pop     edx
    pop     ecx
    pop     eax
    ret

iprintLF:
    call    iprint
    push    eax
    mov     eax, 0Ah
    push    eax
    mov     eax, esp
    call    printHelper
    pop     eax
    pop     eax
    ret
plen:
    push    ebx
    mov     ebx, eax
 
next:
    cmp     byte [eax], 0
    jz      endstr
    inc     eax
    jmp     next
 
endstr:
    sub     eax, ebx
    pop     ebx
    ret
printHelper:
    push    edx
    push    ecx
    push    ebx
    push    eax
    call    plen
 
    mov     edx, eax
    pop     eax
 
    mov     ecx, eax
    mov     ebx, 1
    mov     eax, 4
    int     80h
 
    pop     ebx
    pop     ecx
    pop     edx
    ret
printFunc:
    call    printHelper
    push    eax
    mov     eax, 0Ah
    push    eax
    mov     eax, esp
    call    printHelper
    pop     eax
    pop     eax
    ret
exit:
    mov     ebx, 0
    mov     eax, 1
    int     80h
    ret
