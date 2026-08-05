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
    
    Serial.print("Linedatas:");
    Serial.print(Linedata.values[0], 2);
    Serial.print(", ");
    Serial.print(Linedata.values[1], 2);
    //Serial.print(", ");
    LineMove_loop();
/*     while (Serial2.available())
    {
        Serial.print("LINEDATA=");
        Serial.println(Serial2.read(), 2);
    } */
    
    if (digitalRead(TOGGLE_PIN) == true)
    {
        motors_Update();
        Screen_Update();
        //first_move = 1;
    }
    else
    {
        motorsStop();
        Screen_Update();
        //ClearScreen(); //！！！！本来はこっちだよ（全国大会）！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
    }
    //delay(100);
    
}