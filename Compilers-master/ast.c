//Group 7:
//Prashant Rangarajan 2015B4A70557P
//Vighnesh Hegde 2015B4A70342P
//Biswajit Biswas 2015B5A70418P
//Joy Mukherjee 2015B5A70394P

#include "ast.h"
#include "symbolTable.h"

list* newList(){
	list* ls = (list*)calloc(1,sizeof(list));
	return ls;
}

void ins(list* ls, void *in){
	node* new = malloc(sizeof(node));
	//new->e = malloc(size);
	//memcpy(new->e,in,size);
	new->e = in;
	//printf("%p-%d ",new->e,*(int*)in);

	if(ls->length==0){
		ls->head = new;
		ls->tail = new;
		ls->length++;
		return;
	}
	// ls->tail->next = new;
	// ls->tail = new;
    new->next = ls->head;
    ls->head = new;
	ls->length++;
	return;
}

void * searchList(List l, void* e, int (*compare)(void *,void *)){
    if(l==NULL)return NULL;
    Node temp = l->head;
    while(temp!=NULL){
        if((*compare)(e,temp->e)) return temp->e;
        temp=temp->next;
    }
    return NULL;
}


int isEmpty(list* ls){
	return !(ls->length);
}

void printList(list* ls, void (*fptr)(void *)){
    if(ls==NULL) return;
	node* current = ls->head;
	while(current!=NULL){
		(*fptr)(current->e);
		//printf("%d ",*(int*)current->e);
		current = current->next;
	}
}

Field newField(char* name, TokenInfo tok){
    Field fd = malloc(sizeof(field));
    fd->info = tok;
    fd->name = malloc(strlen(name));
    strcpy(fd->name,name);
    return fd;
}


ASTNode createASTNode(void){
    ASTNode new = myalloc(sizeof(struct astNode));
    new->self = NULL;
    new->tag = ERR;
    new->next = NULL;
    new->firstChild = NULL;
    new->lastChild = NULL;
    new->parent = NULL;
    return new;
}

