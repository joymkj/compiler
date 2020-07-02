//Group 7:
//Prashant Rangarajan 2015B4A70557P
//Vighnesh Hegde 2015B4A70342P
//Biswajit Biswas 2015B5A70418P
//Joy Mukherjee 2015B5A70394P

#include "symbolTable.h"

//#ifndef SEMANALYSIS
//#define SEMANALYSIS

SingleOrRecID arithTypeCheck(ASTNode ari, VariableTable VT, VariableTable GVT, RecordTable RT);
char* boolTypeCheck(ASTNode boo,VariableTable VT,VariableTable GVT);
int compareSOrR(SingleOrRecID s1, SingleOrRecID s2, VariableTable VT, VariableTable GVT, RecordTable RT);
int compareFuncWithRet(Func defn,List ret,VariableTable VT,VariableTable GVT);
int compareFuncWithCall(Func defn,Func call,VariableTable VT,VariableTable GVT);//use for checking call stmt with declaration
char* getTypeOfRec_ID(TokOrRec tor,SingleOrRecID s);
char* getIDType(VariableTable GVT,VariableTable VT,TokenInfo tok);//finds type of the id from the symbol table
TokOrRec getRecFromTable(RecordTable RT,VariableTable GVT,VariableTable VT,char* name);

void getVarList(ASTNode ast, List  l);
void getFuncList(ASTNode ast, List l);
void populateRecordTable(ASTNode root, RecordTable RT);                                    //pass 1
void populateGlobalVariableTableAndFuncTable(ASTNode root,VariableTable globalVT,FuncTable FT);// pass 2
void checkTypeInFn(ASTNode root,VariableTable GVT,VariableTable VT,FuncTable FT,RecordTable RT);       //pass 3
void checkType(ASTNode root,VariableTable VT,FuncTable FT,RecordTable RT);       //pass 4

//#endif

