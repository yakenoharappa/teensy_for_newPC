
#include "motors.hpp"

//motor_convert controller;

//おれは藤城や

float PIDk = 1;
float Vx = 0;
float Vy = 0;
int PIDpower = 0;
// PIDの計算機実体を1つ作成
PID headingPID(0.6f, 0.0f, 0.6f, 0.6f); 

void motors_Setup()
{
    // シリアル
    //Serial.begin(115200); // デバッグ用

    //digitalWrite(LED1, HIGH);
    //motorsInit(&Serial1, 115200);          // モーター初期化


    #ifdef SecondRobot
        //3Dプリンタ機体
        motorsSetMoveSign(1, -1, -1, 1);       // 移動のための符号をセット
        motorsSetPdSign(1, -1, -1, 1);             // PID制御のための符号をセット
        motorsSetDegPosition(45, 315, 135, 225); // モータの物理位置をセット
        motorsStop();                            // 停止させておく
    #else
    //機体１のやつ(ジュラルミン)
        motorsSetMoveSign(-1, -1,- 1,- 1);       // 移動のための符号をセット
        motorsSetPdSign(1, 1, 1, 1);             // PID制御のための符号をセット
        motorsSetDegPosition(135, 45,   225, 315); // モータの物理位置をセット
        motorsStop();                            // 停止させておく
    #endif

    /* 機体２のやつ


    */

    // 不感帯の設定（0.5度以内のズレなら微調整を無視する）
    headingPID.setDeadband(0.5f);

    for (int i = 0; i < 1; i++) //i < 20
    {
        motorsStop();
        delay(100);
    }
    headingPID.useI(false);
}

