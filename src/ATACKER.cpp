#include "move.h"
#include "LineMove.h"
#include "motors.hpp"
#include "Gyro/Gyro.h"
#include "Camera.h"
#include "Screen.h"
#include "PINOUTs.h"
#include "LineRead.h"
int first_move = 0;


void ATACKER_setup()
{
    Screen_setup();
    Gyro_setup();
    move_setup();
    motors_Setup();
}

void ATACKER_loop()
{
    Gyro_update();
    Camera_update();
    //Serial.print("Camera");
    //Serial.println(CameraV.orange_deg);
    
    IR_loop();
    Serial.print("IRV=");
    Serial.println(IRv.deg);
    move_loop();
    LineMove_loop();
    Serial.print("Linedatas:");
    Serial.print(Linedata.values[0], 2);
    Serial.print(", ");
    Serial.print(Linedata.values[1], 2);
    //Serial.print(", ");

    if (digitalRead(TOGGLE_PIN) == true)
    {
/*         if (first_move == 0)
        {
            while (LineNeed == false)
            {
                motorsMove(0, MotorSpeed);
            }
        }
        else
        {
            motors_Update();
        } */
        motors_Update();
        //first_move = 1;
    }
    else
    {
        motorsStop();
    }
    
    Screen_Update();
    
}