ASTNode createAST(TreeNode pTree){
    ASTNode root, children;
    switch (pTree->ruleID) {
        case 0:
            root = createASTNode();
            root->tag = PROG;
            children = createAST(pTree->firstChild);//first child other functions
            ASTNode main = createAST(pTree->lastChild);//last child main functions
            insertChildrenAST(root, children);
            insertChildrenAST(root, main);
            break;
        case 1:
            root = createASTNode();
            root->tag = MAIN;
            root->self = getAddr(pTree);
            children = createAST(pTree->firstChild->next);
            insertChildrenAST(root, children);
            break;
        case 2:
            root = createAST(pTree->firstChild);//for function
            root->next = createAST(pTree->lastChild);//for otherfunctions
            break;
        case 3:
            root = NULL;
            break;
        case 4:
            root = createASTNode();
            root->tag = FUNC;
            root->self = getAddr(pTree); // self or addr?
            pTree = getNext(pTree->firstChild,4);
            children = createAST(pTree);
            insertChildrenAST(root,children);
            break;

        case 16://typedefinitions
            root = createASTNode();
            root->tag = STMTS;
            root->self = getAddr(pTree);
            //needs to return return address, type definitions, and declarations.
            pTree = pTree->firstChild->next->next;
            //pTree is now otherStmts
            children = createAST(pTree);
            insertChildrenAST(root,children);
            break;
        //cases until 28 should be taken care of by getaddr in case 16.
        case 29:
            root = createAST(pTree->firstChild);//for stmt
            root->next = createAST(pTree->lastChild);//for otherStmts
            break;
        case 30:
            root = NULL;
            break;
        case 31://pTree is Stmt -> assignmentStmt
            pTree = pTree->firstChild;
            root = createASTNode();
            root->tag = ASGN;
            pTree= pTree->firstChild;
            root->self = getAddr(pTree);
            //printf("ASGN: %s", root->self->sOrR->recName);
            insertChildrenAST(root, createAST(getNext(pTree, 2)));
            break;
            
        case 32://iterativeStmt
            pTree = pTree->firstChild;
            root = createASTNode();
            root->tag = ITER;
            pTree = pTree->firstChild->next->next;
            insertChildrenAST(root,createAST(pTree));//for Bool
            insertChildrenAST(root,createAST(getNext(pTree,2)));//for stmt
            children = createAST(getNext(pTree,3));//for otherStmts
            insertChildrenAST(root,children);            
            break;
        case 33://conditionalStmt
            pTree = pTree->firstChild;
            root = createASTNode();
            root->tag = COND;
            pTree = pTree->firstChild->next->next;
            insertChildrenAST(root,createAST(pTree));//for Bool
            insertChildrenAST(root,createAST(getNext(pTree,3)));//for stmt
            children = createAST(getNext(pTree,4));//for otherStmts
            insertChildrenAST(root,children);
            pTree = getNext(pTree,5); 
            insertChildrenAST(root,createAST(pTree));//for elsepart
            break;
        case 34://ioStmt
            pTree = pTree->firstChild;
            root = createASTNode();
            if (pTree->ruleID == 48){
                root->tag = IORD;
                root->self = getAddr(getNext(pTree->firstChild, 2));
            }
            else if (pTree->ruleID == 49){
                root->tag = IOWR;
                pTree = getNext(pTree->firstChild, 2);
                root->self = getAddr(pTree);
            }
            break;
        case 35://functioncallStmt
            pTree = pTree->firstChild;
            root = createASTNode();
            root->tag = FUNCALL;
            root->self = getAddr(pTree);
            break;
        case 46://elsePart
            root = createASTNode();
            root->tag = ELSE;
            pTree = pTree->firstChild->next;
            insertChildrenAST(root,createAST(pTree));//for stmt
            children = createAST(getNext(pTree,4));//for otherStmts
            insertChildrenAST(root,createAST(pTree->next));      
            break;    
        case 47://elsePart
            root = createASTNode();
            root->tag = ELSE;//some scam. Doing it for code gen
            break; 
        case 50://all
        case 69://var
            root = createASTNode();
            root->tag = NUM;
            break; 
        case 51://all
        case 70://var
            root = createASTNode();
            root->tag = RNUM;            
            break; 
        case 52://all
            root = createASTNode();
            if(pTree->lastChild->ruleID==38){
                root->tag = ID;
            }
            else root->tag = REC_ID;            
            break; 
        case 53: //Arithmetic expression
        case 56: //term
            root = createAST(pTree->lastChild);
            children = createAST(pTree->firstChild);
            if(root==NULL) root = children;
            else insertAtFrontAST(root,children);
            //insertAtFrontAST(children, root);
            //printf("root %s\n",root->firstChild->self->sOrR->recName);
            break;
        
        case 54:
            root = createASTNode();
            if (pTree->firstChild->ruleID == 63)
                root->tag = PLUS;
            else if (pTree->firstChild->ruleID == 64)
                root->tag = MINUS;
            
            if (pTree->lastChild->ruleID == 55)
                insertChildrenAST(root, createAST(pTree->firstChild->next));
            else if(pTree->lastChild->ruleID == 54){
                children = createAST(pTree->lastChild);
                insertAtFrontAST(children, createAST(pTree->firstChild->next));
                insertChildrenAST(root, children);
            }
            break;
            
        case 55:
        case 58:
            root = NULL;
            break;
        
        case 57: //T'
            root = createASTNode();
            if (pTree->firstChild->ruleID == 61)
                root->tag = MUL;
            else if (pTree->firstChild->ruleID == 62)
                root->tag = DIV;
            
            if (pTree->lastChild->ruleID == 58)
                insertChildrenAST(root, createAST(pTree->firstChild->next));
            else if(pTree->lastChild->ruleID == 57){
                children = createAST(pTree->lastChild);
                insertAtFrontAST(children, createAST(pTree->firstChild->next));
                insertChildrenAST(root, children);
            }
            break;
            
        case 59:
            root = createAST(pTree->firstChild->next);
            break;
            
        case 60://factor
            root = createAST(pTree->firstChild);//passing all
            // root = createASTNode();
            // root->tag = OPND;
            pTree = pTree->firstChild;
            root->self = getAddr(pTree);
            //printf("getAddr returned %s\n",root->self->sOrR->id->lexeme);
            break;
            
        case 65://boolExpression logicalOp
            pTree = pTree->firstChild;
            root = createAST(getNext(pTree,3));//make a node in the name of the logicalOp
            insertChildrenAST(root,createAST(pTree->next));//insert the first boolExp
            insertChildrenAST(root,createAST(getNext(pTree,5)));
            break;
        case 66://boolExpression for relOp
            pTree = pTree->firstChild;
            root = createAST(pTree->next);//make a node from the name of relOp;
            children = createAST(pTree);//for var1;
            //children->tag=OPND;
            children->self = getAddr(pTree);
            insertChildrenAST(root,children);
            children = createAST(getNext(pTree,2));//for var2;
            //children->tag=OPND;
            children->self = getAddr(getNext(pTree,2));
            insertChildrenAST(root,children);
            break;
        case 67:
            root = createASTNode();
            root->tag = NOT;
            pTree = getNext(pTree->firstChild,2);
            insertChildrenAST(root,createAST(pTree));
            break;
        case 68://var
            root = createASTNode();
            root->tag = ID;
            break;
        case 71:
            root = createASTNode();
            root->tag = AND;
            break;
        case 72:
            root = createASTNode();
            root->tag = OR;
            break;
        case 73:
            root = createASTNode();
            root->tag = LT;
            break;
        case 74:
            root = createASTNode();
            root->tag = LE;
            break;
        case 75:
            root = createASTNode();
            root->tag = EQ;
            break;
        case 76:
            root = createASTNode();
            root->tag = GT;
            break;
        case 77:
            root = createASTNode();
            root->tag = GE;
            break;
        case 78:
            root = createASTNode();
            root->tag = NE;
            break;
        default:
            root = NULL;
            break;
    }
    return root;
    
}


