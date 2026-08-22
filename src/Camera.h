#pragma once

#include <Arduino.h>

#include "Serial.h"
#include "readingSerial.h"
#include "convert.h"

extern readingSerial Camera;

class CameraData
{
public:
    int court_deg;
    int yellow_deg;
    int yellow_dis;
    int blue_deg;
    int blue_dis;
    int orange_deg;
    int orange_dis;
};

extern CameraData CameraV;


void Camera_update();

extern bool SettingGoal;
extern bool Delection_Mode;
extern bool DEForATTA;
extern bool CamBallDetected;

extern int GoalDis;
extern int GoalDeg;

extern bool CamGoalDetected;