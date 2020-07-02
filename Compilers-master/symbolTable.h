//Group 7:
//Prashant Rangarajan 2015B4A70557P
//Vighnesh Hegde 2015B4A70342P
//Biswajit Biswas 2015B5A70418P
//Joy Mukherjee 2015B5A70394P
#include "ast.h"

#ifndef SYMTABLE
#define SYMTABLE
//symbol table

#define No_Of_Variable_Buckets 221
#define No_Of_Func_Buckets 71
#define No_Of_Record_Buckets 97

VariableTable createNewVarTable(void);
void addNewVarToTable(VariableTable vt, RecordTable rt, Var v);
int hashFunction(char* name, int buckets); // can reuse from lexer.h later on
int isVarDefined(VariableTable vt, Var v);

FuncTable createNewFuncTable(void);
void addNewFuncToTable(FuncTable funcT, Func F);
void addLocalFuncVariable(FuncTable funcT, VariableTable globalVars, RecordTable RT, Func f, Var v);
void* isFuncDefined(FuncTable FT, Func f);
TokOrRec isRecDefined(RecordTable RT,TokOrRec rec);
void addLocalVars(FuncTable funcT, TokenInfo funID, Var newVar);

RecordTable createNewRecordTable(void);
void addNewRecordToRecordTable(RecordTable RT, TokOrRec rec);
int isRecordDefined(RecordTable RT, TokOrRec rec);

void printAllVarTables(FuncTable FT);
void printRecordTable(RecordTable RT);
void printVarTable(VariableTable VT);
void printFuncTable(FuncTable FT);
void printLocalVarTable(void *d);
void populateLocal(ASTNode root,VariableTable LVT,FuncTable FT,RecordTable RT);
void populateGlobalAndFunc(ASTNode root,VariableTable GVT,FuncTable FT,RecordTable RT);

void setWidth(TokOrRec tor);
int getWidth(TokOrRec tor);
void setScope(Var v, char* scope);
int getTag(SingleOrRecID s);


int compareFunc(void *d1, void *d2);//Compare two functions by name. Use for type extraction
int compareVar(void *d1, void *d2);//compare two Vars by name, use for type extraction
int compareTokOrRec(void *d1, void *d2);
int compareField(void *d1, void *d2);//Compare two Fields. Use for type checking of records
int compareVarType(void *d1, void *d2);//compare two Vars by type, not name. use for typechecking.

#endif
