//Group 7:
//Prashant Rangarajan 2015B4A70557P
//Vighnesh Hegde 2015B4A70342P
//Biswajit Biswas 2015B5A70418P
//Joy Mukherjee 2015B5A70394P

#include "symbolTable.h"

int hashFunction(char* name, int buckets){
    unsigned int hash = 0;
    int c=0;
    while ((c = *name++)){
        hash += c*31;
    }
    return hash%buckets ;
}


// variable table 
VariableTable createNewVarTable(){
	VariableTable vt = (VariableTable)malloc(sizeof(struct variableTable));
	vt->varL = (List*)malloc(sizeof(List)*No_Of_Variable_Buckets);
	for(int i = 0; i<No_Of_Variable_Buckets ; i++){
		vt->varL[i] = newList();
	}
	return vt;
}



void addNewVarToTable(VariableTable vt, RecordTable rt, Var v){

	List recL = rt->RecL[hashFunction(v->info->t->lexeme,No_Of_Record_Buckets)];
	TokOrRec tor = searchList(recL, v->info, compareTokOrRec);
	if(tor==NULL){
		printf(YEL"Line %d: "RESET"Datatype %s does not exist.\n",
		v->info->t->lineNo,v->info->t->lexeme);
		errAST = 1;
		return;		
	}

	List ls = vt->varL[hashFunction(v->name, No_Of_Variable_Buckets)];
	if(searchList(ls,v,compareVar)==NULL){//means it does not exist
		v->width = getWidth(tor);
		ins(ls, v);
	}
	else{
		printf(YEL"Line %d: "RESET"Variable %s is a global variable, cannot declare more than once.\n",
		v->info->t->lineNo,v->name);
		errAST = 1;
	}
}


// function table 

FuncTable createNewFuncTable(){

	FuncTable funcT = (FuncTable)malloc(sizeof(struct funcTable));
	funcT->FuncL = (List*)malloc(sizeof(List)*No_Of_Func_Buckets);
	for(int i = 0; i<No_Of_Func_Buckets ; i++){
		funcT->FuncL[i] = newList();
	}

	return funcT;
}

void addNewFuncToTable(FuncTable funcT,Func f){
	
	// does not check if the definition already exists! 
	List flist = funcT->FuncL[hashFunction(f->funID->lexeme ,No_Of_Func_Buckets)];
	if(searchList(flist,f,compareFunc)==NULL){
		ins(flist,f);
	}
	else{
		printf(YEL"Line %d: "RESET"Function %s already exists.\n",
		f->funID->lineNo,f->funID->lexeme);
		errAST = 1;
	}

}
void* isFuncDefined(FuncTable FT, Func f){
	List flist = FT->FuncL[hashFunction(f->funID->lexeme ,No_Of_Func_Buckets)];
	return searchList(flist,f,compareFunc);
}
void addLocalFuncVariable(FuncTable funcT, VariableTable globalVars, RecordTable RT, Func f, Var v){
	
	List recL = RT->RecL[hashFunction(v->info->t->lexeme,No_Of_Record_Buckets)];
	TokOrRec tor = searchList(recL, v->info, compareTokOrRec);
	if(tor==NULL){
		printf(YEL"Line %d: "RESET"Datatype %s does not exist.\n",
		v->info->t->lineNo,v->info->t->lexeme);
		errAST = 1;
		return;		
	}
	int index = hashFunction(f->funID->lexeme ,No_Of_Func_Buckets);
	Func funcInTable = searchList(funcT->FuncL[index],f,compareFunc);
	if(funcInTable==NULL) printf("func %s not found\n",f->funID->lexeme);
	
	if(f!=NULL){
		int varIndex = hashFunction(v->name,No_Of_Variable_Buckets);
		List globalV = globalVars->varL[varIndex];
		List localV = funcInTable->localVars->varL[varIndex];
		if(searchList(globalV,v,compareVar)==NULL){
			if(searchList(localV,v,compareVar)==NULL){
				//insert if not found in global and local variable table
				//set width first
				v->width = getWidth(tor);
				ins(localV,v);
			}
			else{
				printf(YEL"Line %d: "RESET"Variable %s already exists within the same function\n", v->info->t->lineNo,v->name);
				errAST = 1;
			}
		}
		else{
			printf(YEL"Line %d: "RESET"Variable %s is a global variable and cannot be declared again.\n", v->info->t->lineNo,v->name);
			errAST = 1;
		}
	}

	else{

		// should never reach the else part 

		printf("something is wrong\n\n");
		exit(-1);
	}

}

