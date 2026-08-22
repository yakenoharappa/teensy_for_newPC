#include "Camera.h"

bool SettingGoal = 0;
bool Delection_Mode = 0;
bool DEForATTA = 1;

bool CamBallDetected = 0;
bool CamGoalDetected = 0;
int GoalDis = 0;
int GoalDeg = 0;

readingSerial Camera(cameraSerial, 0x55, 0xAA, 14);
                                            //　↑この数には、STARTとENDは含まない

CameraData CameraV;


void Camera_update()
{
    Camera.readData();
    //Serial.print(Serial3.read());
    //Serial.print("VALUES");
    //Serial.println(Camera.values[10]);
    CameraV.court_deg = int16_t(BitChange(Camera.values[0], Camera.values[1]));
    CameraV.yellow_deg = int16_t(BitChange(Camera.values[2], Camera.values[3]));
    CameraV.yellow_dis = BitChange(Camera.values[4], Camera.values[5]);
    CameraV.blue_deg = int16_t(BitChange(Camera.values[6], Camera.values[7]));
    CameraV.blue_dis = BitChange(Camera.values[8], Camera.values[9]);
    CameraV.orange_deg = int16_t(BitChange(Camera.values[10], Camera.values[11]));
    CameraV.orange_dis = BitChange(Camera.values[12], Camera.values[13]);

    if (uint16_t(CameraV.orange_deg) == 0xFF)
    {
        CamBallDetected = false;
    }
    else
    {
        CamBallDetected = true;
    }

    if (SettingGoal == 1)
    {
        GoalDis = CameraV.yellow_dis;
        GoalDeg = CameraV.yellow_deg;
    }
    else
    {
        GoalDis = CameraV.blue_dis;
        GoalDeg = CameraV.blue_deg;
    }

    if (uint16_t(GoalDis) == 0xFF && uint16_t(GoalDeg) == 0xFF)
    {
        CamGoalDetected = false;
    }
    else
    {
        CamGoalDetected = true;
    }
}


/* 
    START = 0x55
    court_deg
    yellow_deg
    yellow_dis
    blue_deg
    blue_dis
    orange_deg
    orange_dis
    END = 0xFF
*/