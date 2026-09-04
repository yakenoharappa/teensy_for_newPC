#include "Gyro.h"


float deg_data = 0;

int usingGyro = 0;
//#define Gyro_DEBUG
#define BNO 0
#define MIX 1
#define LSM 2

/* void setup()
{
    pinMode(GYRO_RESET, INPUT_PULLDOWN);
    LSM_setup();
}

void loop()
{
    LSM_update();
} */


void Gyro_setup()
{
    //Serial.begin(115200);
    BNO_setup();
    LSM_setup();
}

void Gyro_update()
{
    BNO_update();
    LSM_update();
    if ( LSM_STATUS == true && usingGyro == MIX )
    {
        if ( abs(gz_LSM) > 5.5 && abs(pastdeg - yaw_BNO) <= 0.5 )
        {
            deg_data = theta;
            
        }
        else if ( abs(gz_LSM) > 5.5 &&  abs(pastdeg - yaw_BNO) > 0.5 )
        {
            deg_data = yaw_BNO;
            theta += (yaw_BNO - theta);
        }
        else if ( abs(gz_LSM) < 5.5 &&  abs(pastdeg - yaw_BNO) < 1 )
        {
            //deg_data = pastdeg += gz_LSM * dt_LSM;
            deg_data = yaw_BNO;
        }
        else
        {
            deg_data = yaw_BNO;
        }
    }
    else if (usingGyro == BNO || usingGyro == MIX)
    {
        deg_data = yaw_BNO;
    }
    else 
    {
        deg_data = theta;
    }
        

    #ifdef Gyro_DEBUG
        Serial.print("BNO:");
        Serial.print(yaw_BNO);
        Serial.print(", LSM:");
        Serial.print(theta);
        Serial.print(", degdata:");
        Serial.println(deg_data);
    #endif
    
    pastdeg = yaw_BNO;
} 