void insertChildrenAST(ASTNode parent, ASTNode children){
    while(children != NULL){
        children->parent = parent;
        if(parent->firstChild==NULL){
            parent->firstChild = children;
            parent->lastChild = children;
        }
        else{
            parent->lastChild->next = children;
            parent->lastChild = children;
        }
        children = children ->next;
    }
}

void insertAtFrontAST(ASTNode parent, ASTNode child){
    child->parent = parent;
    if(parent->firstChild==NULL){
        parent->firstChild = child;
        parent->lastChild = child;
    }
    else{
        child->next = parent->firstChild;
        parent->firstChild = child;
    }
}

TreeNode getNext(TreeNode pTree,int n){
    while(n-->0 && pTree->next!=NULL){
        pTree=pTree->next;
    }
    return pTree;
}



int getGlobalStatus(TreeNode pTree){
    if(pTree->ruleID==27)return 1;
    else return 0;
}

List getDeclList(TreeNode pTree){
    List ls;
    if(pTree->ruleID==24){
        ls = getDeclList(pTree->lastChild);
        pTree= pTree->firstChild->firstChild;
        char* name = getNext(pTree,3)->tok->lexeme;
        int globalOrNot = getGlobalStatus(getNext(pTree,4));
        //int width = getWidth();
        pTree=pTree->next;
        if(pTree->ruleID==9){//primitive
            ins(ls,newVar(name,0,globalOrNot,pTree->firstChild->firstChild->tok,NULL));
            //store only tokInfo right now, and type of variable. 
        }
        if(pTree->ruleID==10){//constructed
            ins(ls,newVar(name,0,globalOrNot,pTree->firstChild->lastChild->tok,NULL));
            //getRec(pTree->firstChild->firstChild->tok->lexeme)
            //TODO??: need a getRec function to get the record details
        }
        return ls;
    }
    else if(pTree->ruleID==25){
        return newList();
    }
    else{
        printf("Invalid rule id %d",pTree->ruleID);
        return NULL;
    }
}

