#include"lexer.h"
typedef struct term *Term;
typedef struct grammar *Grammar;
typedef struct termDetails *TermDetails;
typedef struct mapTable *MapTable;

struct term
{
    int termID;
    Term next;
};

struct grammar                  //Rule is a list of termIDs and grammar has an array of rules
{
    Term* rules;
    int numberOfRules;
};


struct termDetails              //termDetails contains other necessary details about a term
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
Grammar newGrammar(int numberOfRules);
MapTable newMapTable();
MapTable addToMapTable(MapTable map, TermDetails newTd);
void displayRules(Grammar G);
int fileLength(char * filename);
Grammar text2list(MapTable map, char * filename);
TermDetails newTermDetails(int id,char * name, int nullable);
void displayTermDetails(TermDetails td);
int isDuplicate(MapTable map, char* termName);
TermDetails findTermDetailsFromID(MapTable map, int term_id);     //finds TermDetails by Term ID in array of TermDetails look up time order1
//int getTermID(MapTable map, char* termName);
TermDetails findTermDetailsFromName(MapTable map, char* termName); // use getTermID and findTermDetailsFromID
void displayAllTermDetails(MapTable map);
Term pushFirst(MapTable map, int termID, Term t);       //pushes Term t in firstlist of Term in map with given termID
Term pushFollow(MapTable map, int termID, Term t);
void displayFirst(MapTable map,int termID);
Term findTerminFirst(MapTable map, int termID, int findID);     //finds Term with findID in first list of Term in map with given termID
Term findTerminRule(Term rule,int findID);
Term findTermFollowingEps(MapTable map, Term rule);                               //returns term in a production whose preceding terms all have epsilon in first
Term calculateFirst(MapTable map, Grammar G);
Term findTerminLHS(MapTable map, Grammar G, int termID);
Term calculateFirst2(MapTable map, Grammar G);
Term calculateFollow(MapTable map, Grammar G);
