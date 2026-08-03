#pragma once

#include <Arduino.h>
#include "Serial.h"
#include "readingSerial.h"
#include "convert.h"


class IR_read
{
private:
    /* data */
public:
    int deg;
    int dis;
};


extern readingSerial IRdata;

extern IR_read IRv;

void IR_loop();