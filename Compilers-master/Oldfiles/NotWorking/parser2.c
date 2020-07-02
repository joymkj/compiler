#include "parser.h"

int getRuleID(int NTID, int tokenID, ParseTable pt){
    int** p = pt->pTable;
    return p[NTID-1][tokenID+NO_TERMINALS-1];
}
ParseTable createParseTable(MapTable M, Grammar G){
    int colnum = NO_TERMINALS-1;//one less because 
    int rownum = M->numberOfTerms-NO_TERMINALS;
    ParseTable ptable = (ParseTable)malloc(rownum*sizeof(parseTable));
    ptable->colSize = colnum;
    ptable->rowSize = rownum; //number of nonterminals to be obtained
    
    ptable->pTable = (int**)malloc(rownum*sizeof(int*));
    int** pt = ptable->pTable;
    int i;
    for (i=0; i<rownum; i++){
        pt[i] = (int*)malloc(colnum*sizeof(int));
        memset(pt[i],ERR,colnum*sizeof(int));
        pt[i][TK_SEM+NO_TERMINALS-1] = SYN;
    }
    Term t, ft;
    TermDetails tdhead, td;
    int j,k, b;

    for (i=0; i < G->numberOfRules; i++){
        t = G->rules[i];
        tdhead = findTermDetailsFromID(M, t->termID);
        j = tdhead->id - 1; //j is index of NT in table
        t = t->next;
        while(t!=NULL){
            td = findTermDetailsFromID(M, t->termID);
            ft = td->firstHead;
            while(ft!=NULL){
                k = ft->termID + NO_TERMINALS-1;
                pt[j][k] = i;
                ft  = ft->next;
            }
            if (!td->nullable) break;
            t = t->next;
        }
        b = (t != NULL);
        ft = tdhead->followHead;
        while(ft!=NULL){
            k = ft->termID + NO_TERMINALS-1;
            if (b){
                if (pt[j][k] < 0)
                    pt[j][k] = SYN;
                }
            else
                pt[j][k] = i; //fill with rule number if alpha is nullable, else is SYN
            ft  = ft->next;
        }
    }
    
    return ptable;
}

void printPT(ParseTable pt){
    int i,j;
    int** p = pt->pTable;
    for (i=0; i<=pt->colSize; i++)
        printf("---");
    printf("\n   ");
    for (i=0; i<pt->colSize; i++)
        printf("%2d|", i);
    printf("\n");
    for (i=0; i<=pt->colSize; i++)
        printf("---");
    printf("\n");
    for (i=0; i<pt->rowSize; i++){
        printf("%2d|",i);
        for  (j=0; j<pt->colSize; j++){
            printf("%2d|",p[i][j]);
        }
        printf("\n");
    }
    for (i=0; i<=pt->colSize; i++)
        printf("---");
    printf("\n");
    
}

Term getruleByID(Grammar G,int id){
    return G->rules[id];
}

Stack newStack(){
	Stack new = (Stack)malloc(sizeof(struct stack));
	new->head = NULL;
	new->depth = 0;
	return new;
}

TreeNode pop(Stack s){
	if(s->depth==0){
		printf("can't pop from an empty stack\n");
		return NULL;
	}
	TreeNode e = s->head->e;
	if(s->depth==1){
        s->head = NULL;
        s->depth = 0;
    }
	else{
		s->head = s->head->next;
		s->depth--;
	}
	return e;
}

int dos(Stack s){
    return s->depth;
}

TreeNode top(Stack s){
    if(s->head==NULL) return NULL;
	return s->head->e;
}

void push(Stack s, TreeNode e){//add in the front using head
	StackNode new = malloc(sizeof(stackNode));
	new->e = e;
	new->next = NULL;
	if(s->depth==0){
		s->head = new;
		s->depth++;
	}
	else{
		new->next = s->head;
		s->head = new;
		s->depth++;
	}
}

TreeNode newNode(int id){
    TreeNode new = malloc(sizeof(struct treeNode));
    new->firstChild = NULL;
    new->lastChild = NULL;
    new->next = NULL;
    new->parent = NULL;
    new->termID = id;
    new->tok = NULL;
    return new;
}

