#include<stdio.h>
#include"lexer.h"

char* kw[] = {"_main","call","else","end","endif","endrecord","endwhile","global",
"if","input","int","list","output","parameter","parameters","read","real","record","return",
"then","type","while","with","write"};

int isKeyword(char* lexeme,RevLookupHashTable HT){
    int ret = isDuplicate(HT,lexeme);
    if(ret==-NO_TERMINALS) return 0;
    else return ret;
}

char* getKeyword(int token,RevLookupHashTable HT){
    return kw[-1*(token)-1];
}

RevLookupEntry newRevLookupEntry(int id,char* name)
{
    RevLookupEntry newEntry = (RevLookupEntry)malloc(sizeof(struct revLookupEntry));
    newEntry->id = id;
    newEntry->name = (char*)malloc(strlen(name)*sizeof(char));
    strcpy(newEntry->name,name);
    newEntry->next = NULL;
    return newEntry;
}

RevLookupHashTable newKeyTable()
{
    RevLookupHashTable HT = (RevLookupHashTable)malloc(sizeof(struct revLookupHashTable));
    HT->table = (RevLookupEntry*)malloc(NO_REVLOOKUP_HASHBUCKETS*sizeof(RevLookupEntry));
    for(int i = 0; i<NO_REVLOOKUP_HASHBUCKETS ; i++)
    {
        HT->table[i] = NULL;
    }
    addNewRevLookupEntry(HT, 0,"eps");
    addNewRevLookupEntry(HT, TK_MAIN,"_main");
    addNewRevLookupEntry(HT, TK_CALL,"call");
    addNewRevLookupEntry(HT, TK_ELSE,"else");
    addNewRevLookupEntry(HT, TK_END,"end");
    addNewRevLookupEntry(HT, TK_ENDIF,"endif");
    addNewRevLookupEntry(HT, TK_ENDRECORD,"endrecord");
    addNewRevLookupEntry(HT, TK_ENDWHILE,"endwhile");
    addNewRevLookupEntry(HT, TK_GLOBAL,"global");
    addNewRevLookupEntry(HT, TK_IF,"if");
    addNewRevLookupEntry(HT, TK_INPUT,"input");
    addNewRevLookupEntry(HT, TK_INT,"int");
    addNewRevLookupEntry(HT, TK_LIST,"list");
    addNewRevLookupEntry(HT, TK_OUTPUT,"output");
    addNewRevLookupEntry(HT, TK_PARAMETER,"parameter");
    addNewRevLookupEntry(HT, TK_PARAMETERS,"parameters");
    addNewRevLookupEntry(HT, TK_READ,"read");
    addNewRevLookupEntry(HT, TK_REAL,"real");
    addNewRevLookupEntry(HT, TK_RECORD,"record");
    addNewRevLookupEntry(HT, TK_RETURN,"return");
    addNewRevLookupEntry(HT, TK_THEN,"then");
    addNewRevLookupEntry(HT, TK_TYPE,"type");
    addNewRevLookupEntry(HT, TK_WHILE,"while");
    addNewRevLookupEntry(HT, TK_WITH,"with");
    addNewRevLookupEntry(HT, TK_WRITE,"write");
    //initialising keywords into the hashtable

    return HT;
}
void addNewRevLookupEntry(RevLookupHashTable HT, int id, char* name)
{
    RevLookupEntry newEntry = newRevLookupEntry(id,name);
    int index = hashFunc(name);
    if(HT->table[index]==NULL)
        HT->table[index] = newEntry;
    else{
        RevLookupEntry indexHead = HT->table[index];
        while(indexHead->next!=NULL)
            indexHead=indexHead->next;

        indexHead->next = newEntry;
    }
}

int isDuplicate(RevLookupHashTable HT, char *name)
{
    int index = hashFunc(name);
    RevLookupEntry head = HT->table[index];
    while(head != NULL){
        if(strcmp(name,head->name)==0)
            return head->id;
        head = head->next;
    }
    return -NO_TERMINALS;
}

int hashFunc(char* name){
    unsigned int hash = 0;
    int c=0;
    int factor=HASHFACTOR;
    while ((c = *name++)){
        hash += c*factor;
    }
    return hash%NO_REVLOOKUP_HASHBUCKETS ;
}

