#include <stdio.h>
#include <math.h>
#include <string.h>
#include "loadFont.h"
#include "structs.h"

void generateGCODE(float X, float Y, float P, float scale, float xOffset, float yOffset, char *GCODE);


int main()
{
    struct fontData letters[256];
    loadFont(&letters[0]);

    float letterHeight;
    printf("\nEnter letter height: ");
    scanf("%f", &letterHeight);
    float letterScale = letterHeight/18;

    FILE *drawFile;
    drawFile = fopen("testText.txt","r");
    char drawWord[128];
    int x=0;
    int filePosition = 4;
    int positionMarker = 0;
    while (1){
        for (positionMarker; positionMarker < filePosition; positionMarker++){
            fgetc(drawFile);
        }
        drawWord[x] = (fgetc(drawFile));
        if (drawWord[x] == 32){
            filePosition = x;
            break;
        }
        x++;
    }
    fclose(drawFile);
    //printf("\ndrawWord: %s", drawWord);

    float remainingWidth = 100;
    float wordWidth = 0;
    float xOffset = 0;
    float yOffset = 0;
    char initialGCODE[3][12] = {"F1000", "M3", "S0"};
    int drawLetter;
    char GCODELine[128];

    for (drawLetter = 0; drawLetter <= filePosition; drawLetter++){
        wordWidth += letters[drawWord[drawLetter]].x[letters[drawWord[drawLetter]].lines - 1] * letterScale;
        //printf("\ncurrent letter printing: %c, wordWidth: %f", drawWord[drawLetter], wordWidth);
    }

    if (wordWidth > 100){
        printf("Error - Word too long");
    } else{
        if (wordWidth > remainingWidth){
            yOffset += (5 + letterScale);
            xOffset = 0;
        }
        int currentLine;
        for (drawLetter = 0; drawLetter <= filePosition; drawLetter++){
            for (currentLine = 0; currentLine <= letters[drawWord[drawLetter]].lines; currentLine++){
                //printf("\ncharacter: %c, printing line: %d", drawWord[drawLetter], currentLine);
                generateGCODE(letters[drawWord[drawLetter]].x[currentLine], letters[drawWord[drawLetter]].y[currentLine], letters[drawWord[drawLetter]].p[currentLine], letterScale, xOffset, yOffset, GCODELine);
                printf("\n%s", GCODELine);
            }
            xOffset += letters[drawWord[drawLetter]].x[letters[drawWord[drawLetter]].lines - 1] * letterScale;
        }
    }

    return 0;
}

void generateGCODE(float X, float Y, float P, float scale, float xOffset, float yOffset, char *GCODE){
    int j = snprintf(GCODE, 128, "S%f G%f X%f Y%f", P*1000, P, ((X*scale) + xOffset), ((Y*scale) + yOffset));
    //printf("GCODE Function Output: %d", *GCODE);
}    //add setpen after testing