List getIdList(TreeNode pTree){//for return
    //assuming the node is returnStmt
    //TODO: check if TokInfo or TokOrRec because idlist can be a list of recordIDs
    List ls;
    switch (pTree->ruleID)
    {
        case 79://reTurnStmt
            if(pTree->firstChild->next->ruleID==80){
                pTree = pTree->firstChild->next->firstChild->next;
                return getIdList(pTree);
            }
            else return NULL;
            break;
        case 82://idList
            ls = getIdList(pTree->lastChild);
            ins(ls,pTree->firstChild->tok);
            return ls;
            break;
        case 83://moreIDs
            return getIdList(pTree->lastChild);
            break;
        case 84://moreIDs
            return newList();
            break;
        default:
            return NULL;
            break;
    }
}


TokOrRec newRec(TokenInfo tok,List lf){
    TokOrRec rec = malloc(sizeof(tokOrRec));
    rec->t = tok;
    rec->lf = lf;
    return rec;
}

TokOrRec getRec(TreeNode pTree){
    //assume pTree is fieldDefinitions or moreFields
    TokOrRec r;
    char* name;
    TokenInfo tok;
    if(pTree->ruleID==20){
        //insert second fieldDefinition first
        r = getRec(pTree->lastChild);
        name = pTree->firstChild->next->firstChild->next->firstChild->tok->lexeme;
        tok = getNext(pTree->firstChild->next->firstChild,3)->tok;
        ins(r->lf,newField(name,tok));
        //now insert first fieldDefinition
        name = pTree->firstChild->firstChild->next->firstChild->tok->lexeme;
        tok = getNext(pTree->firstChild->firstChild,3)->tok;
        ins(r->lf,newField(name,tok));
        return r;
    }
    else if (pTree->ruleID==22){
        r = getRec(pTree->lastChild);
        //insert fieldDefinition
        name = pTree->firstChild->firstChild->next->firstChild->tok->lexeme;
        tok = getNext(pTree->firstChild->firstChild,3)->tok;
        ins(r->lf,newField(name,tok));
        return r;
    }
    else if (pTree->ruleID==23){
        //baprintf("Allocating new record memory\n");
        return newRec(NULL,newList());
    }
    else{
        printf("Invalid rule ID %d\n",pTree->ruleID);
        return NULL;
    }
}

List getRecList(TreeNode pTree){
    //assume pTree is typeDefinitions
    if(pTree->ruleID==17){
        TokOrRec r = getRec(getNext(pTree->firstChild->firstChild,2));
        //newRec(pTree->firstChild->next->tok,NULL);
        r->t = pTree->firstChild->firstChild->next->tok;
        List ls = getRecList(pTree->lastChild);
        ins(ls,r);
        return ls;
    }
    else if(pTree->ruleID==18){
        return newList();
    }
    else{
        printf("Invalid Rule Id %d for RecList\n",pTree->ruleID);
        return NULL;
    }
}

