#include "parser.h"
#include<time.h>

int main(int argc, char** argv){
    //argv[1] will be testcase1.txt
    //argv[2] will be parseTreeOutFile.txt
    if (argc < 3) {
        printf("Insufficient number of arguments have been provided.\n Please provide %d more.\n", 3-argc);
        return(1);
    }
    char inbuf[4];
    int input=-1; 
    while(input!=0){
        printf("Enter command: ");
        fgets(inbuf,4,stdin);
        input = atoi(inbuf);
        if(input==1){
            removeComments(argv[1],"1");
        }
        else if (input==2){
            printf("Printing tokens...\n");
             RevLookupHashTable HT = newRevLookupHashTable();
            LexBuf b = newBuf(argv[1],512,HT);
            while(1){
                TokenInfo tk = getToken(b);
                if(tk->token==DOLLAR){
                    printToken(tk);
                    printf("\nNo more tokens\n");
                    break;
                }
                else printToken(tk);
            }
        }
        else if (input==3){
             RevLookupHashTable HT = newRevLookupHashTable();
            LexBuf b = newBuf(argv[1],512,HT);
            MapTable map = newMapTable();
            // RevLookupHashTable HT = newRevLookupHashTable();
            Grammar G = text2list(map,HT,"ActualModifiedGrammar.txt");
            calculateFirst(map,G);
            calculateFollow(map,G);
            ParseTable pt = createParseTable(map, G);
            TreeNode tree = parseTree(G,b,pt,map);
            FILE* f = fopen(argv[2],"w");
        fprintf(f,"lexeme\tCurrentNode\tlineno\ttokenName\tvalueIfNumber\tparentNodeSymbol\tisLeafNode\tNodeSymbol\n");
            inOrder(tree,map,f);
            
            printf("\nParse tree written to %s succesfully.\nCompilation is complete.\n", argv[2]);
        }
        else if (input==4){
            clock_t    start_time, end_time;
            double total_CPU_time, total_CPU_time_in_seconds;
            start_time = clock();
            RevLookupHashTable HT = newRevLookupHashTable();
            LexBuf b = newBuf(argv[1],512,HT);
            MapTable map = newMapTable();
            Grammar G = text2list(map,HT,"ActualModifiedGrammar.txt");
            calculateFirst(map,G);
            calculateFollow(map,G);
            ParseTable pt = createParseTable(map, G);
            TreeNode tree = parseTree(G,b,pt,map);
            FILE* f = fopen(argv[2],"w");
        fprintf(f,"lexeme\tCurrentNode\tlineno\ttokenName\tvalueIfNumber\tparentNodeSymbol\tisLeafNode\tNodeSymbol\n");
            inOrder(tree,map,f);
            end_time = clock();
            total_CPU_time  =  (double) (end_time - start_time);
            total_CPU_time_in_seconds =  total_CPU_time / CLOCKS_PER_SEC;
            printf("Total CPU Time: %f seconds.\n",total_CPU_time_in_seconds);
        }
    }
}
