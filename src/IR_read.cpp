#include "IR_read.h"

readingSerial IRdata(IRSerial, 0xAA, 0xFF, 3);

IR_read IRv;

void IR_loop()
{
    IRdata.readData();
    IRv.deg = BitChange(IRdata.values[0], IRdata.values[1]);
    IRv.dis = IRdata.values[2];
    //Serial.print("Serial7IR:");
    //Serial.println(Serial7.read());
}