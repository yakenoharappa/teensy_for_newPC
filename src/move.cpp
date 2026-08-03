#include "move.h"

//============================
// 回り込み半径
//============================
const int N = 9;
const int r = 250.0f;
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
    135,
    -175,
    -130,
    -70,
    0,
    70,
    130,
    175,
    -135
};

// 接線（あとで自由に変更）
float m[N] = {
    1.0,
    1.0,
    1.0,
    1.0,
    1.0,
    1.0,
    1.0,
    1.0,
    1.0
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
    float ball_rad=radians(ball_deg);

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
    float ball_dis = IRv.dis;


    //==========================
    // フェーズ切り替え
    //==========================

    if (abs(ball_deg) < 10.0 && ball_dis < r)
    {
        // ボールを取りに行く
        moveDeg = ball_deg;
    }
    else
    {
        // 回り込み
        moveDeg = getMoveDeg(ball_deg, ball_dis);
    }

    Serial.print("Move Deg : ");
    Serial.println(moveDeg);

    // motorsMove(moveDeg, 80);

    //delay(20);
}