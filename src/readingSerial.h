#pragma once

#include <Arduino.h>
#include <cstdint>
#include <vector>
#include "Serial.h"


//template <int Size>
class readingSerial
{
private:
    //int8_t checkV[16];
    std::vector<uint8_t> checkV;
public:
    
    int SerialNumber;
    uint8_t Start;
    uint8_t End;
    int amountData;
    std::vector<uint8_t> values;


    readingSerial(int number, uint8_t StartC, uint8_t endC, int amount);
    //int8_t data[amount] = {0};
    void readData();
};
