//Group 7:
//Prashant Rangarajan 2015B4A70557P
//Vighnesh Hegde 2015B4A70342P
//Biswajit Biswas 2015B5A70418P
//Joy Mukherjee 2015B5A70394P

#ifndef AST
#define AST

#include "astDef.h"

int errAST ;
ASTNode createASTNode(void);
ASTNode createAST(TreeNode pTree);
Construct getAddr(TreeNode pTree);
void insertChildrenAST(ASTNode parent, ASTNode children);
void insertAtFrontAST(ASTNode parent, ASTNode child);
TreeNode getNext(TreeNode pTree,int n);

Field newField(char* name, TokenInfo tok);
Stmts newStmts(List vl,List ret,List rec);
TokOrRec newRec(TokenInfo tok,List lf);
Func newFunc(List inpar, List outpar,TokenInfo tok);
Var newVar(char* name,int offset,int globalOrNot,TokenInfo info,List lf);

void printVar(void *d);
void printID(void* d);
void printTokOrRec(void *d);
void printFunc(void* n);
void printField(void* n);
void printASTNode(ASTNode root);
void printAST(ASTNode root);
void printFuncSize(FuncTable FT);
void inOrderAST(ASTNode root);
int calculateSize(VariableTable VT);

int getGlobalStatus(TreeNode pTree);
List getParamList(TreeNode pTree);
List getDeclList(TreeNode pTree);
List getIdList(TreeNode pTree);
List getRecList(TreeNode pTree);
TokOrRec getRec(TreeNode pTree);

#endif