int idOfNode(TreeNode t){return t->termID;}

void insertChild(TreeNode parent, TreeNode child){
    child->parent = parent;
    if(parent->firstChild==NULL){
        parent->firstChild = child;
        parent->lastChild = child;
    }
    else{
        parent->lastChild->next = child;
        parent->lastChild = child;
    }
    
}

void removeNode(TreeNode node){
    node->termID = -1;
}

int isInList(int termID, Term list){
    while(list!=NULL){
        if (list->termID == termID){
            return 1;
        }
        list = list->next;
    }
    return 0;
}

TreeNode parseTree(Grammar g, LexBuf b, ParseTable pt,MapTable map){
    //initialise
    Stack evalStack = newStack();
    Stack tempStack = newStack();
    int currentSymbol = g->rules[0]->termID;
    TreeNode root = newNode(currentSymbol);
    TreeNode dol = newNode(DOLLAR);
    push(evalStack,dol);
    push(evalStack,root);
    TreeNode tos=top(evalStack),tempNode;
    TokenInfo tok = getToken(b);
    int rid,done=0;
    Term currentRule;
    while((tos = top(evalStack))!=NULL){
        //printf("tok->token= %s, tos->termID = %s\n",findTermDetailsFromID(map,tok->token)->name, findTermDetailsFromID(map,tos->termID)->name);
        //match
        if(tok->token==tos->termID){
            tos->tok = tok;
            tok = getToken(b);//fetch next token only upon a match
            pop(evalStack);
            continue;
        }
        if(tos->termID<0){
            if (tos->termID == TK_SEM){
                printf("Line %d: Semicolon potentially missing.\n", tok->lineNo);
            }
            else
                printf("Line %d: The token %s for lexeme %s does not match with the expected token %s\n",
                tok->lineNo,findTermDetailsFromID(map,tok->token)->name,
                tok->lexeme,findTermDetailsFromID(map,tos->termID)->name);
            tos->termID = SYNTERROR;
            pop(evalStack);
            continue;
        }

        rid = getRuleID(tos->termID,tok->token,pt);
        //printf("Rule ID = %d\n", rid);
        if(rid==ERR){//error
            
            int pres = isInList(TK_SEM, findTermDetailsFromID(map, tos->termID)->followHead);
            if (pres)  {
                pop(evalStack);
                continue;
            }
            pres = isInList(TK_END, findTermDetailsFromID(map, tos->termID)->followHead);
            if (pres)  {
                pop(evalStack);
                continue;
            }
            if (tok-> token == TK_END){
                pop(evalStack);
                continue;
            }

            printf("Line %d: The token of type %s for lexeme %s does not match with the expected token of type %s\n",
             tok->lineNo, findTermDetailsFromID(map,tok->token)->name, tok->lexeme, findTermDetailsFromID(map,tos->termID)->name);
            
            while(rid!=SYN){
                tok = getToken(b);
                //printf("tok->token= %s, tos->termID = %s\n",findTermDetailsFromID(map,tok->token)->name, findTermDetailsFromID(map,tos->termID)->name);
                if(tok->token==DOLLAR){
                    printf("Premature termination of input!\n");
                    while(tos->termID!=DOLLAR){
                        tos->termID = SYNTERROR;
                        pop(evalStack);
                        tos = top(evalStack);
                    }
                    return root;
                }
                rid = getRuleID(tos->termID,tok->token,pt);
                //printf("Rule ID = %d\n", rid);
            }
            tos->termID = SYNTERROR;
        }
        if(rid==SYN){
            //printf("Line %d: Syntax error! %s, %d\n",tok->lineNo,tok->lexeme,tos->termID);
            if (tos->termID != SYNTERROR){
                printf("Line %d: The token of type %s for lexeme %s does not match with the expected token of type %s\n", tok->lineNo, findTermDetailsFromID(map,tok->token)->name,
                       tok->lexeme,findTermDetailsFromID(map,tos->termID)->name);
                tos->termID = SYNTERROR;
            }
           // else printf("Line %d: Syntax error!\n", tok->lineNo);

            pop(evalStack);
            tos = top(evalStack);
            continue;//start over.
        }
        currentRule = g->rules[rid]; 
        while(currentRule->next!=NULL){
            currentRule = currentRule->next;//take the next term in the rule
            tempNode = newNode(currentRule->termID);//manufacture a new treenode from that term
            push(tempStack,tempNode);
            insertChild(tos,tempNode);
        }
        //now reverse it and add to the stack.
        //before that pop the evalStack
        pop(evalStack);
        while(top(tempStack)!=NULL){
            tempNode = pop(tempStack);
            if(tempNode->termID!=0) push(evalStack,tempNode);
        }
            
    }
    return root;
}

