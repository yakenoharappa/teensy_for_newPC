#include "DEFENCE.h"
float IR_x = 0;
float IR_y = 0;
float Line_x = 0;
float Line_y = 0;

void DEFENCE_setup()
{

}


void DEFENCE_loop()
{
    if (CamBallDetected == true)
    {
        ball_deg = CameraV.orange_deg;
        ball_dis = CameraV.orange_dis;
    }
    else
    {
        ball_deg = IRv.deg;
        ball_dis = IRv.dis;
    }
}