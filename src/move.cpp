#include "move.h"

float moveDeg = 0;
float ball_deg = 0;
//float ball_dis = 0;
int GoalDis = 0;
int GoalDeg = 0;
void move_setup()
{
    //Serial.begin(115200);
}


void move_loop()
{
    ball_deg = IRv.deg;
    if (SettingGoal == 1)
    {
        GoalDis = CameraV.yellow_dis;
        GoalDeg = CameraV.yellow_deg;
    }
    else
    {
        GoalDis = CameraV.blue_dis;
        GoalDeg = CameraV.blue_deg;
    }
    
    //MotorSpeed = 80;
    if(ball_deg > 180)
    {
        ball_deg = ball_deg - 360;
    }

if(IRv.dis > 90)
    {
        if (Delection_Mode == true && GoalDis < 80 && abs(ball_deg) < 50 && ball_deg * GoalDeg >= 0) 
        {
            moveDeg = ball_deg;
        }
        else if(abs(ball_deg) < 8 )
        {
            moveDeg = ball_deg;
        }
        else if(ball_deg >= 8)
        {
        if(ball_deg < 20)
        {
            moveDeg = ball_deg + 10;
            //MotorSpeed = 60;
        }
        else if(ball_deg < 30)
        {
            moveDeg = ball_deg + 20;
        }
        else if(ball_deg < 40)
        {
            moveDeg = ball_deg + 25;
        }
        else if(ball_deg < 50)
        {
            moveDeg = ball_deg + 30;
        }
        else if(ball_deg < 60)
        {
            moveDeg = ball_deg + 40;
        }
        else if(ball_deg < 70)
        {
            moveDeg = ball_deg + 55;
        }
        else if(ball_deg < 80)
        {
            moveDeg = ball_deg + 60;
        }
        else if(ball_deg < 90)
        {
            moveDeg = ball_deg + 65;
        }
        else if(ball_deg < 100)
        {
            moveDeg = ball_deg + 70;
        }
        else if(ball_deg < 110)
        {
            moveDeg = ball_deg + 72;
        }
        else if(ball_deg < 120)
        {
            moveDeg = ball_deg + 75;
        }
        else if(ball_deg < 130)
        {
            moveDeg = ball_deg + 77;
        }
        else if(ball_deg < 140)
        {
            moveDeg = ball_deg + 80;
        }
        else if(ball_deg < 150)
        {
            moveDeg = ball_deg + 80;
        }
        else if(ball_deg < 160)
        {
            moveDeg = ball_deg + 80;
        }
        else if(ball_deg < 170)
        {
            moveDeg = ball_deg + 85;
        }
        else if(ball_deg <= 180)
        {
            moveDeg = ball_deg + 90;
        }
        }
        else if(ball_deg <= -8)
        {
        if(ball_deg > -20)
        {
            moveDeg = ball_deg - 10;
            //MotorSpeed = 60;
        }
        else if(ball_deg > -30)
        {
            moveDeg = ball_deg - 20;
        }
        else if(ball_deg > -40)
        {
            moveDeg = ball_deg - 25;
        }
        else if(ball_deg > -50)
        {
            moveDeg = ball_deg - 30;
        }
        else if(ball_deg > -60)
        {
            moveDeg = ball_deg - 40;
        }
        else if(ball_deg > -70)
        {
            moveDeg = ball_deg - 55;
        }
        else if(ball_deg > -80)
        {
            moveDeg = ball_deg - 60;
        }
        else if(ball_deg > -90)
        {
            moveDeg = ball_deg - 65;
        }
        else if(ball_deg > -100)
        {
            moveDeg = ball_deg - 70;
        }
        else if(ball_deg > -110)
        {
            moveDeg = ball_deg - 72;
        }
        else if(ball_deg > -120)
        {
            moveDeg = ball_deg - 75;
        }
        else if(ball_deg > -130)
        {
            moveDeg = ball_deg - 77;
        }
        else if(ball_deg > -140)
        {
            moveDeg = ball_deg - 80;
        }
        else if(ball_deg > -150)
        {
            moveDeg = ball_deg - 80;
        }
        else if(ball_deg > -160)
        {
            moveDeg = ball_deg - 80;
        }
        else if(ball_deg > -170)
        {
            moveDeg = ball_deg - 85;
        }
        else if(ball_deg >= -180)
        {
            moveDeg = ball_deg - 90;
        }
        }
        

        if(moveDeg > 180)
        {
            moveDeg = moveDeg - 360;
        }
        else if(moveDeg < -180)
        {
            moveDeg = moveDeg + 360;
        }
    }
    else
    {
        moveDeg = ball_deg;
    }
        


    
}
/* #include "move.h"

//============================
// 回り込み半径
//============================
const int N = 9;
const int r = 70.0f;
float moveDeg = 0;

float x[N] = {
    -180,
    -135,
    -90,
    -45,
    0,
    45,
    90,
    135,
    180
};

float y[N] = {
    -180,
    -175,
    -130,
    -70,
    0,
    70,
    130,
    175,
    180
};

// 接線（あとで自由に変更）
float m[N] = {
    2.0,
    1.5,
    1.0,
    0.5,
    0.0,
    0.5,
    1.0,
    1.5,
    2.0
};

//================================================
// 三次エルミートスプライン
//================================================
float spline(float input)
{
    if(input <= x[0]) return y[0];
    if(input >= x[N-1]) return y[N-1];

    int i;

    for(i=0;i<N-1;i++)
    {
        if(input >= x[i] && input <= x[i+1])
            break;
    }

    float h = x[i+1]-x[i];
    float t = (input-x[i])/h;

    float h00 = 2*t*t*t - 3*t*t + 1;
    float h10 = t*t*t - 2*t*t + t;
    float h01 = -2*t*t*t + 3*t*t;
    float h11 = t*t*t - t*t;

    return h00*y[i]
         + h10*h*m[i]
         + h01*y[i+1]
         + h11*h*m[i+1];
}

//================================================
// Saphir回り込み
//================================================
float getMoveDeg(float ball_deg,float ball_dis)
{
    float ball_rad = radians(ball_deg);

    if(ball_dis>=r)
    {
        float theta=asin(r/ball_dis);

        if(ball_rad>=0)
            ball_rad+=theta;
        else
            ball_rad-=theta;
    }
    else
    {
        if(ball_rad>=0)
        {
            ball_rad+=PI/2;
        }
        else
        {
            ball_rad-=PI/2;
        }
    }

    float target_deg=degrees(ball_rad);

    // 三次スプライン補間
    return spline(target_deg);
}

void move_setup()
{
    //Serial.begin(115200);
}

void move_loop()
{
    // センサーから取得
    float ball_deg = IRv.deg;
    float ball_dis = 255 - IRv.dis;


    //==========================
    // フェーズ切り替え
    //==========================

    if (abs(ball_deg) < 10.0 )
    {
        // ボールを取りに行く
        moveDeg = ball_deg;
    }
    else if(ball_dis > r)
    {
        // 回り込み
        moveDeg = getMoveDeg(ball_deg, ball_dis);
    }

    Serial.print("Move Deg : ");
    Serial.println(moveDeg);

    // motorsMove(moveDeg, 80);

    //delay(20);
} */