int setCalledOrNot(FuncTable funcT, Func f){
	int index = hashFunction(f->funID->lexeme ,No_Of_Func_Buckets);
	Node ele = funcT->FuncL[index]->head;
	while(ele!=NULL){
		if(strcmp(((Func)ele->e)->funID->lexeme,f->funID->lexeme)==0){
			if(((Func)ele->e)->calledOrNot==1){
				printf(YEL"Line %d: "RESET"Function %s already on the call stack. Recursion is not allowed in this Language.",
				f->funID->lineNo,f->funID->lexeme);
				((Func)ele->e)->calledOrNot++;
				errAST = 1;
                return 1;
			}
			((Func)ele->e)->calledOrNot++;
            return 0;
		}
		ele = ele->next;
	}
	//what if func definition not found?
    return ERR;
}

void resetCalledOrNot(FuncTable funcT, TokenInfo funID){
	int index = hashFunction(funID->lexeme ,No_Of_Func_Buckets);
	Node ele = funcT->FuncL[index]->head;
	while(ele!=NULL){
		if(strcmp(((Func)ele->e)->funID->lexeme,funID->lexeme)==0){
			((Func)ele->e)->calledOrNot-- ;
			break;
		}
		ele = ele->next;
	}
}


RecordTable createNewRecordTable(void){
	RecordTable RT = (RecordTable)malloc(sizeof(struct recordTable));
	RT->RecL = (List*)malloc(sizeof(List)*No_Of_Record_Buckets);
	for(int i=0 ;i<No_Of_Record_Buckets;i++){
		RT->RecL[i]= newList();
	}
	TokOrRec type = malloc(sizeof(tokOrRec));
	type->t = makeTok("int",0,0);
	setWidth(type);
	addNewRecordToRecordTable(RT,type);
	type = malloc(sizeof(tokOrRec));
	type->t = makeTok("real",0,0);
	setWidth(type);
	addNewRecordToRecordTable(RT,type);
	return RT;
	
}


void addNewRecordToRecordTable(RecordTable RT, TokOrRec rec){
	List recL = RT->RecL[hashFunction(rec->t->lexeme,No_Of_Record_Buckets)];
	if(searchList(recL, rec, compareTokOrRec)==NULL){
		ins(recL,rec);
	}
	else{
		printf(YEL"Line %d: "RESET"Record %s already exists\n", rec->t->lineNo,rec->t->lexeme);
		errAST = 1;
	}
}
TokOrRec isRecDefined(RecordTable RT,TokOrRec rec){
	List recL = RT->RecL[hashFunction(rec->t->lexeme,No_Of_Record_Buckets)];
	return (TokOrRec)searchList(recL, rec, compareTokOrRec);
}
void populateRecordTable(ASTNode root,RecordTable RT){
	ASTNode t = root->firstChild;
    while(t!=NULL){
        populateRecordTable(t,RT);
        t=t->next;
    }
    if(root->tag == STMTS){
		Node toAdd = root->self->s->rec->head;
		while(toAdd!=NULL){
			setWidth((TokOrRec)toAdd->e);
			addNewRecordToRecordTable(RT,(TokOrRec)toAdd->e);
			toAdd = toAdd->next;
		}
	}
}

void populateGlobalAndFunc(ASTNode root,VariableTable GVT,FuncTable FT,RecordTable RT){
	ASTNode t = root->firstChild;
    while(t!=NULL){
        populateGlobalAndFunc(t,GVT,FT,RT);
        t=t->next;
    }
    if(root->tag == STMTS){
		Node toAdd = root->self->s->vl->head;
		static int offset=0;
		while(toAdd!=NULL){
			//add only global variables
			if(((Var)toAdd->e)->globalOrNot == 1){
				// ((Var)toAdd->e)->width = getWidth(tor);
				addNewVarToTable(GVT,RT,(Var)toAdd->e);
				((Var)toAdd->e)->offset = offset;
				offset+=((Var)toAdd->e)->width;
				setScope((Var)toAdd->e,"global");
			}
			toAdd = toAdd->next;
		}
	}
	if(root->tag == FUNC||root->tag==MAIN){
		//printf("added %s\n",root->self->f->funID->lexeme);
		addNewFuncToTable(FT,root->self->f);
		//TODO: check if fn defined before added
	}
	if(root->tag==FUNCALL){
		Func f = isFuncDefined(FT,root->self->f); 
		if(f!=NULL){
			// compareFuncWithCall(f,root->self->f,GVT,VT);
		}
		else{
			printf(YEL"Line %d: "RESET"Function %s not defined in this scope.\n",
			root->self->f->funID->lineNo,root->self->f->funID->lexeme);
			errAST = 1;
		}
	}
}

void setScope(Var v, char* scope){
	v->scope = malloc(strlen(scope));
	strcpy(v->scope,scope);
}

