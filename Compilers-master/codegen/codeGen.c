#include "codeGen.h"

Code initialiseCode(){
    Code c = malloc(sizeof(code));
    c->data = initialiseData();
    c->bss = initialiseBSS();
    c->text = initialiseText();
    return c;
}

void prnt(FILE* f,char* str){
    char out[1024]; 
    sprintf(out,"SECTION .data\n"
    "msg     db      '%s', 0Ah     ; assign msg variable with your message string\n"
    "SECTION .bss\n"
    "sinput:     resb    255\n" 
    "SECTION .text\n"
    "global  _start\n" 
    "_start:\n"
        "\tmov     edx, 255        ; number of bytes to read\n"
        "\tmov     ecx, sinput     ; reserved space to store our input (known as a buffer)\n"
        "\tmov     ebx, 0          ; write to the STDIN file\n"
        "\tmov     eax, 3          ; invoke SYS_READ (kernel opcode 3)\n"
        "\tint     80h\n"

        "\tmov     edx, %ld     ; number of bytes to write - one for each letter plus 0Ah (line feed character)\n"
        "\tmov     ecx, msg    ; move the memory address of our message string into ecx\n"
        "\tmov     ebx, 1      ; write to the STDOUT file\n"
        "\tmov     eax, 4      ; invoke SYS_WRITE (kernel opcode 4)\n"
        "\tint     80h\n",
    str,strlen(str)+1
    );
    fwrite(out,1,strlen(out),f);

}

void terminate(FILE *f){
    char exit[133];
    sprintf(exit,
    "\tcall   exit\n"
    );
    fwrite(exit,1,strlen(exit),f);
}

void addToBlock(char* data,char*append){//data has to be allocated in heap for this function to work
    data = realloc(data,strlen(data)+strlen(append));
    strcat(data,append);
    // free(append);//uncomment this later
}
char* initialiseData(){
    char* data = malloc(strlen("SECTION .data\n"));
    strcpy(data,"SECTION .data\n");
    return data;
}

char* initialiseText(){
    char* text = malloc(strlen("SECTION .text\nglobal  _start\n_start:\n"));
    strcpy(text,"SECTION .text\nglobal  _start\n_start:\n");
    return text;
}

char* initialiseBSS(){
    char* bss = malloc(strlen("SECTION .bss\n"));
    strcpy(bss,"SECTION .bss\n");
    return bss;
}

int main(){
    FILE* f = fopen("write.asm","w");
    prnt(f,"Code generation");
    terminate(f);
}

void appendRead(char* data,char*id){
    char* str = malloc(106);
    sprintf(str,"\tmov \tedx, 8 \n"
                "\tmov \tecx, %s\n"
                "\tmov \tebx, 0 \n"
                "\tmov \teax, 3 \n"
                "\tint \t80h\n"
                "\tmov \teax,%s\n"
                "\tcall\tatoi\n"
                "\tmov \t[%s],eax\n\n",id,id,id);
    addToBlock(data,str);
}

void appendWrite(char* data,char*id){
    char* str = malloc(33);
    sprintf(str,"\tmov\t\teax, [%s]\n"
                "\tcall\tiprintLF\n\n",id);
    addToBlock(data,str);
}

void appendDecl(char* data,char*id){
    char* str = malloc(13);
    sprintf(str,"%s:\t\tresb\t8\n",id);
    addToBlock(data,str);
}

