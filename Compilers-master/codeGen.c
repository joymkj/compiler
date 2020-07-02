//Group 7:
//Prashant Rangarajan 2015B4A70557P
//Vighnesh Hegde 2015B4A70342P
//Biswajit Biswas 2015B5A70418P
//Joy Mukherjee 2015B5A70394P
#include "codeGen.h"

Code initialiseCode(){
    Code c = malloc(sizeof(code));
    c->data = initialiseData();
    c->bss = initialiseBSS();
    c->text = initialiseText();
    return c;
}
char* initialiseData(){
    char* data = malloc(strlen("SECTION .data\n"));
    strcpy(data,"SECTION .data\n");
    return data;
}

char* initialiseText(){
    char* text = malloc(strlen("\nSECTION .text\nglobal  _start\n_start:\n"));
    strcpy(text,"\nSECTION .text\nglobal  _start\n_start:\n");
    return text;
}

char* initialiseBSS(){
    char* bss = malloc(strlen("SECTION .bss\n"));
    strcpy(bss,"SECTION .bss\n");
    return bss;
}

void printToFile(FILE* f,Code cd){
    fwrite(cd->data,1,strlen(cd->data),f);
    fwrite(cd->bss,1,strlen(cd->bss),f);
    fwrite(cd->text,1,strlen(cd->text),f);
}

void terminate(FILE *f){
    char exit[12];
    sprintf(exit,
    "\tcall\texit\n"
    );
    fwrite(exit,1,strlen(exit),f);
}

void addToBlock(char** data,char*append){//data has to be allocated in heap for this function to work
    char* str = malloc(strlen(*data)+strlen(append)+1);
    strcpy(str,*data);
    strcat(str,append);
    data = &str;
    //printf("addToBlock: %s\n",*data);
    // free(append);//uncomment this later
}

void addToBSSBlock(Code cd,char*append){
//    char* str = malloc(strlen(cd->bss)+strlen(append)+1);
    cd->bss = realloc(cd->bss,strlen(cd->bss)+strlen(append)+1);
    //strcpy(str,cd->bss);
    strcat(cd->bss,append);
    // free(cd->bss);
    // cd->bss = str;
    free(append);//uncomment this later
}

void addToTextBlock(Code cd,char*append){
    // char* str = malloc(strlen(cd->text)+strlen(append)+1);
    cd->text = realloc(cd->text,strlen(cd->text)+strlen(append)+1);

    // strcpy(str,cd->text);
    strcat(cd->text,append);
    // free(cd->text);
    // cd->text = str;
    // printf("%s\n",cd->text);
    // free(append);//uncomment this later
}


void appendRead(Code cd,char*id){
    char* str = malloc(106+strlen(id)*3);
    sprintf(str,"\tmov \tedx, 8 \n"
                "\tmov \tecx, %s\n"
                "\tmov \tebx, 0 \n"
                "\tmov \teax, 3 \n"
                "\tint \t80h\n"
                "\tmov \teax,%s\n"
                "\tcall\tatoi\n"
                "\tmov \t[%s],eax\n\n",id,id,id);
    addToTextBlock(cd,str);
}

void appendWrite(Code cd,char*id){
    char* str = malloc(34+strlen(id));
    sprintf(str,"\tmov\t\teax, [%s]\n"
                "\tcall\tiprintLF\n\n",id);
    addToTextBlock(cd,str);
}

void appendWriteLiteral(Code cd,char*id){
    char* str = malloc(33);
    sprintf(str,"\tmov\t\teax, %s\n"
                "\tcall\tiprintLF\n\n",id);
    addToTextBlock(cd,str);
}

void appendDecl(Code cd,char*id){
    char* str = malloc(12);
    sprintf(str,"%s:\tresb\t8\n",id);
    // printf("%s\n",id);
    addToBSSBlock(cd,str);
}



