#include "Camera.h"

bool SettingGoal = 0;
bool Delection_Mode = 0;
bool DEForATTA = 1;

/* 変更版
bool SettingGoal = (EEPROM.read(0) >> 1) & 0b01;
bool Delection_Mode = (EEPROM.read(0) >> 2) & 0b01;
bool DEForATTA = (EEPROM.read(0)) & 0b01; 
*/

bool CamBallDetected = 0;
bool CamGoalDetected = 0;
bool SaveGoalDetected = 0;
int GoalDis = 0;
int GoalDeg = 0;
int Save_GoalDis = 0;
int Save_GoalDeg = 0;

bool distanceDevide = false;

int old_GoalDeg[4] = {0};

float GoalY = 0;
float GoalX = 0;
float SaveGoalY = 0;
float SaveGoalX = 0;

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


/*     if(CameraV.orange_dis < 80)
    {
        distanceDevide = true;
    }
    else if(CameraV.orange_dis > 100)
    {
        distanceDevide = false;
    }  */

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
        Save_GoalDis = CameraV.blue_dis;
        Save_GoalDeg = CameraV.blue_deg;
    }
    else
    {
        GoalDis = CameraV.blue_dis;
        GoalDeg = CameraV.blue_deg;
        Save_GoalDis = CameraV.yellow_dis;
        Save_GoalDeg = CameraV.yellow_deg;
    }

    int Look_GoalCheck = 0;
    for (int i = 0; i < 4; i++)
    {
        if (old_GoalDeg[0] != 0xFF)
        {
            
        } 
    }
    

    
    
    if (uint16_t(GoalDis) == 0xFF && uint16_t(GoalDeg) == 0xFF)
    {
        CamGoalDetected = false;
    }
    else
    {
        CamGoalDetected = true;
    }

    if (uint16_t(Save_GoalDeg) == 0xFF && uint16_t(Save_GoalDis) == 0xFF)
    {
        SaveGoalDetected = false;
    }
    else
    {
        SaveGoalDetected = true;
    }
    
    GoalX = sin(deg_radian(GoalDeg + yaw_BNO)) * GoalDis;
    GoalY = cos(deg_radian(GoalDeg + yaw_BNO)) * GoalDis;
    SaveGoalX = sin(deg_radian(Save_GoalDeg + yaw_BNO)) * Save_GoalDis;
    SaveGoalY = cos(deg_radian(Save_GoalDeg + yaw_BNO)) * Save_GoalDis;

    if (old_GoalDeg[0] != GoalDeg)
    {
        for (int i = 3; i > 0; i--)
        {
            old_GoalDeg[i] = old_GoalDeg[i-1];
        }
        old_GoalDeg[0] = GoalDeg;
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