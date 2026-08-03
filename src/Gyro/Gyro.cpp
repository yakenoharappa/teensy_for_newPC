#include "Gyro.h"


float deg_data = 0;

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
    if ( LSM_STATUS == true  )
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
            //deg_data = pastdeg += gz_LSM * dt;
            deg_data = yaw_BNO;
        }
        else
        {
            deg_data = yaw_BNO;
        }
    }
    else
        deg_data = yaw_BNO;

    Serial.print("BNO:");
    Serial.print(yaw_BNO);
    Serial.print(", LSM:");
    Serial.print(theta);
    Serial.print(", degdata:");
    Serial.println(deg_data);
    
    pastdeg = yaw_BNO;
} 