char* appendArithCodeOp(int tag){
    if(tag==PLUS){
        return "\tadd\t\teax,edx\n";
    }
    if(tag==MINUS){
        return "\tsub\t\teax,edx\n";
    }
    if(tag==MUL){
        return "\tmul\t\tedx\n";
    }
    if(tag==DIV){
        return  "\tpush\t ecx\n"
                "\tpush\t edx\n"
                "\tmov\t\t ecx,edx\n"
                "\txor\t\t edx,edx\n"
                "\tdiv\t\t ecx\n"
                "\tpop\t\t edx\n"
                "\tpop\t\t ecx\n";
    }
    return NULL;
}
char* getJump(int tag){
    switch (tag)
    {
        case GT:
            return "jle";
        case GE:
            return "jl";
        case LT:
            return "jge";
        case LE:
            return "jg";
        case EQ:
            return "jne";
        case NE:
            return "jeq";
        default:
            printf("Unknown tag %d\n",tag);
            break;
    }
    return NULL;
}
char* appendBoolCode(Code cd,ASTNode root){
    char *cond,*l,*r;
    switch (root->tag)
    {
        case GT:
        case GE:
        case LT:
        case LE:
        case EQ:
        case NE:
            l = appendBoolCode(cd,root->firstChild);
            r =appendBoolCode(cd, root->lastChild);
            //this handles only id. handle 
            cond = malloc(sizeof(l)+sizeof(r)+52);
            sprintf(cond,"\tmov\t\teax,%s\n"
            "\tmov\t\tedx,%s\n"
            "\tcmp\t\teax,edx\n"
            "\t%s\t\t",
            l,r,getJump(root->tag)
            );
            return cond;
        case ID:
            l = malloc(strlen(root->self->sOrR->id->lexeme)+5);
            sprintf(l,"[%s]",root->self->sOrR->id->lexeme);
            return l;
        case RNUM:
            return root->self->sOrR->id->lexeme;
        case NUM:
            return root->self->sOrR->id->lexeme;
        default:
            printf("Unknown tag %d\n",root->tag);
            break;
    }
    return NULL;
}

char* appendArithCodeRec(Code cd,ASTNode root,char* fld){
    if(PLUS<=root->tag&root->tag<=DIV){
        char* lhs = appendArithCodeRec(cd,root->firstChild,fld);
        char* rhs = appendArithCodeRec(cd,root->lastChild,fld);
        char* op = appendArithCodeOp(root->tag);
        char* str = malloc(65+strlen(lhs)+strlen(rhs)+strlen(op));
        sprintf(str,"\tpush\teax\n"
                    "%s"//RHS
                    "\tmov\t\tedx, eax\n"
                    "\tpop\t\teax\n"
                    "\tpush\tedx\n"
                    "%s"//LHS
                    "\tpop\t\tedx\n"
                    "%s\n",
                    rhs,lhs,op
        );
    return str;
    }
    else if(root->tag==ASGN){
        char* str = malloc(15);
        sprintf(str,"\tmov\t\tedx,0\n\n");
        addToTextBlock(cd,str);
        str = appendArithCodeRec(cd,root->firstChild,fld);
        addToTextBlock(cd,str);
        SingleOrRecID s1 = root->self->sOrR;
        // int tag = getTag(s1);
        str = malloc(22+strlen(s1->id->lexeme)+strlen(fld));
        sprintf(str,"\tmov\t\t[%s.%s], eax\n\n",s1->id->lexeme,fld);
        addToTextBlock(cd,str);
        return "";
    }
    else {
        char* str = malloc(20+strlen(fld)+strlen(root->self->sOrR->id->lexeme));
        sprintf(str,"\tmov\t\teax, [%s.%s]\n",root->self->sOrR->id->lexeme,fld);
        return str;
    }
}

