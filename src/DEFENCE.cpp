#include "DEFENCE.h"
float IR_x = 0;
float IR_y = 0;
float Line_x = 0;
float Line_y = 0;


float Line_Sx = 0;
float Line_Sy = 0;
float ball_X = 0;
float ball_Y = 0;


float Def_X = 0;
float Def_Y = 0;
float MOVE_Deg2 = 0;

Line_States Robo_position = Line_States::NoDetected;
DefenderState DefState = DefenderState::SaveGoal;
const int DefenceDisMAX = 60;

int DefDeg = 0;



void DEFENCE_setup()
{

}


void DEFENCE_loop()
{
    MOVE_Deg2 = 0;

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
    
    Line_Sx = Line_dis * cos(to_Linedegr);
    Line_Sy = Line_dis * sin(to_Linedegr);
    ball_X = cos(deg_radian(ball_deg)) * ball_dis;
    ball_Y = sin(deg_radian(ball_deg)) * ball_dis;
    
    //状態決定
    if (Angel_Need == true)
    {
        /* LineDisの取り扱い
            1. Linedisが一定値以下ならば、動作を行わない
            2. Linedis
        */
        DefState = DefenderState::SaveGoal;
    }
    else if (Save_GoalDis > DefenceDisMAX && Angel_Need == false)
    {
        DefState = DefenderState::ReturnGoal;
    }
    
    
    if (DefState == DefenderState::SaveGoal)
    {
        Def_X = ball_X + Line_Sx * 1.2;
        Def_Y = ball_Y + Line_Sy * 1.2;
        MOVE_Deg2 = atan2(Def_X, Def_Y);
    }
    else if (DefState == DefenderState::ReturnGoal)
    {
        
    }
    
}