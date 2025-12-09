#include <stdio.h>
#include <math.h>
#include <string.h>

void generateGCODE(float X, float Y, float P, float scale, float xOffset, float yOffset, int setPen, char *GCODE){  // no return, writes straight to *GCODE pointer
    if (setPen == 1){   // decides whether to send a pen update command or not
        snprintf(GCODE, 128, "S%f G%f X%f Y%f\n", P*1000, P, ((X*scale) + xOffset), ((Y*scale) + yOffset)); // appends instruction values to GCODE markers and concatenates to a string
    }else{
        snprintf(GCODE, 128, "G%f X%f Y%f\n", P, ((X*scale) + xOffset), ((Y*scale) + yOffset)); // as above, but without pen value
    }
    //printf("\nGCODE Function Output: %s", *GCODE);
}