char* appendArithCode(Code cd,ASTNode root){
            
    if(PLUS<=root->tag&root->tag<=DIV){
        char* lhs = appendArithCode(cd,root->firstChild);
        char* rhs = appendArithCode(cd,root->lastChild);
        char* op = appendArithCodeOp(root->tag);
        char* str = malloc(65+strlen(lhs)+strlen(rhs)+strlen(op));
        sprintf(str,"\tpush\teax\n"
                    "%s"//RHS
                    "\tmov\t\tedx, eax\n"
                    "\tpop\t\teax\n"
                    "\tpush\tedx\n"
                    "%s"//LHS
                    "\tpop\t\tedx\n"
                    "%s\n",
                    rhs,lhs,op
        );
    return str;
    }
    // else if(root->tag){
    //     //printf("%d\n",root->tag);
    // }
    else if(root->tag==REC_ID){
        char* str = malloc(18+strlen(root->self->sOrR->id->lexeme));
        sprintf(str,"\tmov\t\teax, [%s.%s]\n",root->self->sOrR->recName,root->self->sOrR->id->lexeme);
        return str;        
        // printf("this case not handled yet.\n");
        // exit(1);
    }
    else if(root->tag==ASGN){
        char* str = malloc(15);
        sprintf(str,"\tmov\t\tedx,0\n\n");
        addToTextBlock(cd,str);
        str = appendArithCode(cd,root->firstChild);
        addToTextBlock(cd,str);
        SingleOrRecID s1 = root->self->sOrR;
        int tag = getTag(s1);
        if(tag==REC_ID){
            str = malloc(22+strlen(s1->id->lexeme)+strlen(s1->recName));
            sprintf(str,"\tmov\t\t[%s.%s], eax\n\n",s1->recName,s1->id->lexeme);
        }
        else
        {
            str = malloc(19+strlen(s1->id->lexeme));
            sprintf(str,"\tmov\t\t[%s], eax\n\n",s1->id->lexeme);
        }
        addToTextBlock(cd,str);
        return "";
    }
    else if(root->tag==NUM){
        char* str = malloc(16+strlen(root->self->sOrR->id->lexeme));
        sprintf(str,"\tmov\t\teax, %s\n",root->self->sOrR->id->lexeme);
        return str;
        }
    else {
        //printf("%d\n",root->tag);
        char* str = malloc(18+strlen(root->self->sOrR->id->lexeme));
        sprintf(str,"\tmov\t\teax, [%s]\n",root->self->sOrR->id->lexeme);
        return str;
    }
}