Construct getAddr(TreeNode pTree){
    Construct c = malloc(sizeof(construct));
    TreeNode inp,out;
    List inpar, outpar;
    TokenInfo tok;
    SingleOrRecID sOrR;
    switch (pTree->ruleID) {
        case 1://MAIN
            c->f = newFunc(NULL,NULL,pTree->firstChild->tok);
            break;
        case 4://FUNC
            inp = getNext(pTree->firstChild->next->firstChild,4);
            out = getNext(getNext(pTree->firstChild,2)->firstChild,4);
            c->f = newFunc(getParamList(inp),getParamList(out),pTree->firstChild->tok);
            break;
        case 16://STMTS
            pTree=pTree->firstChild;
            c->s = newStmts(getRecList(pTree),getDeclList(pTree->next),getIdList(getNext(pTree,3)));
            break;
        
        case 36:
            c =  getAddr(pTree -> firstChild);
            break;
        case 37:
        case 52:
            sOrR = malloc(sizeof(singleOrRecID));
            sOrR->recName = pTree->firstChild->tok->lexeme;
            switch (pTree->lastChild->ruleID) {
                case 38:
                    sOrR->recName = NULL;
                    sOrR->id = pTree->firstChild->tok;
                    break;
                case 39:
                    sOrR->id = pTree->lastChild->lastChild->tok;
                    break;
                default:
                    printf("Invalid node accessed with ruleID %d.\n", pTree->lastChild->ruleID);
                    break;
            }
            //printf("Returned sOrR for %d\n",pTree->ruleID);
            c->sOrR = sOrR;
            break;

        case 40://funcall
            tok = getNext(pTree->firstChild,2)->tok;
            outpar = getIdList(pTree->firstChild->firstChild->next);
            inpar = getIdList(getNext(pTree->firstChild,5)->firstChild->next);
            //printList(outpar,printID);
            c->f = newFunc(inpar, outpar, tok);
            break;
            
        case 50:
        case 51:
        case 68:
        case 69:
        case 70:
            sOrR = malloc(sizeof(singleOrRecID));
            sOrR->id = pTree->firstChild->tok;
            sOrR->recName = NULL; //not a record type
            c->sOrR = sOrR;
            // printf("Returned sOrR for %d\n",pTree->ruleID);
            break;
        default:
            break;
    }
    return c;
}

List getParamList(TreeNode pTree){
    //assuming this node is paramlist node
    //i.e., ruleId is 8
    List ls;
    if(pTree->lastChild->ruleID==15){
        ls = newList();
        //a list of Var
    }
    else{
        ls = getParamList(pTree->lastChild->lastChild);
        //calling up the list created in the sub-tree
    }
    pTree=pTree->firstChild;
    if(pTree->ruleID==9){//primitive
        ins(ls,newVar(pTree->next->tok->lexeme,0,0,pTree->firstChild->firstChild->tok,NULL));
        //store only tokInfo right now, and type of variable. 
    }
    if(pTree->ruleID==10){//constructed
        ins(ls,newVar(pTree->next->tok->lexeme,0,0,pTree->firstChild->lastChild->tok,NULL));
        //getRec(pTree->firstChild->firstChild->tok->lexeme)
        //TODO??: need a getRec function to get the record details
    }
    return ls;
}

Var newVar(char* name,int offset,int globalOrNot,TokenInfo info,List lf){
    Var v = malloc(sizeof(var));
    TokOrRec tr = newRec(info,lf);
    v->info = tr;
    v->name = malloc(strlen(name));
    strcpy(v->name,name);
    // v->width = width;
    v->globalOrNot=globalOrNot;
    //v->offset = offset;
    return v;
}



Stmts newStmts(List rec,List vl,List ret){
    Stmts s = malloc(sizeof(stmts));
    s->vl = vl;
    s->ret = ret;
    s->rec = rec;
    return s;
}

void inOrderAST(ASTNode root){
    if(root->firstChild==NULL) {
        // printf("%d\n",root->tag);
        printASTNode(root);
        return;
    }
    inOrderAST(root->firstChild);
    printASTNode(root);

    ASTNode t = root->firstChild->next;
    while(t!=NULL){
        inOrderAST(t);
        t=t->next;
    }
}

void printField(void* n){
    printf("%s x ",((Field)n)->name);
}


void printVar(void *d){
    //TODO: print record in  terms of component types. Need information not available in Var
    Var v = (Var)d;
    printf("%s:\t%s\t%s\t%d\n",v->name,v->info->t->lexeme,v->scope,v->offset);
    //if(v->info->t->lexeme)
}

void printTokOrRec(void *d){
    TokOrRec info = (TokOrRec)d;
    if(info->lf!=NULL){    
        printf("%s\t",info->t->lexeme);
        printList(info->lf,printField);
        printf("\b\b \n");
    }
}

