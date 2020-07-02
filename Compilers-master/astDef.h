//Group 7:
//Prashant Rangarajan 2015B4A70557P
//Vighnesh Hegde 2015B4A70342P
//Biswajit Biswas 2015B5A70418P
//Joy Mukherjee 2015B5A70394P

#include "parser.h"

#ifndef LINKLIST
#define LINKLIST

typedef struct node{
	void* e;
	struct node* next;
}node;
typedef struct node* Node;

typedef struct{
	node* head;
	node* tail;
	int length;
}list;
typedef list* List;

list* newList(void);
void ins(list* ls, void *in);
int isEmpty(list* ls);
void printList(list* ls, void (*fptr)(void *));
void * searchList(List l, void* e, int (*compare)(void *,void *));
#endif

#ifndef ASTDEF
#define ASTDEF

#define PROG 0
#define MAIN 1
#define FUNC 2
#define STMTS 3
#define ASGN 4
#define PLUS 5
#define MINUS 6
#define MUL 7
#define DIV 8
#define OPND 9
#define IORD 10
#define IOWR 11
#define FUNCALL 12
#define ITER 13
#define COND 14
#define ELSE 15
#define	OR	16
#define	AND	17
#define	LT	18
#define	LE	19	
#define	GT	20	
#define	GE	21	
#define	NOT	22
#define NE 23
#define EQ 24	
#define ID 25
#define REC 26
#define REC_ID 27
#define NUM 28
#define RNUM 29

struct variableTable{
	//contains primitive as well as records.
	List* varL;//array of list of variables : Node = Var
};
typedef struct variableTable* VariableTable;

struct recordTable{
	//for record definitions.
	List* RecL; // Node = TokOrRec

};
typedef struct recordTable* RecordTable;

typedef struct func{
    //
    int calledOrNot;
    TokenInfo funID;
    List inpar;
    List outpar;
    VariableTable localVars;
}func;
typedef func* Func;


struct funcTable
{
    List* FuncL;//List of Func : Node = Func
};
typedef struct funcTable* FuncTable;


struct astNode;

typedef struct{
    //for record only
    char* name;//name is datatype
    TokenInfo info;
}field;
typedef field* Field;

typedef struct 
{
    TokenInfo t;
    int width;
    List lf; //list of fields
}tokOrRec;

typedef tokOrRec* TokOrRec;

typedef struct{
    //a structure representing all variables. can be a simple var, or a record.
    char* name;
    char* scope;
    int width;
    int offset;
    int globalOrNot;
    TokOrRec info;//this can be tagged based on name. if name starts with # then it is rec
}var;
typedef var* Var;


typedef struct{
    List vl; //(Var) varList from declaration stmt
    List ret; //(TokInfo) idList to be returned. can be NULL
    List rec; //(TokOrRec) list of records
}stmts;
typedef stmts* Stmts;//structure for the construct stmts


typedef struct{
    TokenInfo id;//if it is a.b+c, id will be "b", and id will be c
    char* recName;//this will be "a"
}singleOrRecID;
typedef singleOrRecID* SingleOrRecID;

typedef union{
    Func f;
    Stmts s;
    SingleOrRecID sOrR;
    
} construct;
typedef construct* Construct;

struct astNode{
    int tag;
    Construct self;
    struct astNode* next;
    struct astNode* parent;
    struct astNode* firstChild;
    struct astNode* lastChild;
};
typedef struct astNode* ASTNode;

#endif