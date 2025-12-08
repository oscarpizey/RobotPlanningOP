#include <stdio.h>
#include <math.h>
#include <string.h>

int getDrawWord(int filePosition, int *drawWord, int *x){
    int i=0;
    //printf("\ngetDrawWord called");
    FILE *drawFile;
    drawFile = fopen("test.txt","r");
    int positionMarker=0;
    while (drawWord[i-1] != 32 && drawWord[i-1] != -1){ //32 is ASCII for a space
        for (; positionMarker < filePosition; positionMarker++){
            fgetc(drawFile);
        }
        drawWord[i] = (fgetc(drawFile));
        //printf("\ncurrent character: %c, ascii: %d, x value: %d, filePosition: %d", drawWord[i], drawWord[i], i, filePosition);
        i++;
    }
    if (drawWord[i-1] == -1){
        i-=2;
    }
    filePosition += i;
    *x = i;
    fclose(drawFile);
    //printf("\ngetDrawWord finished");
    return(filePosition);
}