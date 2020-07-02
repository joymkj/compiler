//
//  test.c
//  Compilers Project
//
//  Created by Prashant Rangarajan on 4/15/19.
//  Copyright © 2019 Prashant Rangarajan. All rights reserved.
//

#include "semanticAnalysis.h"

int main(){
    RevLookupHashTable KT = newKeyTable();
    LexBuf b = newBuf("testcase3.txt",512,KT);
    MapTable map = newMapTable();
    RevLookupHashTable HT = newRevLookupHashTable();
    Grammar G = text2list(map,HT,GRAMMARFILE);
    calculateFirst(map,G);
    calculateFollow(map,G);
    ParseTable pt = createParseTable(map, G);
    int rootSymbol = G->rules[0]->termID;
    TreeNode root = newNode(rootSymbol);
    parseTree(root,G,b,pt,map);
    ASTNode ast = createAST(root);
    RecordTable RT = createNewRecordTable();
    populateRecordTable(ast,RT);
    VariableTable GVT = createNewVarTable();
    FuncTable FT = createNewFuncTable();
    populateGlobalAndFunc(ast,GVT,FT,RT);
    populateLocal(ast,GVT,FT,RT);
    printAST(ast);
    //printRecordTable(RT);

    return 0;
}
