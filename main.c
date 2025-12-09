#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
//#include <windows.h>
#include "rs232.h"
#include "serial.h"
#include "structs.h"
#include "loadFont.h"
#include "getDrawWord.h"
#include "generateGCODE.h"

#define bdrate 115200               /* 115200 baud */

void SendCommands (char *buffer );

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
    
    struct fontData letters[256];   // allocates space for each letter within the structure
    loadFont(&letters[0]);  // loads the font into the structure, starting at [0]

    float letterHeight;
    printf("\nEnter letter height: ");  // prompts the user for input of letter height
    scanf("%f", &letterHeight);         // reads input into float
    while (letterHeight < 4 || letterHeight > 10){      // checks validity in number range
        printf("\nLetter height entered is invalid\nEnter letter height: ");
        scanf("%f", &letterHeight);
    }

// Initialise variables

    float letterScale = letterHeight/18;   // scale the letter height by the standard unit height

    int drawWord[24];   // the current word to be drawn, longer than 24 characters at 4 height is too long for a line.

    int filePosition = 0;  // position of the next word to be read out. Increments by the word length each loop.

    int x;    // variable used to iterate over each letter in the word within the file

    float remainingWidth = 100;    // how much space is available on the line, initialised to width of canvas

    float wordWidth;    // float for the calculated width of the current word

    float xOffset = 0;    // how much to offset each x value in the GCODE

    float yOffset = 0;    // how much to offset each y value in the GCODE

    int penCommand;    // indicates whether to update the robot's pen state,

    int drawLetter;    // used to iterate over word within the FOR loop

    char GCODELine[256];    // char array to store the GCODE output before being sent to the buffer

    int currentLine;    // iterates over each line of instructions within a letter



    while (drawWord[x+1] != -1){    //check for end of file

        filePosition = getDrawWord(filePosition, &drawWord[0], &x);     // update filePosition to mark start of next word in the file
        wordWidth = 0;  //reset the word width

        for (drawLetter = 0; drawLetter <= x; drawLetter++){    // iterate over the word
            wordWidth += letters[drawWord[drawLetter]].x[letters[drawWord[drawLetter]].lines - 1] * letterScale;    // add the width of each letter into wordWidth
            //printf("\ncurrent letter printing: %c, ascii: %d, wordWidth: %f, remaining width: %f", drawWord[drawLetter], drawWord[drawLetter], wordWidth, remainingWidth);
        }

        if (wordWidth > 100){   //checks if the word is wider than the canvas, therefore impossible
            printf("\nError - Word too long");
        } else{
            remainingWidth = 100 - xOffset;     // calculates the remaining width on the line
            for (drawLetter = 0; drawLetter <= x; drawLetter++){    // iterates over each letter in the word
                if (wordWidth > remainingWidth || letters[drawWord[drawLetter]].ascii == 10 || letters[drawWord[drawLetter]].ascii == 13){      // checks for newline conditions (longer than remaining space, \n character, carriage return character)
                    yOffset -= (5 + letterHeight);  // spaces the next line by 5
                    xOffset = 0;    // resets the cursor to the start of the line
                    remainingWidth = 100;   // resets the space on the line
                    //printf("\nNewline triggered, remaining width = %f", remainingWidth);
                }
                for (currentLine = 0; currentLine <= letters[drawWord[drawLetter]].lines-1; currentLine++){     // iterates over each instruction in the letter
                    //printf("\ncharacter: %c, printing line: %d, required lines: %d", drawWord[drawLetter], currentLine, letters[drawWord[drawLetter]].lines);
                    if (letters[drawWord[drawLetter]].p[currentLine] != letters[drawWord[drawLetter]].p[(currentLine-1)] || currentLine == 0){      // checks if the pen state has changed since the last letter. If first instruction, prompts a state change.
                        penCommand = 1;
                    }else{
                        penCommand = 0;
                    }                    
                    generateGCODE(letters[drawWord[drawLetter]].x[currentLine], letters[drawWord[drawLetter]].y[currentLine], letters[drawWord[drawLetter]].p[currentLine], letterScale, xOffset, yOffset, penCommand, GCODELine);      // convert line of instructions into GCODE
                    SendCommands(GCODELine);    // send commands to robot
                    //printf("\n%s", GCODELine);
                }
                xOffset += letters[drawWord[drawLetter]].x[letters[drawWord[drawLetter]].lines - 1] * letterScale;  // increment the xOffset by the width of the letter
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
    PrintBuffer (&buffer[0]);
    //printf("Send Command: %s", buffer);
    WaitForReply();
    Sleep(100); // Can omit this when using the writing robot but has minimal effect
    // getch(); // Omit this once basic testing with emulator has taken place
}