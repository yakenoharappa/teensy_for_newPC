#include "LineMove.h"

float LineMoveDegd = 0;



void LineMove_setup()
{
    

}

void LineMove_loop()
{
    LineRead_update();
    float linemovedegr2 = DegRangeChange(Angel.Linedegr, -180);
/* 
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
    } */

/* 
    DegRangeChange(radian_deg(Angel.Linedegr) + 180, 180);

    if ( abs(LineMoveDegd - (radian_deg(Angel.old_Linedegr[0]) + 180)) > 180 && (millis() - Angel.old_detect_times[0]) < 500 )
    {
        LineMoveDegd = DegRangeChange(radian_deg(Angel.Linedegr) + 180, 180);
    } */

    
    LineMoveDegd = DegRangeChange(radian_deg(Angel.Linedegr) + 180, 180);
    
    //LineMoveDegd = DegRangeChange(radian_deg(Angel.Linedegr) + 180, 180);

/* 
    if (abs(CameraV.court_deg - LineMoveDegd) > 120)
    {
        LineMoveDegd = DegRangeChange(LineMoveDegd + 180, 180);
    } 
    */
    
}