void printSubRoutines(FILE *f){//call this in the end to append all the helper functions
    char str[] = "atoi:\n"
            "    push    ebx\n"
            "    push    ecx\n"
            "    push    edx\n"
            "    push    esi\n"
            "    mov     esi, eax\n"
            "    mov     eax, 0  \n"
            "    mov     ecx, 0  \n"
            "    ;check minus\n"
            "    mov     edi,0\n"
            "    xor     ebx,ebx\n"
            "    mov     bl,[esi+ecx]\n"
            "    cmp     bl,45       ;45 is minus symbol\n"
            "    jne     .multiplyLoop\n"
            "    mov     edi,1\n"
            "    inc     ecx   \n"
            "\n"
            ".multiplyLoop:\n"
            "    xor     ebx, ebx\n"
            "    mov     bl, [esi+ecx]\n"
            "    cmp     bl, 48\n"
            "    jl      .finished\n"
            "    cmp     bl, 57\n"
            "    jg      .finished\n"
            "    cmp     bl, 10\n"
            "    je      .finished\n"
            "    cmp     bl, 0\n"
            "    jz      .finished    \n"
            "\n"
            "    sub     bl, 48\n"
            "    add     eax, ebx\n"
            "    mov     ebx, 10 \n"
            "    mul     ebx\n"
            "    inc     ecx\n"
            "    jmp     .multiplyLoop\n"
            "\n"
            ".finished:\n"
            "    mov     ebx, 10\n"
            "    div     ebx\n"
            "    cmp     edi,1\n"
            "    jne     .notNeg\n"
            "    not     eax\n"
            "    add     eax,1\n"
            "\n"
            ".notNeg:\n"
            "    pop     esi\n"
            "    pop     edx\n"
            "    pop     ecx\n"
            "    pop     ebx\n"
            "    ret\n"
            "\n"
            "iprint:\n"
            "    push    eax\n"
            "    push    ecx\n"
            "    push    edx\n"
            "    push    esi\n"
            "    mov     ecx, 0\n"
            "    cmp     eax, 0\n"
            "    jge     divideLoop\n"
            "    push    eax\n"
            "    mov     eax, 2Dh\n"
            "    push    eax\n"
            "    mov     eax, esp\n"
            "    call    printHelper\n"
            "    pop     eax\n"
            "    pop     eax\n"
            "    not     eax\n"
            "    add     eax, 1  \n"
            "\n"
            "divideLoop:\n"
            "    inc     ecx    \n"
            "    mov     edx, 0 \n"
            "    mov     esi, 10\n"
            "    idiv    esi    \n"
            "    add     edx, 48\n"
            "    push    edx    \n"
            "    cmp     eax, 0 \n"
            "    jnz     divideLoop\n"
            "\n"
            "printLoop:\n"
            "    dec     ecx\n"
            "    mov     eax, esp\n"
            "    call    printHelper\n"
            "    pop     eax\n"
            "    cmp     ecx, 0\n"
            "    jnz     printLoop\n"
            "    pop     esi\n"
            "    pop     edx\n"
            "    pop     ecx\n"
            "    pop     eax\n"
            "    ret\n"
            "\n"
            "iprintLF:\n"
            "    call    iprint\n"
            "    push    eax\n"
            "    mov     eax, 0Ah\n"
            "    push    eax\n"
            "    mov     eax, esp\n"
            "    call    printHelper\n"
            "    pop     eax\n"
            "    pop     eax\n"
            "    ret\n"
            "plen:\n"
            "    push    ebx\n"
            "    mov     ebx, eax\n"
            " \n"
            "next:\n"
            "    cmp     byte [eax], 0\n"
            "    jz      endstr\n"
            "    inc     eax\n"
            "    jmp     next\n"
            " \n"
            "endstr:\n"
            "    sub     eax, ebx\n"
            "    pop     ebx\n"
            "    ret\n"
            "printHelper:\n"
            "    push    edx\n"
            "    push    ecx\n"
            "    push    ebx\n"
            "    push    eax\n"
            "    call    plen\n"
            " \n"
            "    mov     edx, eax\n"
            "    pop     eax\n"
            " \n"
            "    mov     ecx, eax\n"
            "    mov     ebx, 1\n"
            "    mov     eax, 4\n"
            "    int     80h\n"
            " \n"
            "    pop     ebx\n"
            "    pop     ecx\n"
            "    pop     edx\n"
            "    ret\n"
            "printFunc:\n"
            "    call    printHelper\n"
            "    push    eax\n"
            "    mov     eax, 0Ah\n"
            "    push    eax\n"
            "    mov     eax, esp\n"
            "    call    printHelper\n"
            "    pop     eax\n"
            "    pop     eax\n"
            "    ret\n"
            "quit:\n"
            "    mov     ebx, 0\n"
            "    mov     eax, 1\n"
            "    int     80h\n"
            "    ret\n";
    fwrite(str,1,strlen(str),f);
    fclose(f);
}