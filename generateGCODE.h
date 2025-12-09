#include <stdio.h>
#include <math.h>
#include <string.h>

void generateGCODE(float X, float Y, float P, float scale, float xOffset, float yOffset, int setPen, char *GCODE){
    if (setPen == 1){
        snprintf(GCODE, 128, "S%f G%f X%f Y%f\n", P*1000, P, ((X*scale) + xOffset), ((Y*scale) + yOffset));
    }else{
        snprintf(GCODE, 128, "G%f X%f Y%f\n", P, ((X*scale) + xOffset), ((Y*scale) + yOffset));
    }
    //printf("\nGCODE Function Output: %s", *GCODE);
}