void motors_Update()
{
    // この1行で、内部の現在の向きと目標値の計算がすべて更新されます
/*     if( CamGoalDetected == true && Delection_Mode == true )
    {
        motorsPidProcess(&headingPID, -GoalDeg, 0.0f);
    }
    else
    {
        motorsPidProcess(&headingPID, yaw_BNO, 0.0f );
    } */
    //PIDpower = abs(GoalDeg) * 25 / 90;
    
    if (Delection_Mode == true &&  CamGoalDetected == true  && y > 0 && abs(ball_deg + yaw_BNO) < 120 && (CamBallDetected == true || IRv.detected == true))
    {
        motorsPidProcess(&headingPID, -GoalDeg, 0.0f);
        PIDpower = abs(GoalDeg) * 25 / 75;
    }
    else
    {
        motorsPidProcess(&headingPID, yaw_BNO, 0.0f );
    }
/*     if (Delection_Mode == true && CamGoalDetected == true && abs(ball_deg + yaw_BNO) < 80 && y < 80 && (CamBallDetected == true || IRv.detected == true))
    {
        motorsPidProcess(&headingPID, -GoalDeg, 0.0f);
        //MotorSpeed = speedmix;
        PIDk = 1;
    }
    else
    {
        motorsPidProcess(&headingPID, yaw_BNO, 0.0f );
        PIDk = 1;
    }  */
/*     motorsPdMove();
       // motorsStop();
    } */

/* 
    if ( IRv.deg  == 0b1111111111 )
    {
        motorsPdMove();
    }
    else
    {
        
        if ()
        {
            
        } 
        
        motorsMove(moveDeg, MotorSpeed);
    } 
    */

    //motorsPdMove();!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
    
    float realmoveX = 0;
    float realmoveY = 0;
/* realmovedegd（ライン＋回り込み角度）
    float realmovedegd = 0;

    if (LineNeed == true)
    {
        realmoveX = sin(LineMoveDegd);
        realmoveY = cos(LineMoveDegd);
    }


    if ( IRv.deg != 0b1111111111 )
    {
        realmoveX += sin(moveDeg) + sin(LineMoveDegd);
        realmoveY += cos(moveDeg) + cos(LineMoveDegd);
    }

    realmovedegd = radian_deg(atan2(realmoveY, realmoveX));  */


/*     if (IRv.deg == 0b1111111111 && LineNeed == false)
    {
        motorsPdMove();
    }
    else
    {
        motorsMove(realmovedegd, MotorSpeed);
    }  */

    /*
    if (IRv.deg == 0b1111111111)
    {
        motorsPdMove();
    }
    else
    {
        motorsMove(IRv.deg, MotorSpeed);
        //motorsMove(moveDeg, MotorSpeed);
    } */
    if (CamGoalDetected && GoalY < 62 && GoalY > 1 && abs(GoalX) < 40 && Line_over == false)
    {
        digitalWrite(LED4, HIGH);
        motorsMove((180 - yaw_BNO), MotorSpeed);
    }
    else if (SaveGoalDetected == true && -SaveGoalY > 0 && -SaveGoalY < 55 && abs(SaveGoalX) < 40 && abs(DegRangeChange(CameraV.court_deg + yaw_BNO)) < 45) //-SaveGoalY < 50 && abs(SaveGoalX) < 40 &&
    {
        digitalWrite(LED4, HIGH);
        motorsMove(-(yaw_BNO), MotorSpeed);
    }
    else if (LineNeed == true && Line_over == false)
    {
        digitalWrite(LED4, HIGH);
        float LineMove_X = cos(deg_radian(LineMoveDegd));
        float LineMove_Y = sin(deg_radian(LineMoveDegd));

        float moveDeg_X = cos(deg_radian(ball_deg));
        float moveDeg_Y = sin(deg_radian(ball_deg));

        float MOVE_Deg2 = 0;
        float LineSpeed = constrain(1 - Line_dis, 0.6, 1);
        
        if (Angel_Need == true)
        {
            digitalWrite(LED2, LOW);
            //motorsMove(LineMoveDegd, MotorSpeed); 
            if (IRv.detected == true || CamBallDetected == true)
            {
                if (Line_trace == true)
                {
                    tone(Buzzer_PIN, NOTE_C5, 150);
                    if (trace_X > 0)
                    {
                        realmoveX = min(trace_X, sin(deg_radian(ball_deg)) * 1.2);
                    }
                    else
                    {
                        realmoveX = max(trace_X, sin(deg_radian(ball_deg)) * 1.2);
                    }
                    realmoveY = trace_Y;
                    MOVE_Deg2 = atan2(trace_X, trace_Y); //なんか逆かも
                    motorsMove(radian_deg(MOVE_Deg2), MotorSpeed * LineSpeed);
                    Serial.print("Tmix");
                    Serial.print(radian_deg(MOVE_Deg2));
                    Serial.print("motomoto=");
                    Serial.println(LineMoveDegd);
                }
                else
                {
                    MOVE_Deg2 = atan2(LineMove_Y * 1.2 + moveDeg_Y, LineMove_X * 1.2 + moveDeg_X);
                    motorsMove(radian_deg(MOVE_Deg2), MotorSpeed * LineSpeed);
                    /* 
                    Serial.print("Lmix");
                    Serial.print(radian_deg(MOVE_Deg2));
                    Serial.print("motomoto=");
                    Serial.println(LineMoveDegd); */
                    //motorsMove(LineMoveDegd, MotorSpeed);
                }
            }
            else
            {
                motorsMove(LineMoveDegd, MotorSpeed * LineSpeed);
            } 
        }
        else //if (Goalout == false)
        {
            digitalWrite(LED2, LOW);
            if (abs(DegRangeChange((moveDeg)) - DegRangeChange(CameraV.court_deg)) < 90 && (IRv.detected == true || CamBallDetected == true))
            {
                motorsMove(moveDeg, (150 - MoveSpeed) * 0.5);
            }
            else
            {
                MOVE_Deg2 = atan2(sin(deg_radian(CameraV.court_deg)) * 1.2 + moveDeg_Y, cos(deg_radian(CameraV.court_deg)) * 1.2 + moveDeg_X);  // + yaw_BNO
                motorsMove(radian_deg(MOVE_Deg2), (150 - MoveSpeed) * 0.6);
                /* 
                Serial.print("Smix");
                Serial.print(radian_deg(MOVE_Deg2));
                Serial.print("move=");
                Serial.println(moveDeg);  */
            }
        }
    }
    //       CamGoalDetected == true && GoalY > 0 && GoalY < Goal_over_dis && abs(GoalX) < 40 && abs(DegRangeChange(CameraV.court_deg + yaw_BNO, 180)) > 120
    else if (IRv.detected == true || CamBallDetected == true)
    {
        digitalWrite(LED4, LOW);
        if (Line_over == true && LineNeed == true)
        {
            digitalWrite(LED2, HIGH);
            motorsMove(ball_deg, MotorSpeed * 0.5);
        }
        else
        {
            digitalWrite(LED2, LOW);
            motorsMove(moveDeg, MoveSpeed);
        }
    }
    else
    {
        digitalWrite(LED2, LOW);
        digitalWrite(LED4, LOW);
        motorsPdMove();
        //motorsMove(moveDeg, MotorSpeed);
    }
    
/* 
    motorsMove(IRv.deg , MotorSpeed);
    Serial.print("realmovedegd=");
    Serial.println(realmovedegd);  */
    
    // Screen_Update();
    
    // delay(10); // 制御周期安定化のためのウェイト */
}