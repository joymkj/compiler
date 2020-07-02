#include "../parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char filename[100];
    //printf("Enter File name\n");
    //scanf("%s",filename);
    
    MapTable map = newMapTable();
    RevLookupHashTable HT = newRevLookupHashTable();
    Grammar G = text2list(map,HT,"ActualModifiedGrammar.txt");
    displayAllTermDetails(map);
    displayRules(G);
    
    printf("\n\n\n*********************DISPLAY FIRST LIST******************\n\n\n");
    int i=0;
    calculateFirst(map,G);

     for(i =0;i<map->numberOfTerms;i++){
         displayFirst(map,i-53);
     }
    printf("\n\n\n*********************DISPLAY FOLLOW LIST******************\n\n\n");
    calculateFollow(map,G);
    for(i = NO_TERMINALS;i<map->numberOfTerms;i++){
        displayFollow(map,i-53);
    }
    
    // ParseTable pt = createParseTable(map, G);
    // printPT(pt);
    return 0;

}
