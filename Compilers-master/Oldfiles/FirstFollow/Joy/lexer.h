#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<unistd.h>


typedef struct
{
    char * lexeme;
    int token;
    int lineNo;
    // int value;
}tokenInfo;
typedef tokenInfo* TokenInfo;


typedef struct
{
    FILE* fp;
    char* buf;
    int ptr;
    int end;
    int size;
}lexBuf;
typedef lexBuf* LexBuf;

LexBuf newBuf(FILE* f,int size);
TokenInfo getToken(LexBuf fp);
int isKeyword(char* lexeme);// implement using hashtable. Trie is better?
char nextChar();
void removeComments(char *testcaseFile, char *cleanFile);
char* getKeyword(int token);
TokenInfo makeTok(char* lexeme, int token, int lineNo);
void printToken(TokenInfo tk);
int bgetc(LexBuf b);
int retracc(LexBuf b);




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
