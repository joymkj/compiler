#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<unistd.h>

#ifndef LEXER
#define LEXER
#define NO_REVLOOKUP_HASHBUCKETS 157
#define NO_TERMINALS 54

typedef struct revLookupEntry *RevLookupEntry;
typedef struct revLookupHashTable *RevLookupHashTable;

typedef union
{
    int num;
    float rnum;
}numOrRnum;

typedef struct 
{
    char * lexeme;
    int token;
    int lineNo;
    numOrRnum value;
}tokenInfo;
typedef tokenInfo* TokenInfo;

typedef struct
{
    FILE* fp;
    char* buf;
    int ptr;
    int end;
    int size;
    RevLookupHashTable ht;
}lexBuf;
typedef lexBuf* LexBuf;

struct revLookupEntry{
    int id;
    char* name;
    RevLookupEntry next;
};

struct revLookupHashTable{
    RevLookupEntry* table;
    int numOfBuckets;
};

LexBuf newBuf(char* fname,int size,RevLookupHashTable ht);
TokenInfo getToken(LexBuf fp);
int isKeyword(char* lexeme,RevLookupHashTable HT);
char nextChar();
void removeComments(char *testcaseFile, char *cleanFile);
char* getKeyword(int token,RevLookupHashTable HT);
TokenInfo makeTok(char* lexeme, int token, int lineNo);
void printToken(TokenInfo tk);
int bgetc(LexBuf b);
int retracc(LexBuf b);

int hashFunc(char* name);
RevLookupEntry newRevLookupEntry(int id,char *name);
RevLookupHashTable newRevLookupHashTable();
void addNewRevLookupEntry(RevLookupHashTable HT, int id, char* name);
int isDuplicate(RevLookupHashTable HT, char *name);//return termID if found else return -NO_TERMINALS
void displayHTData(RevLookupHashTable HT);



#define	TK_MAIN		-1
#define	TK_CALL		-2
#define	TK_ELSE		-3
#define	TK_END		-4
#define	TK_ENDIF		-5
#define	TK_ENDRECORD	-6
#define	TK_ENDWHILE		-7
#define	TK_GLOBAL		-8
#define	TK_IF		-9
#define	TK_INPUT	-10
#define	TK_INT		-11
#define	TK_LIST		-12
#define	TK_OUTPUT		-13
#define	TK_PARAMETER    -14
#define	TK_PARAMETERS	-15
#define	TK_READ		-16
#define	TK_REAL		-17
#define	TK_RECORD	-18
#define	TK_RETURN	-19
#define	TK_THEN		-20
#define	TK_TYPE		-21
#define	TK_WHILE	-22
#define	TK_WITH		-23
#define	TK_WRITE	-24
//keywords until here
#define	TK_ASSIGNOP	-25
#define	TK_FUNID	-26
#define	TK_RECORDID	-27
#define	TK_COMMA	-28
#define	TK_SEM		-29
#define	TK_COLON	-30
#define	TK_NE		-31
#define	TK_DOT		-32
#define	TK_EQ		-33
#define	TK_OP		-34
#define	TK_CL		-35
#define	TK_SQL		-36
#define	TK_NUM		-37
#define	TK_RNUM		-38
#define	TK_FIELDID	-39
#define	TK_ID		-40
#define	TK_SQR		-41
#define	TK_OR		-42
#define	TK_AND		-43
#define	TK_MUL		-44
#define	TK_DIV		-45
#define	TK_PLUS		-46
#define	TK_MINUS	-47
#define	TK_LT		-48
#define	TK_LE		-49
#define	TK_GT		-50
#define	TK_GE		-51
#define	TK_NOT		-52
#define DOLLAR      -53

#endif
