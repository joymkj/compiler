#include "parser.h"


Term newTerm(int id)
{
    Term t= (Term) malloc (sizeof(struct term));
    t->termID=id;
    t->next=NULL;
    //printf("New Term created. Term Number=%d\n",t->termID);
    return t;
}


Term addTerm(Term currentTerm, Term newT)
{
    //printf("Inserting term %d\n",newT->termID);

    if(currentTerm==NULL)
    {
        currentTerm=newT;
    }

    else
    {
        Term temp=currentTerm;
        while(temp->next!=NULL)
        {
            temp=temp->next;
        }
        temp->next=newT;
    }

    return currentTerm;
}

Grammar newGrammar()
{
    Grammar G = (Grammar) malloc (sizeof(struct grammar));
    G->rules = (Term*)malloc(150*sizeof(Term));
    for(int i = 0; i<150;i++)
    {
        G->rules[i] = NULL;
    }
    //G->numberOfRules = numberOfRules;
    //printf("Grammar created with %d Rules\n",numberOfRules);
    return G;
}


TermDetails newTermDetails(int id,char * name, int nullable)
{
    TermDetails term_d = (TermDetails) malloc(sizeof(struct termDetails));
    term_d->id = id;
    term_d->name = (char*)malloc(strlen(name)*sizeof(char));
    strcpy(term_d->name,name);
    term_d->nullable = nullable;
    term_d->firstHead = NULL;
    term_d->followHead = NULL;
    return term_d;
}

MapTable newMapTable()
{
    MapTable table = (MapTable)malloc(sizeof(struct mapTable));

    table->allTerms = (TermDetails *)malloc(sizeof(TermDetails)*200);
    table->allTerms[53] = newTermDetails(0,"eps",1);
    table->allTerms[52] = newTermDetails(TK_MAIN,"TK_MAIN",0);
    table->allTerms[51] = newTermDetails(TK_CALL,"TK_CALL",0);
    table->allTerms[50] = newTermDetails(TK_ELSE,"TK_ELSE",0);
    table->allTerms[49] = newTermDetails(TK_END,"TK_END",0);
    table->allTerms[48] = newTermDetails(TK_ENDIF,"TK_ENDIF",0);
    table->allTerms[47] = newTermDetails(TK_ENDRECORD,"TK_ENDRECORD",0);
    table->allTerms[46] = newTermDetails(TK_ENDWHILE,"TK_ENDWHILE",0);
    table->allTerms[45] = newTermDetails(TK_GLOBAL,"TK_GLOBAL",0);
    table->allTerms[44] = newTermDetails(TK_IF,"TK_IF",0);
    table->allTerms[43] = newTermDetails(TK_INPUT,"TK_INPUT",0);
    table->allTerms[42] = newTermDetails(TK_INT,"TK_INT",0);
    table->allTerms[41] = newTermDetails(TK_LIST,"TK_LIST",0);
    table->allTerms[40] = newTermDetails(TK_OUTPUT,"TK_OUTPUT",0);
    table->allTerms[39] = newTermDetails(TK_PARAMETER,"TK_PARAMETER",0);
    table->allTerms[38] = newTermDetails(TK_PARAMETERS,"TK_PARAMETERS",0);
    table->allTerms[37] = newTermDetails(TK_READ,"TK_READ",0);
    table->allTerms[36] = newTermDetails(TK_REAL,"TK_REAL",0);
    table->allTerms[35] = newTermDetails(TK_RECORD,"TK_RECORD",0);
    table->allTerms[34] = newTermDetails(TK_RETURN,"TK_RETURN",0);
    table->allTerms[33] = newTermDetails(TK_THEN,"TK_THEN",0);
    table->allTerms[32] = newTermDetails(TK_TYPE,"TK_TYPE",0);
    table->allTerms[31] = newTermDetails(TK_WHILE,"TK_WHILE",0);
    table->allTerms[30] = newTermDetails(TK_WITH,"TK_WITH",0);
    table->allTerms[29] = newTermDetails(TK_WRITE,"TK_WRITE",0);
    table->allTerms[28] = newTermDetails(TK_ASSIGNOP,"TK_ASSIGNOP",0);
    table->allTerms[27] = newTermDetails(TK_FUNID,"TK_FUNID",0);
    table->allTerms[26] = newTermDetails(TK_RECORDID,"TK_RECORDID",0);
    table->allTerms[25] = newTermDetails(TK_COMMA,"TK_COMMA",0);
    table->allTerms[24] = newTermDetails(TK_SEM,"TK_SEM",0);
    table->allTerms[23] = newTermDetails(TK_COLON,"TK_COLON",0);
    table->allTerms[22] = newTermDetails(TK_NE,"TK_NE",0);
    table->allTerms[21] = newTermDetails(TK_DOT,"TK_DOT",0);
    table->allTerms[20] = newTermDetails(TK_EQ,"TK_EQ",0);
    table->allTerms[19] = newTermDetails(TK_OP,"TK_OP",0);
    table->allTerms[18] = newTermDetails(TK_CL,"TK_CL",0);
    table->allTerms[17] = newTermDetails(TK_SQL,"TK_SQL",0);
    table->allTerms[16] = newTermDetails(TK_NUM,"TK_NUM",0);
    table->allTerms[15] = newTermDetails(TK_RNUM,"TK_RNUM",0);
    table->allTerms[14] = newTermDetails(TK_FIELDID,"TK_FIELDID",0);
    table->allTerms[13] = newTermDetails(TK_ID,"TK_ID",0);
    table->allTerms[12] = newTermDetails(TK_SQR,"TK_SQR",0);
    table->allTerms[11] = newTermDetails(TK_OR,"TK_OR",0);
    table->allTerms[10] = newTermDetails(TK_AND,"TK_AND",0);
    table->allTerms[9] = newTermDetails(TK_MUL,"TK_MUL",0);
    table->allTerms[8] = newTermDetails(TK_DIV,"TK_DIV",0);
    table->allTerms[7] = newTermDetails(TK_PLUS,"TK_PLUS",0);
    table->allTerms[6] = newTermDetails(TK_MINUS,"TK_MINUS",0);
    table->allTerms[5] = newTermDetails(TK_LT,"TK_LT",0);
    table->allTerms[4] = newTermDetails(TK_LE,"TK_LE",0);
    table->allTerms[3] = newTermDetails(TK_GT,"TK_GT",0);
    table->allTerms[2] = newTermDetails(TK_GE,"TK_GE",0);
    table->allTerms[1] = newTermDetails(TK_NOT,"TK_NOT",0);
    table->allTerms[0] = newTermDetails(DOLLAR,"DOLLAR",0);

    for(int i=NO_TERMINALS;i<200;i++)
    {
        table->allTerms[i] = NULL;
    }

    table->numberOfTerms = NO_TERMINALS;

    return table;
}



