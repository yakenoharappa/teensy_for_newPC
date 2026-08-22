
#include "motors.hpp"

//motor_convert controller;

//おれは藤城や

/* void setup() {
    
    int TrySetup = 0;
    while (!Serial && TrySetup < 4)
    {
        Serial.begin(115200);
        TrySetup ++;
        delay(100);
    }

    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(PIN_LED1, OUTPUT);
    pinMode(PIN_LED2, OUTPUT);
    pinMode(PIN_LED3, OUTPUT);

    Serial1.begin(115200);
    //motorsInit(&Serial2, 115200);
    Serial2.begin(115200);


    Jyunya_Setup();
    readController_Setup();
    Kicker_Setup();
    //motors_Setup();
    Screen_Setup();


} */

// PIDの計算機実体を1つ作成
PID headingPID(0.6f, 0.6f, 0.6f, 0.6f); 

void motors_Setup()
{
    // シリアル
    Serial.begin(115200); // デバッグ用

    digitalWrite(LED1, HIGH);
    //motorsInit(&Serial1, 115200);          // モーター初期化


    #ifdef SecondRobot
        //3Dプリンタ機体
        motorsSetMoveSign(1, -1, -1, -1);       // 移動のための符号をセット
        motorsSetPdSign(1, -1, -1, -1);             // PID制御のための符号をセット
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

    for (int i = 0; i < 20; i++)
    {
        motorsStop();
        delay(100);
    }
}

void motors_Update()
{

    // この1行で、内部の現在の向きと目標値の計算がすべて更新されます
    if( CamGoalDetected == true && Delection_Mode == true )
    {
        motorsPidProcess(&headingPID, -GoalDeg, 0.0f);
    }
    else
    {
        motorsPidProcess(&headingPID, yaw_BNO, 0.0f );
    }
    

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

    realmovedegd = radian_deg(atan2(realmoveY, realmoveX)); 

/*     if (IRv.deg == 0b1111111111 && LineNeed == false)
    {
        motorsPdMove();
    }
    else
    {
        motorsMove(realmovedegd, MotorSpeed);
    }  */

/*     if (IRv.deg == 0b1111111111)
    {
        motorsPdMove();
    }
    else
    {
        motorsMove(IRv.deg, MotorSpeed);
        //motorsMove(moveDeg, MotorSpeed);
    } */
    
    if (LineNeed == true)
    {
        
        float LineMove_X = cos(deg_radian(LineMoveDegd));
        float LineMove_Y = sin(deg_radian(LineMoveDegd));

        float moveDeg_X = cos(deg_radian(moveDeg));
        float moveDeg_Y = sin(deg_radian(moveDeg));

        float MOVE_Deg2 = atan2(LineMove_Y*1.1 + moveDeg_Y, LineMove_X*1.1 + moveDeg_X);
        if (IRv.detected == true)
        {
            motorsMove(radian_deg(MOVE_Deg2), MotorSpeed);
        }
        else
        {
            motorsMove(LineMoveDegd, MotorSpeed);
        }
        
    }
    else if (IRv.detected == true)
    {
        motorsMove(moveDeg, MotorSpeed);
    }
    else
    {
        motorsPdMove();
        //motorsMove(moveDeg, MotorSpeed);
    }
    
/*     motorsMove(IRv.deg , MotorSpeed);

    Serial.print("realmovedegd=");
    Serial.println(realmovedegd);  */
    
    // Screen_Update();
    
    

   // delay(10); // 制御周期安定化のためのウェイト */
}