void printFunc(void* n){
    Func f = (Func) n;
    printf("%s:\tinput - \n",f->funID->lexeme);
    printList(f->inpar,printVar);
    printf("\toutput - \n");
    printList(f->outpar,printVar);
    printf("\n");
}

void printFuncSize(FuncTable FT){
    int size;
    int i;
    printf(GRN"Printing function size:\n"RESET);
    printf("Name\tSize\n");
    for(i=0;i<No_Of_Func_Buckets;i++){
        Node n = FT->FuncL[i]->head;
        Func f;
        while (n != NULL){
            f = (Func)n->e;
            size = calculateSize(f->localVars);
            printf("%s\t%d\n",f->funID->lexeme, size);
            n = n->next;
            
        }
    }
}

int calculateSize(VariableTable VT){
    int i, size=0;
    Node n;
    for(i=0;i<No_Of_Variable_Buckets;i++){
        n = VT->varL[i]->head;
        while (n != NULL){
            Var v = ((Var)n->e);
            size += v->width;
            n = n->next;
        }
    }
    return size;
}

void printID(void* d){
    TokenInfo tok = (TokenInfo)d;
    printf("%s ",tok->lexeme);
}

void printASTNode(ASTNode root){
//    if (root->tag==OPND){
//        if (root->self->sOrR->recName !=  NULL)
//            printf("Name: %s\n",root->self->sOrR->recName);
//        if (root->self->sOrR->id != NULL)
//            printf("Token lexeme: %s\n",root->self->sOrR->id->lexeme);
//    }
    if (root->tag == PROG){
        printf(RED"AST printed succesfully using post-order traversal.\n"RESET);
    }
    
    else if(root->tag==FUNCALL){
        printf(CYN"Function being called: %s\n"RESET, root->self->f->funID->lexeme);
        printf("Input parameter call list: ");
        printList(root->self->f->inpar,printID);
        printf("\nOutput parameter call list: ");
        printList(root->self->f->outpar,printID);
        printf("\n");
    }
    else if(root->tag==STMTS){
        printf(CYN"List of statements under function %s: \n"RESET, root->parent->self->f->funID->lexeme);
        if (root->self->s->vl->head != NULL)
        {
            printf(GRN"Variable Declarations\n"RESET);
            printf(YEL"Name\tType\tScope\tOffset\n"RESET);
            printList(root->self->s->vl,printVar);
            printf("\n");
            
        }
        if (((TokOrRec)root->self->s->rec->head) != NULL){
            printf(GRN"Record Declarations\n"RESET);
            printf(YEL"Name\tComponent Types\n"RESET);
            printList(root->self->s->rec,printTokOrRec);
            printf("\n");
        }
        if(root->self->s->ret!=NULL){
            printf(GRN"Return Variables: "RESET);
            printList(root->self->s->ret,printID);
            printf("\n");
        }
    }
    else if(root->tag==FUNC){
        printf(CYN"Function name: %s\n"RESET, root->self->f->funID->lexeme);
        printf("Formal Input parameter list:\n");
        printf(YEL"Name\tType\tScope\tOffset\n"RESET);
        printList(root->self->f->inpar,printVar);
        printf("Formal Output parameter list:\n");
        printf(YEL"Name\tType\tScope\tOffset\n"RESET);
        printList(root->self->f->outpar,printVar);
        printf("\n");
    }
    else if (root->tag == IORD){
        printf(CYN"Reading "RESET"%s\n", root->self->sOrR->id->lexeme);
    }
    else if (root->tag == IOWR){
        printf(CYN"Writing "RESET"%s\n", root->self->sOrR->id->lexeme);
    }
    else if (root->tag == ASGN){
        printf(CYN"Assignment statement\n"RESET"Assigning value to %s.\n",
               root->self->sOrR->id->lexeme);
    }
    else if(root->tag == ITER){
        printf(CYN"While construct\n"RESET);
    }
}

void printAST(ASTNode root){
    ASTNode temp = root->firstChild;
    while (temp != NULL) {
        printAST(temp);
        temp = temp->next;
    }
    printASTNode(root);
}
