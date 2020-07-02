#include"../lexer.h"
#define NO_TERMINALS 54
#define NO_REVLOOKUP_HASHBUCKETS 157

typedef struct term *Term;
typedef struct termDetails *TermDetails;
typedef struct mapTable *MapTable;
typedef struct grammar *Grammar;
typedef struct revLookupEntry *RevLookupEntry;
typedef struct revLookupHashTable *RevLookupHashTable;

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


//termDetails contains other necessary details about a term 

//Number of terms??!
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

struct revLookupEntry{
    int id;
    char* name;
    RevLookupEntry next;
};

struct revLookupHashTable{
    RevLookupEntry* table;
    int numOfBuckets;
};

Term newTerm(int k);

Term addTerm(Term currentList, Term newT);
Grammar newGrammar();

MapTable newMapTable();
MapTable addToMapTable(MapTable map, TermDetails newTd);

void displayRules(Grammar G);
int fileLength(char * filename);
Grammar text2list(MapTable map, RevLookupHashTable HT, char * filename);

TermDetails newTermDetails(int id,char * name, int nullable);
void displayTermDetails(TermDetails td);
//int isDuplicate(MapTable map, char* termName); 

int hashFunc(char* name);
RevLookupEntry newRevLookupEntry(int id,char *name);
RevLookupHashTable newRevLookupHashTable();
void addNewRevLookupEntry(RevLookupHashTable HT, int id, char* name);
int isDuplicate(RevLookupHashTable HT, char *name);//return termID if found else return -NO_TERMINALS

TermDetails findTermDetailsFromID(MapTable map, int term_id);     //finds TermDetails by Term ID in array of TermDetails look up time order1 
//int getTermID(MapTable map, char* termName); 
TermDetails findTermDetailsFromName(MapTable map, char* termName); // use getTermID and findTermDetailsFromID

void calculateFirst(MapTable map, Grammar G);
void calculateFollow(MapTable map, Grammar G);

void displayAllTermDetails(MapTable map);
void displayFirst(MapTable map,int termID);
void displayFollow(MapTable map,int termID);

Term searchRule(MapTable map,int id,Term rule);
int isNullable(MapTable map, int termID);

void displayHTData(RevLookupHashTable HT);