void displayRules(Grammar G)
{
    for(int i = 0 ; i<G->numberOfRules ; i++)
    {
        if(G->rules[i]==NULL)
            printf("Rule No %d.\tEmpty Rule!",i+1);
        else
        {
            printf("Rule No %d.\t",i+1);
            Term temp = G->rules[i];
            while(temp!=NULL)
            {
                printf("%d ", temp->termID);
                temp = temp->next;
            }
        }
        printf("\n");

    }
}
/*
int fileLength(char * filename)
{
    FILE *fp;
    fp=fopen(filename,"r");
    if(fp==NULL)
    {
        printf("File cannot be read");
        return -1;
    }

    int count = 0;
    char c;
    for (c=getc(fp); c!=EOF; c=getc(fp))
        if(c=='\n')
            count++;
    // ++count;
    // printf("LENGTH OF FILE:%d\n",count);
    fclose(fp);
    return count;
}
*/
/*
int isDuplicate(MapTable map,char* termName)
{
    for(int i =0 ; i<map->numberOfTerms;i++)
    {
        if(strcmp(map->allTerms[i]->name,termName)==0)
            return 1;
    }
    return 0;
}
*/

MapTable addToMapTable(MapTable map, TermDetails newTd)
{
    //assuming mapTable already created!
    map->allTerms[map->numberOfTerms] = newTd;
    map->numberOfTerms++;
    return map;
}

int getTermID(MapTable map, char* termName)
{
    for(int i =0 ; i<map->numberOfTerms;i++)
    {
        if(strcmp(map->allTerms[i]->name,termName)==0)
            return i+map->allTerms[0]->id;
    }

    return -1;
}

void displayTermDetails(TermDetails td)
{
    printf("Term id %d\n",td->id);
    printf("Term name %s\n",td->name);
    if(td->nullable)printf("It is nullable.\n");
}

void displayAllTermDetails(MapTable map)
{   
    printf("\n\n\tTerm Name \t\t|\tTermID\n");
    printf("________________________________|____________________\n");
    for(int i =0;i<map->numberOfTerms;i++)
        printf("%s\r\t\t\t\t|   Term Id : %2d\n",map->allTerms[i]->name,map->allTerms[i]->id);
}



TermDetails findTermDetailsFromID(MapTable map, int term_id)
{
    return map->allTerms[term_id - map->allTerms[0]->id];
}