TokenInfo makeTok(char* lexeme, int token, int lineNo){
    TokenInfo tk = malloc(sizeof(tokenInfo));
    tk->lexeme = malloc(strlen(lexeme)*sizeof(char));
    strcpy(tk->lexeme,lexeme);
    tk->token = token;
    tk->lineNo = lineNo;
    if(token==TK_RNUM) tk->value.rnum = atof(lexeme);
    else if(token==TK_NUM) tk->value.num = atoi(lexeme);
    return tk;
}

void printToken(TokenInfo tk){
    printf("Token %d, LineNo %d, Lexeme %s\n",tk->token,tk->lineNo,tk->lexeme);
}

LexBuf newBuf(char* fname,int size,RevLookupHashTable ht){
    FILE* f = fopen(fname,"r");
    if(f==NULL){
        perror("fopen");
        printf("Quitting because of file read error.\n");
        exit(0);
    }
    LexBuf b = malloc(sizeof(lexBuf));
    b->fp = f;
    b->size = size; 
    b->buf = malloc(2*size*sizeof(char));
    b->ptr = 0;
    int nread = fread(b->buf+b->ptr,sizeof(char),b->size,b->fp);
    b->end = nread;
    b->ht = ht;
    return b;
}

int bgetc(LexBuf b){
    if(b->ptr==b->end){//means we need to read more now.
        if(feof(b->fp)) {
            //fclose(b->fp);
            return -1;//there is nothing to read
        }
        int nread = fread(b->buf+b->ptr,sizeof(char),b->size,b->fp);
        b->end = (b->end+nread)%(2*b->size);//maintaining circular buffer
    }

    char c = b->buf[b->ptr];
    //if(c=='\0') return c;
    b->ptr = (b->ptr+1)%(2*b->size);
    return c;
}

int retracc(LexBuf b){
    b->ptr = (2*b->size+b->ptr-1)%(2*b->size);
    //return ungetc(c,fp);
    return b->buf[b->ptr];
}

