#include "parser.h"
#include<time.h>

#define GRAMMARFILE "grammar.txt"

extern int errFlag;

int main(int argc, char** argv){
    //argv[1] will be testcase1.txt
    //argv[2] will be parseTreeOutFile.txt
    if (argc < 3) {
        printf(RED"Insufficient number of arguments have been provided.\n"RESET
        "Please provide %d more.\n",3-argc );
        return(1);
    }
    char inbuf[4];
    int input=-1,count=0; 
    printf(YEL"------------IMPLEMENTATION STATUS------------\n"RESET
        "Both lexer and parser modules implemented.\n"
        "First and Follow set calculation has been automated.\n"
        "All four given testcases work without segmentation faults.\n"
        YEL"------------------------------------------------------------\n"
        "Commands:\n"RESET
        "0: to exit.\n"
        "1: for printing comment-free code on the console.\n"
        "2: for printing a list of tokens.\n"
        "3: for parsing and printing parse tree to"BLU" %s.\n"RESET
        "4: for time-calculation.\n",argv[2]);

    while(input!=0){
        printf(YEL"Enter command: "RESET);
        fgets(inbuf,4,stdin);
        input = atoi(inbuf);
        if(input==1){
            printf("Printing de-commented code:\n");
            removeComments(argv[1],"1");
            printf(YEL"De-commented code has been printed above.\n"RESET);
        }
        else if (input==2){
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
        else if (input==3){
            RevLookupHashTable KT = newKeyTable();
            LexBuf b = newBuf(argv[1],512,KT);
            MapTable map = newMapTable();
            RevLookupHashTable HT = newRevLookupHashTable();
            Grammar G = text2list(map,HT,GRAMMARFILE);
            calculateFirst(map,G);
            calculateFollow(map,G);
            ParseTable pt = createParseTable(map, G);
            TreeNode tree = parseTree(G,b,pt,map);
            FILE* f = fopen(argv[2],"w");
            printInOrder(tree,map,f);
            printf("Parse tree written to"BLU" %s "RESET"succesfully.\n", argv[2]);
            if(!errFlag)
            printf(GRN"No syntax errors found.\n"RESET);
            else
            {
                printf(GRN"Syntax analysis is complete,"YEL" but with errors.\n"RESET);
            }
        }
        else if (input==4){
            clock_t    start_time, end_time;
            double total_CPU_time, total_CPU_time_in_seconds;
            start_time = clock();
            RevLookupHashTable KT = newKeyTable();
            RevLookupHashTable HT = newRevLookupHashTable();
            LexBuf b = newBuf(argv[1],512,KT);
            MapTable map = newMapTable();
            Grammar G = text2list(map,HT,GRAMMARFILE);
            calculateFirst(map,G);
            calculateFollow(map,G);
            ParseTable pt = createParseTable(map, G);
            TreeNode tree = parseTree(G,b,pt,map);
            FILE* f = fopen(argv[2],"w");
            printInOrder(tree,map,f);
            printf("Parse tree written to"BLU" %s "RESET"succesfully.\n", argv[2]);
            if(!errFlag)
            printf(GRN"No syntax errors found.\n"RESET);
            else
            {
                printf(GRN"Syntax analysis is complete,"YEL" but with errors.\n"RESET);
            }
            

            end_time = clock();
            total_CPU_time  =  (double) (end_time - start_time);
            total_CPU_time_in_seconds =  total_CPU_time / CLOCKS_PER_SEC;
            printf("Total CPU Time: %f seconds.\n",total_CPU_time_in_seconds);
        }
    }
    if(input==0) printf(YEL"----------------------------------------------\n"
    CYN"Exit command received. Goodbye!\n"RESET); 
}