// New MapTable must be created beforehand with newMapTable()
Grammar text2list(MapTable map, RevLookupHashTable HT, char * filename)
{
    //int count=fileLength(filename);
    Grammar G = newGrammar();
    FILE *fp;
    char line[100];
    char *token;
    fp=fopen(filename,"r");
    if(fp==NULL)
    {
        printf("File cannot be read");
        return NULL;
    }

    int ruleNumber=0;
    while(fgets(line,100,fp)!=NULL)
    {   
        line[strcspn(line, "\r\n")] = 0;
        token=strtok(line," ");
        while(token!=NULL)
        {
            //printf("%s\n",token);

            //printf("%s\n",map->allTerms[map->numberOfTerms-1]->name);

            //printf("\n\n comp result : %d\n\n",strcmp(map->allTerms[map->numberOfTerms-1]->name,token));

            int termID = isDuplicate(HT,token);

            if(termID != -NO_TERMINALS)
            {
                //printf("\n\nduplicate Term\n\n");
                G->rules[ruleNumber]=addTerm(G->rules[ruleNumber],newTerm(termID));         //replace with proper ID
            }
            else
            {
                int newID = map->numberOfTerms+map->allTerms[0]->id;

                TermDetails newTd = newTermDetails(newID,token,0); //nullable?
                map = addToMapTable(map,newTd);

                addNewRevLookupEntry(HT,newID,token);

                G->rules[ruleNumber]=addTerm(G->rules[ruleNumber],newTerm(newID));         //replace with proper ID

            }
            token = strtok(NULL, " ");
        }
        ruleNumber++;
        G->numberOfRules++;
    }
    //printf("\n\n\n*************** DISPLAY RULES ***************\n\n\n");
    //displayRules(G);
    //printf("\n\n************** DISPLAY ALL TERM DETAILS **************\n");
    //displayAllTermDetails(map);
    fclose(fp);
    return G;
}

void displayFirst(MapTable map,int termID){
        TermDetails currentTd = findTermDetailsFromID(map,termID);
        printf("First of %s\t: ",currentTd->name);
        Term rec=currentTd->firstHead;
        if(rec==NULL){
            printf("NULL");
        }
        while(rec!=NULL){
            printf("%s ",findTermDetailsFromID(map,rec->termID)->name);
            rec=rec->next;
        }
        if(isNullable(map,termID)) printf("eps ");
        printf("\n");
}
void displayFollow(MapTable map,int termID){
        TermDetails currentTd = findTermDetailsFromID(map,termID);
        printf("Follow of %s\t: ",currentTd->name);
        Term rec=currentTd->followHead;
        if(rec==NULL){
            printf("NULL");
        }
        while(rec!=NULL){
            printf("%s ",findTermDetailsFromID(map,rec->termID)->name);
            rec=rec->next;
        }
        printf("\n");
}

Term findTerminFirst(MapTable map, int termID, int findID){
    Term rec=findTermDetailsFromID(map,termID)->firstHead;
    while(rec!=NULL){
        if(rec->termID==findID){
            return rec;
        }
        rec=rec->next;
    }
    return rec;
}

Term findTerminRule(Term rule, int findID){
        if(rule==NULL){
            return rule;
        }
        else
        {
            Term temp = rule;
            while(temp!=NULL)
            {
                if(temp->termID==findID){
                    return temp;
                }
                temp = temp->next;
            }
        }
        return NULL;
}



// Term findTermFollowingEps(MapTable map, Term rule){
//         int flag=0;
//         if(rule==NULL||rule->next==NULL||rule->next->next==NULL){
//             return NULL;
//         }
//         else
//         {
//             Term temp = rule->next;
//             while(temp!=NULL)
//             {
//                 if((findTerminFirst(map,temp->termID,0))!=NULL){
//                     flag=1;
//                     if(flag)
//                         temp = temp->next;
//                 }
//                 else{
//                     if (!flag){
//                         return NULL;
//                     }
//                     else
//                         return temp;
//                 }
//             }
//         }
// }

int isNullable(MapTable map, int termID){
    return findTermDetailsFromID(map,termID)->nullable;
}

