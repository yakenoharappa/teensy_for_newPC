#pragma once

#include <Arduino.h>
#include <vector>
#include "Serial.h"
#include "readingSerial.h"
#include "convert.h"
#include "Camera.h"
#include "Gyro/Gyro.h"




void LineRead_Setup();
void LineRead_update();


class Line
{
public:
    Line(int amount);
    int HowManyLine;
    float degs[16];
    //int port[16];
    float sumX;
    float sumY;
    float Linedegr;
    unsigned long last_detect_time;
    std::vector<float> old_Linedegr;
    //float old_Linedegr[16];
    bool Detect[16];
    std::vector<unsigned long> old_detect_times;

    bool Left;
    bool Back;
    bool Right;
    int number_of_detect;
};

extern Line Angel; 
extern readingSerial Linedata;




extern bool SideLineV[3];
extern bool LineNeed;
extern bool first_detected;
extern float first_deg;