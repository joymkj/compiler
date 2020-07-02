//Group 7:
//Prashant Rangarajan 2015B4A70557P
//Vighnesh Hegde 2015B4A70342P
//Biswajit Biswas 2015B5A70418P
//Joy Mukherjee 2015B5A70394P

#include"lexer.h"

#ifndef MYALLOC
#define MYALLOC

#include<stdlib.h>
#include<stdio.h>
size_t total_mem;
void printMem(void);
void* myalloc(size_t size);
void myfree(void * ptr);
size_t getMem(void);
void newMEM(void);

#endif

#define ERR -1
#define SYN -2
#define SYNTERROR -100

#define GRAMMARFILE "grammar.txt"

#define NO_TERMINALS 54
#define NO_REVLOOKUP_HASHBUCKETS 157

typedef struct term *Term;
typedef struct termDetails *TermDetails;
typedef struct mapTable *MapTable;
typedef struct grammar *Grammar;


//each term is uniquely identified by termID
struct term
{
    int termID;
    Term next;
};

//Rule is a list of termIDs and grammar has an array of rules
struct grammar
{
    Term* rules;
    int numberOfRules;
};

struct termDetails
{
    int id;
    char* name;
    int nullable;
    Term firstHead;
    Term followHead;

};

struct mapTable
{
	TermDetails* allTerms;
	int numberOfTerms;
};

Term newTerm(int k);

Term addTerm(Term currentList, Term newT);
Grammar newGrammar(void);

MapTable newMapTable(void);
MapTable addToMapTable(MapTable map, TermDetails newTd);

void displayRules(Grammar G);
int fileLength(char * filename);
Grammar text2list(MapTable map, RevLookupHashTable HT, char * filename);

TermDetails newTermDetails(int id,char * name, int nullable);
void displayTermDetails(TermDetails td);

RevLookupHashTable newRevLookupHashTable(void);

TermDetails findTermDetailsFromID(MapTable map, int term_id);     //finds TermDetails by Term ID in array of TermDetails look up time order1 
TermDetails findTermDetailsFromName(MapTable map, char* termName); // use getTermID and findTermDetailsFromID

void calculateFirst(MapTable map, Grammar G);
void calculateFollow(MapTable map, Grammar G);

void displayAllTermDetails(MapTable map);
void displayFirst(MapTable map,int termID);
void displayFollow(MapTable map,int termID);

Term searchRule(MapTable map,int id,Term rule);
int isNullable(MapTable map, int termID);
int isInList(int termID, Term list);

void displayHTData(RevLookupHashTable HT);

typedef struct{
    int colSize;
    int rowSize;
    int** pTable;
}parseTable;
typedef parseTable* ParseTable;

ParseTable createParseTable(MapTable M, Grammar G);
int getRuleID(int NTID, int tokenID, ParseTable pt);
void printPT(ParseTable pt);

int errFlag;

typedef struct treeNode *TreeNode;

struct treeNode{
    int termID;
    int ruleID;
    TreeNode next;
    TreeNode parent;
    TreeNode firstChild;
    TreeNode lastChild;
    TokenInfo tok;
};

TreeNode newNode(int id);
void insertChild(TreeNode parent, TreeNode child);
void insertSibling(TreeNode elder, TreeNode younger);
void removeNode(TreeNode node);//removing can be setting id to -1

Term getruleByID(Grammar G,int id);

typedef struct stacknode{//maintain stack of treenodes
	TreeNode e;
	struct stacknode* next;
}stackNode;

typedef stackNode *StackNode;

struct stack{
	StackNode head;
	int depth;
};

typedef struct stack* Stack;

Stack newStack(void);

void push(Stack s, TreeNode e);
TreeNode top(Stack s);
TreeNode pop(Stack s);

int parseTree(TreeNode root, Grammar g, LexBuf b, ParseTable pt,MapTable map);

void printTreeNode(TreeNode root, MapTable map,FILE* f);
void inOrder(TreeNode root,MapTable map,FILE* f);
void printInOrder(TreeNode root,MapTable map,FILE* f);
void clearMem();