void setNullable(MapTable map,int termID){
    findTermDetailsFromID(map,termID)->nullable=1;
}
void setNotNullable(MapTable map,int termID){
    findTermDetailsFromID(map,termID)->nullable=0;
}
int pushFirst(MapTable map, int termID, Term t){//returns success/failure
        if(t==NULL) return 0;
        TermDetails rec=findTermDetailsFromID(map,termID);
        int count = 0;        
        while(t!=NULL){
            if(rec->firstHead==NULL||rec->firstHead->termID > t->termID){
                Term temp = newTerm(t->termID);
                temp->next = rec->firstHead;
                rec->firstHead=temp;
                //printf("Added %d to head of First of %d\n",t->termID,termID);
                t=t->next;
                count++;
                continue;
            }
            Term curr = rec->firstHead;
            Term curr2 = curr;
            while(curr!=NULL && t!=NULL){
                if(curr->termID==t->termID) {
                    break;
                }
                //this means curr->termID < t->termID
                curr2=curr;
                curr=curr->next;
                if(curr==NULL||curr->termID > t->termID){
                    Term temp = newTerm(t->termID);
                    temp->next = curr;
                    curr2->next = temp;

                    count++;
                    break;
                }       
            }
            if(t!=NULL)t = t->next;
        }
        //printf("Count %d\n",count);
        return count;
}
void calculateFirst(MapTable map, Grammar G){
    int i=0;
    for(i =-52;i<0;i++){
            pushFirst(map,i,newTerm(i));
    }
    for(i=0;i<G->numberOfRules;i++){
        if(findTerminRule(G->rules[i],0)!=NULL){
            setNullable(map,G->rules[i]->termID);
        }
    }
    int change=1;
    TermDetails toAdd;
    int check;
    for(check=0;change>0;check++){
        change=0;
        for(i=0;i<G->numberOfRules;i++){
            //Term current = findTermDetailsFromID(map,G->rules[i]->termID)->firstHead;
            Term currRuleTerm = G->rules[i]->next;
            //if(current==NULL) {
            toAdd = findTermDetailsFromID(map,currRuleTerm->termID);
            change+=pushFirst(map,G->rules[i]->termID,toAdd->firstHead);
            while(toAdd->nullable){
                //setNullable(map,cur)
                currRuleTerm = currRuleTerm->next;
                if(currRuleTerm==NULL){
                    setNullable(map,G->rules[i]->termID);
                    //change-=10;
                    break;
                }
                toAdd = findTermDetailsFromID(map,currRuleTerm->termID);
                change+=pushFirst(map,G->rules[i]->termID,toAdd->firstHead);
            }
            //if(isNullable(map,G->rules[i]->next->termID))
            //continue;
            //}
            //printf("Rule No. %d\n",i);
            //change+=pushFirst(map,G->rules[i]->termID,current);//pushes everything in the list current in order
            //if there is any change to the first of term i, then change is positive and the loop runs again
        }
        //printf("%d\n",change);
    }
}
int pushFollow(MapTable map, int termID, Term t){
        if(t==NULL) return 0;
        TermDetails rec=findTermDetailsFromID(map,termID);
        int count = 0;        
        while(t!=NULL){
            if(rec->followHead==NULL||rec->followHead->termID > t->termID){
                Term temp = newTerm(t->termID);
                temp->next = rec->followHead;
                rec->followHead=temp;
                t=t->next;
                count++;
                continue;
            }
            Term curr = rec->followHead;
            Term curr2 = curr;
            while(curr!=NULL && t!=NULL){
                if(curr->termID==t->termID) {
                    break;
                }
                //this means curr->termID < t->termID
                curr2=curr;
                curr=curr->next;
                if(curr==NULL||curr->termID > t->termID){
                    Term temp = newTerm(t->termID);
                    temp->next = curr;
                    curr2->next = temp;
                    curr2 = temp;
                    count++;
                    break;
                }       
            }
            if(t!=NULL)t = t->next;
        }
        return count;
}
void calculateFollow(MapTable map, Grammar G){
    pushFollow(map,G->rules[0]->termID,newTerm(DOLLAR));
    int id,rul,count;
    Term nextInList;
    int change=1;
    while(change>0){
        change=0;
        for(id=1; id <= map->numberOfTerms-NO_TERMINALS;id++){
            for(rul=0;rul<G->numberOfRules;++rul){
                nextInList = searchRule(map,id,G->rules[rul]);
                if (nextInList == NULL) continue;
                nextInList=nextInList->next;
                while(nextInList!=NULL){
                    change+=pushFollow(map,id,findTermDetailsFromID(map,nextInList->termID)->firstHead);
                    if(isNullable(map,nextInList->termID)){
                        nextInList=nextInList->next;
                    }
                    else break;
                }
                if(nextInList==NULL){
                    change+=pushFollow(map,id,findTermDetailsFromID(map,G->rules[rul]->termID)->followHead);
                }
            }
        }
    }
}

Term searchRule(MapTable map,int id,Term rule){
    rule = rule->next;
    while(rule!=NULL){
        if(id==rule->termID){
            return rule;
        }
        rule = rule->next;
    }
    return NULL;
    
}
