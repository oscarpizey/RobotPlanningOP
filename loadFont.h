#include <stdio.h>
#include <math.h>
#include <string.h>
#include "structs.h"

int loadFont(struct fontData *letters)      // reads straight into pointed array, eliminates messy return.
{
    FILE *file;
    file = fopen("SingleStrokeFont.txt", "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    char line[128];
    //structure comprised of an array of all the characters
    int asciiCode;
    int codeLines;
    while (fgets(line, sizeof(line), file) != NULL) //iterate through lines until end of file
    {
        if (sscanf(line,"999 %d %d", &asciiCode, &codeLines) != -1){  //scan line for 999 (meaning start of a character)
            letters[asciiCode].ascii = asciiCode;   //append metadata to structure
            letters[asciiCode].lines = codeLines;
            int x; int y; int p;
            int i=0;
            for (i=0; i<codeLines; i++) //iterate over character through the amount of codelines it has
            {
                fgets(line, sizeof(line), file);    //read next line of file
                sscanf(line, "%d %d %d", &x, &y, &p);   //scan for 3 values within the line and output as integers
                letters[asciiCode].x[i] = x;    //append scan data into the array
                letters[asciiCode].y[i] = y;
                letters[asciiCode].p[i] = p;
                //printf("ASCII: %d, LINES: %d\n%d %d %d\n",letters[asciiCode].ascii,letters[asciiCode].lines,letters[asciiCode].x[i],letters[asciiCode].y[i],letters[asciiCode].p[i]);
            }
        }
    }
    fclose(file);
    return 0;
}