void printTreeNode(TreeNode root, MapTable map,FILE* f){
    if(root->termID==SYNTERROR){
        fprintf(f,"Syntax Error in child of %s\n",
        findTermDetailsFromID(map, root->parent->termID)->name);
    }
    else if(root->parent==NULL){
        fprintf(f,"----\t%s\t  -  \t----\t----\troot\tnon-Leaf\t%s\n",
        findTermDetailsFromID(map, root->termID)->name,
        findTermDetailsFromID(map, root->termID)->name);
    }
    else if(root->termID<0){
        if(root->termID==TK_NUM){
            fprintf(f,"%s\t----\t%d\t%s\t%d\t%s\tleaf\t%s\n",
            root->tok->lexeme,root->tok->lineNo,findTermDetailsFromID(map, root->termID)->name,
            root->tok->value.num,findTermDetailsFromID(map, root->parent->termID)->name,
            findTermDetailsFromID(map, root->termID)->name);
        }
        else if (root->termID==TK_RNUM){
            fprintf(f,"%s\t----\t%d\t%s\t%f\t%s\tleaf\t%s\n",
            root->tok->lexeme,root->tok->lineNo,findTermDetailsFromID(map, root->termID)->name,
            root->tok->value.rnum,findTermDetailsFromID(map, root->parent->termID)->name,
            findTermDetailsFromID(map, root->termID)->name);
        }
        else{
            fprintf(f,"%s\t----\t%d\t%s\t----\t%s\tleaf\t%s\n",
            root->tok->lexeme,root->tok->lineNo,findTermDetailsFromID(map, root->termID)->name,
            findTermDetailsFromID(map, root->parent->termID)->name,
            findTermDetailsFromID(map, root->termID)->name);
        }
    }
    else
    {
        fprintf(f,"----\t%s\t-\t----\t----\t%s\tnon-leaf\t%s\n",
        findTermDetailsFromID(map, root->termID)->name,
        findTermDetailsFromID(map, root->parent->termID)->name,
        findTermDetailsFromID(map, root->termID)->name);
    }
}

void inOrder(TreeNode root,MapTable map,FILE* f){
    if(root->firstChild==NULL) {
        printTreeNode(root,map,f);
        return;
    }
    inOrder(root->firstChild,map,f);
    printTreeNode(root,map,f);
    TreeNode t = root->firstChild->next;
    while(t!=NULL){
        inOrder(t,map,f);
        t=t->next;
    }
}
/*
int main(){
    // FILE* f = fopen("testcase1.txt","r");
    LexBuf b = newBuf("testcase1.txt",512);
    MapTable map = newMapTable();
    RevLookupHashTable HT = newRevLookupHashTable();
    Grammar G = text2list(map,HT,"ActualModifiedGrammar.txt");
    calculateFirst(map,G);
    calculateFollow(map,G);
    ParseTable pt = createParseTable(map, G);
    //printPT(pt);
    TreeNode tree = parseTree(G,b,pt,map);
    //printf("lexeme\tCurrentNode\tlineno\ttokenName\tvalueIfNumber\tparentNodeSymbol\tisLeafNode\tNodeSymbol\n");
    //inOrder(tree,map,stdout);
}*/


