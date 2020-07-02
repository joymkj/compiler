#include "firstfollow.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char filename[100];
    //printf("Enter File name\n");
    //scanf("%s",filename);

    MapTable map = newMapTable();
    Grammar G = text2list(map,"ActualModifiedGrammar.txt");

    printf("\n*********************DISPLAY FIRST LIST******************\n");

    calculateFirst(map,G);
    int i=0;
    for(i =-53;i<53;i++){
        displayFirst(map,i);
    }

    return 0;

}
