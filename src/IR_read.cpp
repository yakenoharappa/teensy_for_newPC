#include "IR_read.h"

readingSerial IRdata(IRSerial, 0xAA, 0xBB, 3);

IR_read IRv;

void IR_loop()
{
    IRdata.readData();
    IRv.deg = BitChange(IRdata.values[0], IRdata.values[1]);
    IRv.dis = IRdata.values[2];
    if (IRv.deg == IR_noDetectV)
    {
        IRv.detected = false;
    }
    else
    {
        IRv.detected = true;
    }
    
    //Serial.print("Serial7IR:");
    //Serial.println(Serial7.read());
}