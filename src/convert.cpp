#include "convert.h"

float deg_radian(int deg){
    float radian = deg * M_PI / 180;
    return radian;
}



//radian → deg(度数法)
int radian_deg(float radian){
    int deg = radian * 180 / M_PI;
    return deg;
}

//基準方向変換
int changedeg(int value, int change_range){
    value += change_range;
    while (value > 180)
    {
        value -= 360;
    }
    while (value < -180)
    {
        value += 360;
    }
    return value;
}

//角度範囲変換
int DegRangeChange(int before, int which){
    if (which == 360)
    {
        while (before > 360)
        {
            before -= 360;
        }
        while (before < 0)
        {
            before += 360;
        }
    }
    else
    {
        while (before > 180)
        {
            before -= 360;
        }
        while (before < -180)
        {
            before += 360;
        }
    }
    return before;
}


//8bit→16bit(Serial関係)__v1=LOWB,v2=HIGHB
uint16_t BitChange(uint8_t v1, uint8_t v2)
{
    //v2 = v2  << 8; 
    uint16_t v3 = v1 | (v2 << 8);
    return v3;
}
