#pragma once

#include <Arduino.h>
#include "Serial.h"
#include "readingSerial.h"
#include "convert.h"

#define IR_noDetectV 0b1111111111

class IR_read
{
private:
    /* data */
public:
    int deg;
    int dis;
    bool detected;
};


extern readingSerial IRdata;

extern IR_read IRv;

void IR_loop();