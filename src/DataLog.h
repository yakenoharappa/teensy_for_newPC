#pragma once

#include <Arduino.h>

class DataLog
{
private:
    /* data */
public:
    int amount;
    int LogData[10];
    unsigned long Last_log_time;
    uint log_pace;
    std::vector<int> values;

    DataLog(int amount);
    void Log();
};

DataLog::DataLog(int amount) : values(amount)
{


    
}

void DataLog::Log()
{
    if ((millis() - Last_log_time) > log_pace)
    {
        for (int i = (amount-1); i > 0; i--)
        {
            values[i] = values[i-1];
        }
    }
}