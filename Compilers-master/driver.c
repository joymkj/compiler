//Group 7:
//Prashant Rangarajan 2015B4A70557P
//Vighnesh Hegde 2015B4A70342P
//Biswajit Biswas 2015B5A70418P
//Joy Mukherjee 2015B5A70394P

#include "codeGen.h"
#include<time.h>


int main(int argc, char** argv){
    //argv[1] will be testcase1.txt
    //argv[2] will be code.asm
    errAST =0 ;
    if (argc < 3) {
        printf(RED"Insufficient number of arguments have been provided.\n"RESET
        "Please provide %d more.\n",3-argc );
        return(1);
    }
    char inbuf[4];
    int input=-1,count=0; 
    printf(YEL"------------IMPLEMENTATION STATUS------------\n"RESET
        CYN"LEVEL 4:\n"RESET"AST/Symbol table/Semantic Rules/Type Extractor/Type Checker/ modules work.\n"
        "All three given testcases for semantic errors report all the given errors.\n"
        "Assembly Code is generated successfully.\n"
        YEL"------------------------------------------------------------\n"
        "Commands:\n"RESET
        " 0: to exit.\n"
        " 1: for printing a list of tokens.\n"
        " 2: for parsing and printing parse tree.\n"
        " 3: for printing the Abstract Syntax Tree.\n"
        " 4: for time-calculation.\n"
        " 5: for printing out symbol table. \n"
        " 6: for printing out global table. \n"
        " 7: for printing total memory requirement for each function.\n"
        " 8: for printing out record table. \n"
        " 9: total time taken by the integrated compiler.\n"
        "10: for producing Assembly code.\n"
        );

    while(input!=0){
        printf(YEL"Enter command: "RESET);
        fgets(inbuf,4,stdin);
        input = atoi(inbuf);
        if (input==1){
            RevLookupHashTable KT = newKeyTable();
            LexBuf b = newBuf(argv[1],512,KT);
            printf("Printing tokens...\n");
            while(1){
                TokenInfo tk = getToken(b);
                if(tk->token==DOLLAR){
                    printToken(tk);
                    printf("%d tokens read.\n"GRN"Lexical analysis complete.\n"RESET,count);
                    break;
                }
                else {
                    printToken(tk);
                    ++count;
                }
            }
        }
        else if (input==2){
            RevLookupHashTable KT = newKeyTable();
            LexBuf b = newBuf(argv[1],512,KT);
            MapTable map = newMapTable();
            RevLookupHashTable HT = newRevLookupHashTable();
            Grammar G = text2list(map,HT,GRAMMARFILE);
            calculateFirst(map,G);
            calculateFollow(map,G);
            ParseTable pt = createParseTable(map, G);
            int rootSymbol = G->rules[0]->termID;
            TreeNode root = newNode(rootSymbol);
            int errFlag = parseTree(root, G,b,pt,map);
            printInOrder(root,map,stdout);
            printf("Parse tree printed succesfully.\n");
            if(!errFlag)
            printf(GRN"No syntax errors found.\n"RESET);
            else
            {
                printf(GRN"Syntax analysis is complete,"YEL" but with errors.\n"RESET);
            }

        }
        
        else if (input == 3){
            RevLookupHashTable KT = newKeyTable();
            LexBuf b = newBuf(argv[1],512,KT);
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
            
        }
        
        else if (input == 4){
            RevLookupHashTable KT = newKeyTable();
            LexBuf b = newBuf("main1.txt",512,KT);
            MapTable map = newMapTable();
            RevLookupHashTable HT = newRevLookupHashTable();
            Grammar G = text2list(map,HT,GRAMMARFILE);
            calculateFirst(map,G);
            calculateFollow(map,G);
            ParseTable pt = createParseTable(map, G);
            int rootSymbol = G->rules[0]->termID;
            TreeNode root = newNode(rootSymbol);
            parseTree(root,G,b,pt,map);
            long mem = getMem();
            long mem2;
            printf("ParseTree Created. Number of nodes = %ld\n"
                   "Memory allocated =  %ld bytes\n",mem/sizeof(struct treeNode), mem);
            clearMem();
           
            createAST(root);
            mem2 = getMem();
            printf("Abstract Syntax Tree Created. Number of nodes = %ld\n"
                   "Memory allocated =  %ld bytes\n",mem2/sizeof(struct astNode),mem2);
            printf("Compression percentage =  %.2f%%.\n", (float)(mem-mem2)/mem*100);
        }
        else if(input == 5){
            RevLookupHashTable KT = newKeyTable();
            LexBuf b = newBuf(argv[1],512,KT);
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
            printf(CYN"Name\tType\tScope\tOffset\n"RESET);
            printAllVarTables(FT);
            printf(GRN"Symbol Table has been printed.\n"RESET);
        }
        
        else if(input == 6){
            RevLookupHashTable KT = newKeyTable();
            LexBuf b = newBuf(argv[1],512,KT);
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
            printf(CYN"Name\tType\tScope\tOffset\n"RESET);
            printVarTable(GVT);
            printf(GRN"Global Symbol Table has been printed.\n"RESET);
        }
        
        else if (input== 7){
            RevLookupHashTable KT = newKeyTable();
            LexBuf b = newBuf(argv[1],512,KT);
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
            printFuncSize(FT);
            
        }
        
        else if(input == 8){
            RevLookupHashTable KT = newKeyTable();
            LexBuf b = newBuf(argv[1],512,KT);
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
            printRecordTable(RT);
        }
        else if (input==9){
            clock_t    start_time, end_time;
            double total_CPU_time, total_CPU_time_in_seconds;
            start_time = clock();
            RevLookupHashTable KT = newKeyTable();
            LexBuf b = newBuf(argv[1],512,KT);
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
            checkType(ast,GVT,FT,RT);
            if (!errAST){
                printf(RED"No errors found!!\n"RESET);
            }
            end_time = clock();
            total_CPU_time  =  (double) (end_time - start_time);
            total_CPU_time_in_seconds =  total_CPU_time / CLOCKS_PER_SEC;
            printf("Total CPU Time: %.2f\n",total_CPU_time);
            printf("Total CPU Time in seconds: %f\n",total_CPU_time_in_seconds);
        }
        else if(input == 10){
            RevLookupHashTable KT = newKeyTable();
            LexBuf b = newBuf(argv[1],512,KT);
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
            checkType(ast,GVT,FT,RT);
            
            FILE* f = fopen(argv[2],"w");
            if(f==NULL) 
            {
                perror("fopen: ");
            }
            Code cd = initialiseCode();
            generateCode(ast,NULL,GVT,RT,cd);
            printToFile(f,cd);
            printSubRoutines(f);
            printf(GRN"Instructions: type\n"CYN"\tnasm -f elf code.asm\n\tld -m elf_i386 code.o -o code\n"GRN
            "to assemble the code using nasm and\n"CYN"\t./code\n"GRN"to run. Alternatively, just type"
            CYN"\n\tmake nasm\n"GRN"on the terminal.\n"RESET
            "NOTE: Please give input to assembly code line-by-line. Space separated inputs are not supported.\n");
        }
    }
    if(input==0) printf(YEL"----------------------------------------------\n"
    CYN"Exit command received. Goodbye!\n"RESET); 
}