TokenInfo getToken(LexBuf b){
    static int lineNo=1;//ALMOST LIKE GLOBAL
    int state = 0, flag=0;
    char* lexName; //lexName can only be 30
    int lexLen = 0;
    char c;// = fgetc(b);
    while(flag==0){
        c = bgetc(b);
        if (c==-1){
            lexLen = lineNo;//reusing this variable 
            lineNo = 1;//setting this to 1 so as to not affect next calls of getToken
            return makeTok("$",DOLLAR,lexLen);
        }
        if(state == 0)
                switch(c){
                    case ' ': continue;
                    case '\t': continue;
                    case '\n': lineNo++; continue;
                    case '-': return makeTok("-",TK_MINUS,lineNo); 
                    case '~': return makeTok("~",TK_NOT,lineNo);
                    case '+': return makeTok("+",TK_PLUS,lineNo);
                    case '(': return makeTok("(",TK_OP,lineNo);
                    case ')': return makeTok(")",TK_CL,lineNo);
                    case '*': return makeTok("*",TK_MUL,lineNo);
                    case ',': return makeTok(",",TK_COMMA,lineNo);
                    case '.': return makeTok(".",TK_DOT,lineNo);
                    case '/': return makeTok("/",TK_DIV,lineNo);
                    case ':': return makeTok(":",TK_COLON,lineNo);
                    case ';': return makeTok(";",TK_SEM,lineNo);
                    case '[': return makeTok("[",TK_SQL,lineNo);
                    case ']': return makeTok("]",TK_SQR,lineNo);
                    case '!': state = 1; continue;
                    case '&': state = 2; continue;
                    case '@': state = 4; continue;
                    case '=': state = 6; continue;
                    case '%': state = 7; continue;
                    case '<': state = 8; continue;
                    case '>': state = 11; continue;
                    case '#': 
                        state = 12; 
                        lexName = calloc(31,sizeof(char));//RECORDID size not given, assuming 30
                        lexName[0] = c;
                        continue;
                    case '_':
                        state = 14;
                        lexName = calloc(31,sizeof(char));//FUNID size not given, assuming 30
                        lexName[0] = c;
                        continue;
                    default: 
                        if(c=='b'||c=='c'||c=='d') {
                            state =17;
                            lexName = calloc(30,sizeof(char));//allocating 30 
                            lexName[0] = c;
                        }
                        else if(c=='a'||(c>='d'&&c<='z')) {
                            state = 20;
                            lexName = calloc(30,sizeof(char));
                            lexName[0] = c;
                        }
                        else if(c>='0'&&c<='9'){
                            state = 21;
                            lexName = calloc(30,sizeof(char));
                            lexName[0] = c;
                        }
                        else{
                            printf(RED"Line %d:"RESET" Unknown symbol %c\n",lineNo,c);
                        }
                        continue;
                }
        switch(state){
            case 1: 
                if(c=='=') return makeTok("!=",TK_NE,lineNo);
                else {
                    printf(RED"Line %d:"RESET" Unknown pattern !\n",lineNo);
                    retracc(b);
                    continue;
                }
                break;
            case 2:
                if(c=='&') state = 3;
                else {
                    printf(RED"Line %d:"RESET" Unknown pattern &\n",lineNo);
                    retracc(b);
                    state = 0;                     
                    continue;
                }
                break;
            case 3:
                if(c=='&') return makeTok("&&&",TK_AND,lineNo);
                else {
                    printf(RED"Line %d:"RESET" Unknown pattern &&\n",lineNo);
                    retracc(b);
                    state = 0;                     
                    continue;
                }
                break;
            case 4:
                if(c=='@') state = 5;
                else {
                    printf(RED"Line %d:"RESET" Unknown pattern @\n",lineNo);
                    retracc(b);
                    state = 0;                     
                    continue;
                }
                break;
            case 5:
                if(c=='@') return makeTok("@@@",TK_OR,lineNo);
                else {
                    printf(RED"Line %d:"RESET" Unknown pattern @@\n",lineNo);
                    retracc(b);
                    state = 0;                     
                    continue;
                }
                break;
            case 6:
                if(c=='=') return makeTok("==",TK_EQ,lineNo);
                else {
                    printf(RED"Line %d:"RESET" Unknown pattern =\n",lineNo);
                    retracc(b);
                    state = 0;                     
                    continue;
                }
                break;
            case 7:
                if(c=='\n') {
                    state = 0;
                    lineNo++;
                }
                break;
            case 8:
                if(c=='=') return makeTok("<=",TK_LE,lineNo);
                else if(c=='-') state = 9;
                else {
                    retracc(b);
                    return makeTok("<",TK_LT,lineNo);
                }
                break;
            case 9:
                if(c=='-') state = 10;
                else {
                    printf(RED"Line %d:"RESET" Unknown pattern <-\n",lineNo);
                    // flag =1;
                    retracc(b);
                    state = 0;                     
                    continue;
                }
                break;
            case 10:
                if(c=='-') return makeTok("<---",TK_ASSIGNOP,lineNo);
                else {
                    printf(RED"Line %d:"RESET" Unknown pattern <--\n",lineNo);
                    // flag =1;
                    retracc(b);
                    state = 0;
                    continue;
                }
                break;
            case 11:
                if(c=='=') return makeTok(">=",TK_GE,lineNo);
                else{
                    retracc(b);
                    return makeTok(">",TK_GT,lineNo);
                }
            case 12:
                if(c>='a'&&c<='z'){
                    state = 13;
                    lexName[++lexLen]=c;
                }
                else {
                    printf(RED"Line %d:"RESET" # must be followed with recordID\n",lineNo);
                    // flag =1;
                    retracc(b);
                    state = 0; 
                    continue;
                }
                break;
            case 13:
                if((c>='a'&&c<='z')){
                    if(lexLen>=29){
                        printf(RED"Line %d:"RESET" Identifier is longer than the prescribed length of 30 characters.\n"
                        RED"Warning:"RESET" It will be truncated to 30 characters\n",lineNo);
                        while(c>='a'&&c<='z')c=bgetc(b);//flush out remaining characters
                        retracc(b);
                        return makeTok(lexName,TK_RECORDID,lineNo);
                    }
                    state = 13;
                    lexName[++lexLen] = c;
                }
                else{
                    retracc(b);
                    return makeTok(lexName,TK_RECORDID,lineNo);
                }
                break;
            case 14:
                if((c>='a'&&c<='z')||(c>='A'&&c<='Z')){
                    state = 15;
                    lexName[++lexLen]=c;
                }
                else {
                    printf(RED"Line %d:"RESET" _ must be followed by a letter.\n",lineNo);
                    // flag =1;
                    retracc(b);
                    state = 0;
                    continue;
                }
                break;
            case 15:
                if(lexLen>=29){
                    printf(RED"Line %d:"RESET" Identifier is longer than the prescribed length of 30 characters.\n"
                    RED"Warning:"RESET" It will be truncated to 30 characters\n",lineNo);
                    while((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c>='0'&&c<='9'))c=bgetc(b);
                    //flush out remaining characters
                    retracc(b);
                    int tok =  isKeyword(lexName,b->ht);
                    if(tok==0) return  makeTok(lexName,TK_FUNID,lineNo);
                    else return makeTok(getKeyword(tok,b->ht),tok,lineNo);
                }            
                else if(((c>='a'&&c<='z')||(c>='A'&&c<='Z'))){
                    state = 15;
                    lexName[++lexLen] = c;
                }
                else if(c>='0'&&c<='9'){
                    state = 16;
                    lexName[++lexLen] = c;
                }
                else{
                    retracc(b);
                    int tok =  isKeyword(lexName,b->ht);
                    if(tok==0) return makeTok(lexName,TK_FUNID,lineNo);
                    else return makeTok(getKeyword(tok,b->ht),tok,lineNo);
                }
                break;
            case 16:
                if(lexLen>=29){
                    printf(RED"Line %d:"RESET" Identifier is longer than the prescribed length of 30 characters.\n"
                    RED"Warning:"RESET" It will be truncated to 30 characters\n",lineNo);
                    while(c>='0'&&c<='9')c=bgetc(b);//flush out remaining characters
                    retracc(b);//ideally check for keyword, but we don't have any that longso it is fine.
                    return  makeTok(lexName,TK_FUNID,lineNo);
                }  
                else if(c>='0'&&c<='9'){
                    state = 16;
                    lexName[++lexLen] = c;
                }
                else{
                    retracc(b);
                    int tok =  isKeyword(lexName,b->ht);
                    if(tok==0) return makeTok(lexName,TK_FUNID,lineNo);
                    else return makeTok(getKeyword(tok,b->ht),tok,lineNo);
                }
                break;
            case 17:
                //lexName = calloc(20,sizeof(char));//ID size 20
                lexName[++lexLen]=c;
                if(c>='2'&&c<='7'){
                    state = 18;
                }
                else if(c>='a'&&c<='z'){
                    state = 20;  
                }
                else{
                    retracc(b);
                    int tok =  isKeyword(lexName,b->ht);
                    if(tok==0) return makeTok(lexName,TK_FIELDID,lineNo);
                    else return makeTok(getKeyword(tok,b->ht),tok,lineNo);                   
                }
                break;
            case 18:
                if(lexLen>=19){
                    printf(RED"Line %d:"RESET" Identifier is longer than the prescribed length of 20 characters.\n"
                    RED"Warning:"RESET" It will be truncated to 20 characters\n",lineNo);
                    while((c>='b'&&c<='d')||(c>='2'&&c<='7'))c=bgetc(b);//flush out remaining characters
                    retracc(b);
                    return  makeTok(lexName,TK_ID,lineNo);
                }  
                else if(c>='b'&&c<='d'){
                    state = 18;
                    lexName[++lexLen] = c;
                }
                else if(c>='2'&&c<='7'){
                    state = 19;
                    lexName[++lexLen] = c;
                }
                else{
                    retracc(b);
                    return makeTok(lexName,TK_ID,lineNo);
                }
                break;
            case 19:
                if(lexLen>=19){
                    printf(RED"Line %d:"RESET" Identifier is longer than the prescribed length of 20 characters.\n"
                    RED"Warning:"RESET" It will be truncated to 20 characters\n",lineNo);
                    while((c>='2'&&c<='7'))c=bgetc(b);//flush out remaining characters
                    retracc(b);
                    return  makeTok(lexName,TK_ID,lineNo);
                }
                else if(c>='2'&&c<='7'){
                    state = 19;
                    lexName[++lexLen] = c;
                }
                else{
                    retracc(b);
                    return makeTok(lexName,TK_ID,lineNo);
                }
                break;
            case 20:
                if(lexLen>=19){
                    printf(RED"Line %d:"RESET" Identifier is longer than the prescribed length of 30 characters.\n"
                    RED"Warning:"RESET" It will be truncated to 30 characters\n",lineNo);
                    while(c>='a'&&c<='z')c=bgetc(b);//flush out remaining characters
                    retracc(b);
                    return  makeTok(lexName,TK_FIELDID,lineNo);
                }
                else if(c>='a'&&c<='z'){
                    state = 20;
                    lexName[++lexLen] = c;
                }
                else{
                    retracc(b);
                    int tok =  isKeyword(lexName,b->ht);
                    if(tok==0) return makeTok(lexName,TK_FIELDID,lineNo);
                    else return makeTok(getKeyword(tok,b->ht),tok,lineNo);                   
                }
                break;
            case 21:
                if(c=='.'){
                    state = 22;
                    lexName[++lexLen] = c;
                }
                else if(c>='0'&&c<='9'){
                    state = 21;
                    lexName[++lexLen] = c;
                }
                else{
                    retracc(b);
                    return makeTok(lexName,TK_NUM,lineNo);
                }
                break;
            case 22:
                if(c>='0'&&c<='9'){
                    state = 23;
                    lexName[++lexLen] = c;
                }
                else{//error
                    lexName[lexLen]='\0';
                    printf(RED"Line %d:"RESET" Unknown pattern %s., assuming number %s\n",lineNo,lexName,lexName);
                    retracc(b);
                    return makeTok(lexName,TK_NUM,lineNo);
                }
                break;
            case 23:
                if(c>='0'&&c<='9'){
                    lexName[++lexLen] = c;
                }
                else{
                    printf(RED"Line %d:"RESET" Unknown pattern %s, assuming Real Number %s0\n",lineNo,lexName,lexName);
                    retracc(b);
                }
                return makeTok(lexName,TK_RNUM,lineNo);
                break;
            default:
                printf("Unknown state. This should never be printed\n");
                flag=1;
                break;
        }
    }
    //printf("Line &d: Unknown symbol %c",lineNo,c);
    return makeTok("Error",0,0);
}

