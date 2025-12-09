#include <stdio.h>
#include <math.h>
#include <string.h>

int getDrawWord(int filePosition, int *drawWord, int *x){
    int i=0;    // variable used to keep track of pointer x and return back to it at end
    FILE *drawFile;
    drawFile = fopen("test.txt","r");
    int positionMarker=0;   // keeps track of current position within file
    while (drawWord[i-1] != 32 && drawWord[i-1] != -1 && drawWord[i-1] != 10){ // keep reading letters until either a space, EOF, or newline detected

        // the while loop checks the character before, since i++ called just before comparison.

        for (; positionMarker < filePosition; positionMarker++){    // iterate through to required starting position
            fgetc(drawFile);    // disregard character since not needed
        }
        drawWord[i] = (fgetc(drawFile));    // once starting position reached, start writing word into char array
        //printf("\ncurrent character: %c, ascii: %d, x value: %d, filePosition: %d", drawWord[i], drawWord[i], i, filePosition);
        i++;    // iterate position in drawWord
    }
    i--;    //last letter is a space, so step x back so program doesn't write it
    if (drawWord[i] == -1){
        i--;    //last letter is EOF, so remove another character
    }
    filePosition += i+1;    // increment filePosition by the length of the word
    *x = i;     // update pointer to x
    fclose(drawFile);
    //printf("\ngetDrawWord finished");
    return(filePosition);
}