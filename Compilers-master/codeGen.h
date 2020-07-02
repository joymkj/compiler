//Group 7:
//Prashant Rangarajan 2015B4A70557P
//Vighnesh Hegde 2015B4A70342P
//Biswajit Biswas 2015B5A70418P
//Joy Mukherjee 2015B5A70394P

#include "semanticAnalysis.h"

typedef struct {
    char* data;
    char* bss;
    char* text;
}code;
typedef code* Code;

Code initialiseCode(void);
char* initialiseData(void);
char* initialiseText(void);
char* initialiseBSS(void);
void printSubRoutines(FILE *f);

void addToBlock(char** data,char*append);
void appendDecl(Code cd,char*id);
void appendWriteLiteral(Code cd,char*id);
void appendWrite(Code cd,char*id);
void appendRead(Code cd,char*id);
void addToBSSBlock(Code cd,char*append);
void addToTextBlock(Code cd,char*append);

char* appendBoolCode(Code cd,ASTNode root);
char* appendArithCode(Code cd,ASTNode root);
char* appendArithCodeOp(int tag);


void generateCode(ASTNode root,VariableTable VT,VariableTable GVT, RecordTable RT,Code cd);

void printToFile(FILE* f,Code cd);