void generateCode(ASTNode root,VariableTable VT,VariableTable GVT, RecordTable RT,Code cd){
    Node decl,recl;
    char* name,*str;
    ASTNode t;
    int tag;
    switch (root->tag)
    {
        case PROG:
            generateCode(root->lastChild,NULL,GVT,RT,cd);
            break;

        case MAIN:
            generateCode(root->firstChild,root->self->f->localVars,GVT,RT,cd);
            break;

        case STMTS:
            decl = root->self->s->vl->head;
            while(decl!=NULL){
                if(((Var)decl->e)->info->t->lexeme[0]=='#'){//means record
                    name = ((Var)decl->e)->name;
                    recl = isRecDefined(RT,((Var)decl->e)->info)->lf->head;
                    while(recl!=NULL){//iterate through the list of fields
                        str = malloc(strlen(name)+strlen(((Field)recl->e)->info->lexeme)+2);
                        sprintf(str,"%s.%s",name,((Field)recl->e)->info->lexeme);
                        appendDecl(cd,str);
                        free(str);
                        recl=recl->next;
                    }
                }
                else appendDecl(cd,((Var)decl->e)->name);
                decl=decl->next;
            }
            t = root->firstChild;
            while(t!=NULL){
                generateCode(t,VT,GVT,RT,cd);
                t=t->next;
            }
            break;
        case IORD:
            name = root->self->sOrR->recName;
            if(getTag(root->self->sOrR)==REC_ID){
                str = malloc(strlen(name)+strlen(root->self->sOrR->id->lexeme)+2);
                sprintf(str,"%s.%s",name,root->self->sOrR->id->lexeme);
                appendRead(cd,str);
                free(str);
            }
            else if(getTag(root->self->sOrR)==ID){
                appendRead(cd,root->self->sOrR->id->lexeme);
            }
            else{
                printf("Unsupported read.\n");
            }
            break;
        case IOWR:
            name = getIDType(GVT,VT,root->self->sOrR->id);
            if(name[0]=='#'){//means record
                TokOrRec tor = getRecFromTable(RT,GVT,VT,root->self->sOrR->id->lexeme);

                if(tor!=NULL){
                    Node field = tor->lf->head;
                    while(field!=NULL){
                        //these don't make too much sense because some naming is messed up
                        name = malloc(sizeof(root->self->sOrR->id->lexeme)+3+
                            sizeof(((Field)field->e)->info->lexeme));
                        sprintf(name,"%s.%s",root->self->sOrR->id->lexeme,
                        ((Field)field->e)->info->lexeme);
                        appendWrite(cd,name);
                        field = field->next;                    
                        }
                }
            }
            else if(getTag(root->self->sOrR)==REC_ID){
                name = malloc(sizeof(root->self->sOrR->recName)+2+
                    sizeof(root->self->sOrR->id->lexeme));
                sprintf(name,"%s.%s",root->self->sOrR->recName,
                        root->self->sOrR->id->lexeme);
                appendWrite(cd,name);
            }
            else if(getTag(root->self->sOrR)==ID){
                appendWrite(cd,root->self->sOrR->id->lexeme);
            }
            else if(getTag(root->self->sOrR)==NUM){
                appendWriteLiteral(cd,root->self->sOrR->id->lexeme);
            }
            else//RNUM
            {
                printf("RNUM not supported yet\n");
            }
            break;
        case ASGN:
            tag = getTag(root->self->sOrR);
            if(tag==REC||tag==ID){
            name = getIDType(GVT,VT,root->self->sOrR->id);
                if(name!=NULL&&name[0]=='#'){//means record
                    // printf("%d %s\n",tag,name);
                    TokOrRec tor = getRecFromTable(RT,GVT,VT,root->self->sOrR->id->lexeme);
                    if(tor!=NULL){
                        recl = tor->lf->head;
                        while(recl!=NULL){
                        appendArithCodeRec(cd,root,((Field)recl->e)->info->lexeme);
                        recl=recl->next;
                        }
                    }
                }
                else appendArithCode(cd,root);
            }
            else appendArithCode(cd,root);
            break;
        case ITER:
            addToTextBlock(cd,"\t.loop:\n");
            name = appendBoolCode(cd,root->firstChild);
            addToTextBlock(cd,name);
            addToTextBlock(cd,".endloop\n");
            t = root->firstChild->next;
            while(t!=NULL){
                generateCode(t,VT,GVT,RT,cd);
                t=t->next;
                //printf("%d\n",t->tag);
            }
            addToTextBlock(cd,"\tjmp\t\t.loop\n\t.endloop:\n");
            break;
        case COND:
            name = appendBoolCode(cd,root->firstChild);
            addToTextBlock(cd,name);
            addToTextBlock(cd,".else\n");
            t = root->firstChild->next;
            while(t!=NULL){
                generateCode(t,VT,GVT,RT,cd);
                t=t->next;
                //printf("%d\n",t->tag);
            }
            break;
        case ELSE:
            addToTextBlock(cd,"\tjmp\t\t.endif\n\t.else:\n");
            t = root->firstChild;
            while(t!=NULL){
                generateCode(t,VT,GVT,RT,cd);
                t=t->next;
            }
            addToTextBlock(cd,"\t.endif:\n");

        default:
            break;
    }
}



void printSubRoutines(FILE *f){//call this in the end to append all the helper functions
    char str[] =     "\tcall\texit\n\n" 
            "atoi:\n"
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
            "exit:\n"
            "    mov     ebx, 0\n"
            "    mov     eax, 1\n"
            "    int     80h\n"
            "    ret\n";
    fwrite(str,1,strlen(str),f);
    fclose(f);
}