void populateLocal(ASTNode root,VariableTable GVT,FuncTable FT,RecordTable RT){
	ASTNode t = root->firstChild;
    while(t!=NULL){
        populateLocal(t,GVT,FT,RT);
        t=t->next;
    }
    if(root->tag == FUNC||root->tag == MAIN){
		Func f = root->self->f;
		int  localOffset=0;
		Node toAdd;
		if(root->tag == FUNC){//main does not have parameters
			toAdd = root->self->f->inpar->head;
			//adding inpars
			while(toAdd!=NULL){
				addLocalFuncVariable(FT,GVT,RT,f,(Var)toAdd->e);
				((Var)toAdd->e)->offset = localOffset;
				localOffset+=((Var)toAdd->e)->width;
				setScope((Var)toAdd->e,f->funID->lexeme);
				toAdd = toAdd->next;
			}
			toAdd = root->self->f->outpar->head;
			//adding outpars
			while(toAdd!=NULL){
				addLocalFuncVariable(FT,GVT,RT,f,(Var)toAdd->e);
				((Var)toAdd->e)->offset = localOffset;
				localOffset+=((Var)toAdd->e)->width;
				setScope((Var)toAdd->e,f->funID->lexeme);
				toAdd = toAdd->next;
			}
		}
		toAdd = root->firstChild->self->s->vl->head;
		//stmts which is the child of this node
		while(toAdd!=NULL){
			//add only local variables
			if(((Var)toAdd->e)->globalOrNot == 0){
				addLocalFuncVariable(FT,GVT,RT,f,(Var)toAdd->e);
				((Var)toAdd->e)->offset = localOffset;
				localOffset+=((Var)toAdd->e)->width;
				setScope((Var)toAdd->e,f->funID->lexeme);
			}
			toAdd = toAdd->next;
		}

	}
}

void setWidth(TokOrRec tor){
    if(strcmp(tor->t->lexeme,"int")==0) tor->width=2;
    else if(strcmp(tor->t->lexeme,"real")==0) tor->width=4;
    else{
        int acc = 0;
        Node temp = tor->lf->head;
        while(temp!=NULL){
            if(strcmp(((Field)temp->e)->name,"int")==0) acc+= 2;
            else if(strcmp(((Field)temp->e)->name,"real")==0) acc+= 4;
            else printf("Error in GetWidth %s\n",tor->t->lexeme);
            temp=temp->next;     
        }
        tor->width = acc;
        // printf("width set to %d\n",tor->width);
    }
}

int getWidth(TokOrRec tor){
    return tor->width;
}



void printRecordTable(RecordTable RT){
	if(RT==NULL) return;
	int i;
    printf(CYN"Record Declarations:\n"RESET);
    printf(GRN"Name\tComponent Types\n"RESET);
	for(i=0;i<No_Of_Record_Buckets;i++){
		printList(RT->RecL[i],printTokOrRec);
	}
}
void printVarTable(VariableTable VT){
	if(VT==NULL) return;
	int i;
	for(i=0;i<No_Of_Variable_Buckets;i++){
		printList(VT->varL[i],printVar);
	}
}
void printFuncTable(FuncTable FT){
	if(FT==NULL) return;
	int i;
	for(i=0;i<No_Of_Func_Buckets;i++){
		printList(FT->FuncL[i],printFunc);
	}
}

void printLocalVarTable(void *d){
	if(d==NULL) return;
	int i;
	for(i=0;i<No_Of_Variable_Buckets;i++){
		printList(((Func)d)->localVars->varL[i],printVar);
	}
}

void printAllVarTables(FuncTable FT){
	int i;
	for(i=0;i<No_Of_Func_Buckets;i++){
		printList(FT->FuncL[i],printLocalVarTable);
	}	
}


int compareFunc(void *d1, void *d2){//Compare two functions by name. Use for type extraction
    Func f1 = (Func) d1;
    Func f2 = (Func) d2;
    return (strcmp(f1->funID->lexeme,f2->funID->lexeme)==0);
}

int compareField(void *d1, void *d2){//Compare two Fields. Use for type checking of records
    Field f1 = (Field) d1;
    Field f2 = (Field) d2;
    return (strcmp(f1->info->lexeme,f2->info->lexeme)==0);
}

int compareVar(void *d1, void *d2){//compare two Vars by name, use for type extraction
    Var v1 = (Var) d1;
    Var v2 = (Var) d2;
    return (strcmp(v1->name,v2->name)==0);
}

int compareVarType(void *d1, void *d2){//compare two Vars by type, not name. use for typechecking.
    Var v1 = (Var) d1;
    Var v2 = (Var) d2;
    return (strcmp(v1->info->t->lexeme,v2->info->t->lexeme)==0);
}
int compareTokOrRec(void *d1, void *d2){
    TokOrRec t1 = (TokOrRec) d1;
    TokOrRec t2 = (TokOrRec) d2;
    return (strcmp(t1->t->lexeme,t2->t->lexeme)==0);
    //no further comparison required because name equivalence
}


Func newFunc(List inpar, List outpar,TokenInfo tok){
    Func f = malloc(sizeof(func));
    f->inpar = inpar;
    f->outpar = outpar;
    f->funID = tok;
    f->localVars = createNewVarTable();
    return f;
}
