%include        'functions.asm'

SECTION .data
msg1        db      'Please enter number: ', 0h      ; message string asking user for input
msg2        db      'Answer: ', 0h                       ; message string to use after user has entered their name
msg3        db      20

SECTION .bss
b.b3:     resb    8                                 ; reserve a 255 byte space in memory for the users input string
b2:     resb    8
c2:     resb    8
d2:     resb    8

SECTION .text
global  _start
 
_start:
     
    mov     edx, 8        ; number of bytes to read
    mov     ecx, b2     ; reserved space to store our input (known as a buffer)
    mov     ebx, 0          ; write to the STDIN file
    mov     eax, 3          ; invoke SYS_READ (kernel opcode 3)
    int     80h
    mov     eax,b2
    call    atoi
    mov     [b2],eax

    mov     eax,20
    mov     [c2],eax

    mov     edx, 8        ; number of bytes to read
    mov     ecx, d2     ; reserved space to store our input (known as a buffer)
    mov     ebx, 0          ; write to the STDIN file
    mov     eax, 3          ; invoke SYS_READ (kernel opcode 3)
    int     80h
    mov     eax,d2
    call    atoi
    mov     [d2],eax

    mov     edx,0
    mov     eax,[c2]
    mov     edx,eax
    
    mov     eax,[d2]
    add     eax,edx
    mov     edx,eax

    mov     eax,[b2]
    
    add     eax,edx
    mov     [b.b3],eax

    mov     eax, 2
    call    iprintLF
  
    call    quit
