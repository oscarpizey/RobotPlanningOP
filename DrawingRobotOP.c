#include <stdio.h>
#include <math.h>
#include <string.h>
#include "loadFont.h"
#include "structs.h"

int main()
{
    //struct fontData *pLetters = &letters[0];

    struct fontData letters[256];
    loadFont(&letters[0]);
    return 0;
}