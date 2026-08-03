#include "LineMove.h"

float LineMoveDegd = 0;



void LineMove_setup()
{
    

}

void LineMove_loop()
{
    LineRead_update();
    float linemovedegr2 = DegRangeChange(Angel.Linedegr, -180);
    if ((linemovedegr2 > 60 && linemovedegr2 < 120) || (linemovedegr2 > -120 && linemovedegr2 < -60) ) //よこ
    {
        LineMoveDegd = DegRangeChange(radian_deg(Angel.Linedegr) - 90, -180);
    }
    else if (linemovedegr2 > -30 && linemovedegr2 < 30) //ぜんご
    {
        LineMoveDegd = DegRangeChange(radian_deg(Angel.Linedegr) - 270, -180);
    }
    else
    {
        LineMoveDegd = DegRangeChange(radian_deg(Angel.Linedegr) - 180, -180);
    }
}