#include "ATACKER.h"
int first_move = 0;


void ATACKER_setup()
{
    Screen_setup();
    display.println("Gyro_setup");
    display.display();
    Gyro_setup();
    display.println("move_setup");
    display.display();
    move_setup();
    display.println("motors_setup");
    display.display();
    motors_Setup();
    display.println("Kicker_setup");
    display.display();
    Kicker_Setup();
}

void ATACKER_loop()
{
    Gyro_update();
    Camera_update();
    
    IR_loop();
    move_loop();
    
    LineMove_loop();
    
    //Serial.print("CATCH=");
    //Serial.println(digitalRead(Catch_PIN));
/* 
    Serial.print("SIDE");
    Serial.print(Angel.Left);
    Serial.print(", ");
    Serial.print(Angel.Back);
    Serial.print(", ");
    Serial.print(Angel.Right);
    Serial.println(", "); */
    
    /* 
    while (Serial2.available())
    {
        Serial.print("LINEDATA=");
        Serial.println(Serial2.read(), 2);
    } */
    
    if (digitalRead(TOGGLE_PIN) == true)
    {
        motors_Update();
        Screen_Update();
        //first_move = 1;
        //ClearScreen(); //！！！！本来はこっちだよ（全国大会）！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
        Kicker();
    }
    else
    {
        motorsStop();
        Screen_Update();
        Kicker_end();
    }
/* 
    Serial.print("MoveSpeed=");
    Serial.println(MoveSpeed); */
    //delay(100);
}