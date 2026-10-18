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
    
    
    if (digitalRead(TOGGLE_PIN) == true)
    {
        motors_Update();
        //Screen_Update();
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
}