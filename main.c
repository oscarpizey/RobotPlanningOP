#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
//#include <windows.h>
#include "rs232.h"
#include "serial.h"
#include "structs.h"
#include "loadFont.h"
#include "getDrawWord.h"

#define bdrate 115200               /* 115200 baud */

void SendCommands (char *buffer );

void generateGCODE(float X, float Y, float P, float scale, float xOffset, float yOffset, char *GCODE);

int main()
{

    //char mode[]= {'8','N','1',0};
    char buffer[100];
    
    // If we cannot open the port then give up immediately
    if ( CanRS232PortBeOpened() == -1 )
    {
        printf ("\nUnable to open the COM port (specified in serial.h) ");
        exit (0);
    }

    // Time to wake up the robot
    printf ("\nAbout to wake up the robot\n");

    // We do this by sending a new-line
    sprintf (buffer, "\n");
     // printf ("Buffer to send: %s", buffer); // For diagnostic purposes only, normally comment out
    PrintBuffer (&buffer[0]);
    Sleep(100);

    // This is a special case - we wait  until we see a dollar ($)
    WaitForDollar();

    printf ("\nThe robot is now ready to draw\n");

        //These commands get the robot into 'ready to draw mode' and need to be sent before any writing commands
    sprintf (buffer, "G1 X0 Y0 F1000\n");
    SendCommands(buffer);
    sprintf (buffer, "M3\n");
    SendCommands(buffer);
    sprintf (buffer, "S0\n");
    SendCommands(buffer);
    
    struct fontData letters[256];
    loadFont(&letters[0]);

    float letterHeight;
    printf("\nEnter letter height: ");
    scanf("%f", &letterHeight);
    float letterScale = letterHeight/18;

    int drawWord[24];
    //int whiteSpace[4] = {32,10,13,-1};
    int filePosition = 0;
    int x = 0;
    int totalLines=0;
    float remainingWidth = 100;
    float wordWidth;
    float xOffset = 0;
    float yOffset = 0;
    int drawLetter;
    char GCODELine[256];

    while (drawWord[x+1] != -1){ //check for end of file

        //memset(drawWord,0,sizeof(drawWord[0]));
        filePosition = getDrawWord(filePosition, &drawWord[0], &x);
        wordWidth = 0;

        for (drawLetter = 0; drawLetter <= x; drawLetter++){
            wordWidth += letters[drawWord[drawLetter]].x[letters[drawWord[drawLetter]].lines - 1] * letterScale;
            //printf("\ncurrent letter printing: %c, ascii: %d, wordWidth: %f, remaining width: %f", drawWord[drawLetter], drawWord[drawLetter], wordWidth, remainingWidth);
        }

        if (wordWidth > 100){
            printf("\nError - Word too long");
        } else{
            int currentLine;
            remainingWidth = 100 - xOffset;
            for (drawLetter = 0; drawLetter <= x; drawLetter++){
                if (wordWidth > remainingWidth || letters[drawWord[drawLetter]].ascii == 10){
                    yOffset -= (5 + letterHeight);
                    xOffset = 0;
                    remainingWidth = 100;
                    //printf("\nNewline triggered, remaining width = %f", remainingWidth);
                }
                for (currentLine = 0; currentLine <= letters[drawWord[drawLetter]].lines-1; currentLine++){
                    totalLines+=1;
                    //printf("\ncharacter: %c, printing line: %d, required lines: %d, totalLines: %d", drawWord[drawLetter], currentLine, letters[drawWord[drawLetter]].lines, totalLines);                    
                    generateGCODE(letters[drawWord[drawLetter]].x[currentLine], letters[drawWord[drawLetter]].y[currentLine], letters[drawWord[drawLetter]].p[currentLine], letterScale, xOffset, yOffset, GCODELine);
                    SendCommands(GCODELine);
                    printf("\n%s", GCODELine);
                }
                xOffset += letters[drawWord[drawLetter]].x[letters[drawWord[drawLetter]].lines - 1] * letterScale;
            }
        }
    }
    CloseRS232Port();
    printf("Com port now closed\n");

    return (0);
}

// Send the data to the robot - note in 'PC' mode you need to hit space twice
// as the dummy 'WaitForReply' has a getch() within the function.
void SendCommands (char *buffer )
{
    // printf ("Buffer to send: %s", buffer); // For diagnostic purposes only, normally comment out
    //PrintBuffer (&buffer[0]);
    //printf("Send Command: %s", buffer);
    //WaitForReply();
    //Sleep(100); // Can omit this when using the writing robot but has minimal effect
    // getch(); // Omit this once basic testing with emulator has taken place
}

void generateGCODE(float X, float Y, float P, float scale, float xOffset, float yOffset, char *GCODE){
    snprintf(GCODE, 128, "S%f G%f X%f Y%f\n", P*1000, P, ((X*scale) + xOffset), ((Y*scale) + yOffset));
    //printf("\nGCODE Function Output: %s", *GCODE);
}    //add setpen after testing