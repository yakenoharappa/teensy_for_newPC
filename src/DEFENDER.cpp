#include "DEFENDER.h"

void DEFENDER_setup()
{
    Screen_setup();
    display.println("Gyro_setup");
    Gyro_setup();
    display.println("move_setup");
    move_setup();
    display.println("motors_setup");
    motors_Setup();
    display.println("Kicker_setup");
    Kicker_Setup();
}

void DEFENDER_loop()
{
    Gyro_update();
    Camera_update();
    
    IR_loop();
    //move_loop();
    LineMove_loop();

    DEFENCE_loop();
    
    
    
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
    //delay(100);
}