#include <stdio.h>
#include <stdlib.h>

#include <xc.h>

#include "Common.h"

int tIEC0, tIEC1, tIEC2, tIEC3, tIEC4;

void INT_OFF() {
    tIEC0 = IEC0;
    tIEC1 = IEC1;
    tIEC2 = IEC2;
    tIEC3 = IEC3;
    tIEC4 = IEC4;
    IEC0 = 0;
    IEC1 = 0;
    IEC2 = 0;
    IEC3 = 0;
    IEC4 = 0;
}

void INT_ON() {
    IEC0 = tIEC0;
    IEC1 = tIEC1;
    IEC2 = tIEC2;
    IEC3 = tIEC3;
    IEC4 = tIEC4;
}
