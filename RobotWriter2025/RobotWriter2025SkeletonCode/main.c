#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
//#include <windows.h>
#include "rs232.h"
#include "serial.h"
#include "structs.h"
#include "loadFont.h"

#define bdrate 115200               /* 115200 baud */

void SendCommands (char *buffer );

void generateGCODE(float X, float Y, float P, float scale, float xOffset, float yOffset, char *GCODE);

int main()
{

    //char mode[]= {'8','N','1',0};
    char buffer[100];
    /*
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
    */
    struct fontData letters[256];
    loadFont(&letters[0]);

    float letterHeight;
    printf("\nEnter letter height: ");
    scanf("%f", &letterHeight);
    float letterScale = letterHeight/18;

    FILE *drawFile;
    drawFile = fopen("testText.txt","r");
    int drawWord[128];
    int x=0;
    int filePosition = 4;
    int positionMarker = 0;
    while (1){
        for (; positionMarker < filePosition; positionMarker++){
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
    // printf("\ndrawWord: %s", drawWord);

    float remainingWidth = 100;
    float wordWidth = 0;
    float xOffset = 0;
    float yOffset = 0;
    int drawLetter;
    char GCODELine[100];

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
                //printf("\ncharacter: %c, printing line: %d, required lines: %d", drawWord[drawLetter], currentLine, letters[drawWord[drawLetter]].lines);
                generateGCODE(letters[drawWord[drawLetter]].x[currentLine], letters[drawWord[drawLetter]].y[currentLine], letters[drawWord[drawLetter]].p[currentLine], letterScale, xOffset, yOffset, GCODELine);
                SendCommands(GCODELine);
                //printf("\nGCODE Function Output: %s", GCODELine);
            }
            xOffset += letters[drawWord[drawLetter]].x[letters[drawWord[drawLetter]].lines - 1] * letterScale;
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
    PrintBuffer (&buffer[0]);
    //printf("Send Command: %s", buffer);
    //WaitForReply();
    Sleep(100); // Can omit this when using the writing robot but has minimal effect
    // getch(); // Omit this once basic testing with emulator has taken place
}

void generateGCODE(float X, float Y, float P, float scale, float xOffset, float yOffset, char *GCODE){
    snprintf(GCODE, 128, "S%f G%f X%f Y%f\n", P*1000, P, ((X*scale) + xOffset), ((Y*scale) + yOffset));
    //printf("\nGCODE Function Output: %s", *GCODE);
}    //add setpen after testing