#include "../semanticAnalysis.h"

typedef struct {
    char* data;
    char* bss;
    char* text;
}code;
typedef code* Code;

Code initialiseCode();
char* initialiseData();
char* initialiseText();
char* initialiseBSS();
void printSubRoutines(FILE *f);

void addToBlock(char* data,char*append);