void removeComments(char *testcaseFile, char *cleanFile){
    long bufSize = 512;// sysconf(_SC_PAGESIZE);
    char buf[bufSize];
    int i,j, flag=0;
    size_t nread;
    // FILE* fp = fopen(testcaseFile,"r");
    FILE* fw;
    RevLookupHashTable KT = newKeyTable();
    LexBuf b = newBuf(testcaseFile,bufSize,KT);
    if(strcmp(cleanFile,"1")==0){
        fw = stdout;
    }
    else fw = fopen(cleanFile,"w");
    int c;
    for(i=0;(c=bgetc(b))>=0;){
        if(c=='%'){
	    while((c=bgetc(b))!='\n');
	    continue;
	}
        buf[i%bufSize]=c;
        if(i==bufSize-1) fwrite(buf,1,bufSize,fw);
        i=(i+1)%bufSize;
    }
    if(c==-1) fwrite(buf,1,i,fw);
    //fclose(fp);
    if(strcmp(cleanFile,"1")!=0)fclose(fw);
}


void displayHTData(RevLookupHashTable HT)
{
    for(int i = 0 ; i<NO_REVLOOKUP_HASHBUCKETS ; i++)
    {
        RevLookupEntry head = HT->table[i];
        while(head!=NULL)
        {
            printf("bucket Number: %3d\t term id : %d \tTerm name :%s\n",i,head->id,head->name);
            head = head->next;
        }
    }
}
