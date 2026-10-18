#include "Camera.h"

bool SettingGoal = 0;
bool Delection_Mode = 0;
bool DEForATTA = 1;

unsigned long Separate_time = 0;
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

const int stable_check_number = 5;
bool stable_check_Goal[stable_check_number] = {false};
bool stable_check_SGoal[stable_check_number] = {false};

bool is_front = false;
bool is_left = false;
float SideX = 0;

bool distanceDevide = false;

int old_GoalDeg[4] = {0};
int old_CourtDeg[4] = {0};

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

/* 
    int Look_GoalCheck = 0;
    for (int i = 0; i < 4; i++)
    {
        if (old_GoalDeg[0] != 0xFF)
        {
            
        } 
    } */
    
    
    
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

    if ((CamGoalDetected + stable_check_Goal[0] + stable_check_Goal[1] + stable_check_Goal[2] + stable_check_Goal[3] + stable_check_Goal[4]) > 3 && CamGoalDetected)
    {
        Serial.print("Goal");
        Serial.print(sin(deg_radian(GoalDeg + yaw_BNO)));
        SideX = sin(deg_radian(GoalDeg + yaw_BNO));
        if (sin(deg_radian(GoalDeg + yaw_BNO)) < 0)
        {
            is_left = false;
        }
        else
        {
            is_left = true;
        }
    }
    else if ((SaveGoalDetected + stable_check_SGoal[0] + stable_check_SGoal[1] + stable_check_SGoal[2] + stable_check_SGoal[3] + stable_check_SGoal[4]) > 3 && SaveGoalDetected)
    {
        Serial.print("Save");
        Serial.print(sin(deg_radian(Save_GoalDeg + yaw_BNO)));
        SideX = sin(deg_radian(Save_GoalDeg + yaw_BNO));
        if (sin(deg_radian(Save_GoalDeg + yaw_BNO)) < 0)
        {
            is_left = false;
        }
        else
        {
            is_left = true;
        }
    }
    else
    {
        Serial.print("Court");
        Serial.print(sin(deg_radian(CameraV.court_deg + yaw_BNO)));
        SideX = sin(deg_radian(CameraV.court_deg + yaw_BNO));
        if (sin(deg_radian(CameraV.court_deg + yaw_BNO)) < 0)
        {
            is_left = false;
        }
        else
        {
            is_left = true;
        }
    }
    
    

    
    
    GoalX = sin(deg_radian(GoalDeg + yaw_BNO)) * GoalDis;
    GoalY = cos(deg_radian(GoalDeg + yaw_BNO)) * GoalDis;
    SaveGoalX = sin(deg_radian(Save_GoalDeg + yaw_BNO)) * Save_GoalDis;
    SaveGoalY = cos(deg_radian(Save_GoalDeg + yaw_BNO)) * Save_GoalDis;

    if ((millis() - Separate_time) > 35)
    {
        for (int i = 3; i > 0; i--)
        {
            old_GoalDeg[i] = old_GoalDeg[i-1];
            old_CourtDeg[i] = old_CourtDeg[i-1];
        }
        old_GoalDeg[0] = GoalDeg;
        old_CourtDeg[0] = CameraV.court_deg;
        Separate_time = millis();
    }
    CameraV.court_deg = (CameraV.court_deg + old_CourtDeg[0] + old_CourtDeg[1] + old_CourtDeg[2]) / 5;
    for (int i = (stable_check_number-1); i > 0; i--)
    {
        stable_check_Goal[i] = stable_check_Goal[i-1];
        stable_check_SGoal[i] = stable_check_SGoal[i-1];
    }
    stable_check_Goal[0] = CamGoalDetected;
    stable_check_SGoal[0] = SaveGoalDetected;
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