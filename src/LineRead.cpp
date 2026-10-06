#include "LineRead.h"
//#define First_Change

//const int HowManyLine = 8;
//bool Line[8] = {0, 0, 0, 0, 0, 0, 0, 0};
//bool Lineport[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
bool LineNeed = 0;
bool Side_Need = false;
bool Angel_Need = false;

const float Linedegs[] = {5.625 *2, 5.625*6, 5.625*10 , 5.625*14, 5.625*18, 5.625*22, 5.625*26, 5.625*30, 5.625*34, 5.625*38, 5.625*42, 5.625*46, 5.625*50, 5.625*54, 5.625*58, 5.625*62};


//Line_dis
float basic_sumX = 0;
float basic_sumY = 0;
int basic_SUM = 0;
float Linedis_X = 0;
float Linedis_Y = 0;
float Line_dis = 0;
float last_Line_dis[16] = {0};

float old_first_deg = 0;
unsigned long old_first_detect_time = 0;
unsigned long last_trace_time = 0;
unsigned long simple_last_time = 0;
int position = 0;


//for Making Blocks
int amount_LineBlock = 0;
int last_amount_LineBlock = 0;
int BlockStart = 20;
int BlockEnd = 20;
int LineBlocks_START[8] = {0};
int LineBlocks_END[8] = {0};
float LineBlocks_DEG[8] = {0};
float LineBlocks_cos[8] = {0};
float LineBlocks_sin[8] = {0};
//int Goal_over_dis = 60;

Line_States Line_state = Line_States::NoDetected;
Line_States Line_state_rel = Line_States::NoDetected;

bool first_detected = 0;
float first_deg = 0;
unsigned long first_detected_time = 0;

unsigned long Side_time = 0;

bool is_reverse = false;
bool is_reverses[16] = {0};
bool is_solved = 0;
bool last_solved[16] = {0};  


//Line_over
int all_same_check = 0;
int lineover_check = 0;
bool Line_over = false;
unsigned long lineover_time = 0;

//Line_trace
bool Line_trace = false;
float trace_X = 0;
float trace_Y = 0;


int amount_no_BlockLine = 0;
int no_BlockLine[16] = {0};

bool SideLineV[3] = {0}; 
Line Angel(16); 

readingSerial Linedata(LineSerial, 0xAA, 0xFF, 3);


Line::Line(int amount) : old_Linedegr(amount), old_detect_times(amount)
{
    for (int i = 0; i < amount; i++)
    {
        Line::degs[i] = Linedegs[i];
    } 
    Line::HowManyLine = amount;
}


void lineover_move()
{
    if ((millis() - lineover_time) < 100 && GoalY > 58 && abs(GoalDeg) < 8 )
    {
        Line_over = true;
    }
    else
    {
        Line_over = false;
        //LineNeed = true;
    }
}

void Line_trace_check()
{
    
    Serial.print("Trace?=");
    Serial.print(Line_trace);
    Serial.print(", OVER?=");
    Serial.print(Line_over);
/* 
    Serial.print("sumX:");
    Serial.print(Angel.sumX);
    Serial.print(", sumY:");
    Serial.println(Angel.sumY);
    Serial.print("LinneX:"); */

    Linedis_X = basic_SUM ? basic_sumY / basic_SUM : 0;
    Linedis_Y = basic_SUM ? basic_sumX / basic_SUM : 0;
    Line_dis = sqrt(Linedis_X * Linedis_X + Linedis_Y * Linedis_Y);
    
    if ( (amount_no_BlockLine - amount_LineBlock) != 0)
    {
        //Linedis_X = amount_no_BlockLine ? Angel.sumY / (amount_no_BlockLine) * (amount_no_BlockLine - amount_LineBlock + 1) : 0;   //(amount / no) 
        //Linedis_Y = amount_no_BlockLine ? Angel.sumX / (amount_no_BlockLine) * (amount_no_BlockLine - amount_LineBlock + 1) : 0;

    }
    else
    {
        //Linedis_X = amount_no_BlockLine ? Angel.sumY / (amount_no_BlockLine) : 0;
        //Linedis_Y = amount_no_BlockLine ? Angel.sumX / (amount_no_BlockLine) : 0;

    }



/* 
    Serial.print("amount=");
    Serial.print(amount_no_BlockLine);
    Serial.print(", sa=");
    Serial.println((amount_no_BlockLine - amount_LineBlock)); */

    if (DegRangeChange(radian_deg(Angel.old_Linedegr[0]) + yaw_BNO, 180) >= 0)
    {
        trace_X = (Linedis_X - (0.2)) * 1;
    }
    else
    {
        trace_X = (Linedis_X + (0.2)) * 1;
    }
    trace_Y = 1;
    Serial.print("ball_X=");
    Serial.print(sin(deg_radian(ball_deg)));
    Serial.print(", Trace_X=");
    Serial.println(trace_X);
    
    //Serial.print((fabs(Linedis_X) / (amount_no_BlockLine)));
    if (Line_state == Line_States::SIDE)
    {
        if (abs(DegRangeChange(radian_deg(Angel.old_Linedegr[0]) + yaw_BNO, -180) - ball_deg) < 110 && Line_trace == true && abs(DegRangeChange(radian_deg(Angel.old_Linedegr[0]) + yaw_BNO, 180)) > abs(DegRangeChange(ball_deg, 180)) && abs(DegRangeChange(CameraV.court_deg + yaw_BNO, 180)) < 99 ) //&& (sin(Angel.old_Linedegr[0]) * sin(deg_radian(ball_deg))) >= 0
        {
            //trace_X = (1 - fabs(Linedis_X));
            //trace_X = trace_X;
            //Serial.print("TRACE=");
            //Serial.println(radian_deg(atan2(trace_Y, trace_X)));
            Line_trace = true;  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            last_trace_time = millis();
        }
        else if ((millis() - last_trace_time) < 300 && Line_state == Line_States::SIDE && abs(DegRangeChange(CameraV.court_deg + yaw_BNO, 180)) < 99)
        {
            Line_trace = true; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        }
        else
        {
            Line_trace = false;
        }
    }
    else
    {
        Line_trace = false;
    }
}

void Line_trace_move()
{

}



void MakingBlock()
{
    amount_no_BlockLine = 0;
    amount_LineBlock = 0;
    BlockStart = 20;
    BlockEnd = 20;
    basic_sumX = 0;
    basic_sumY = 0;
    basic_SUM = 0;
    for (int  i = 0; i < (Angel.HowManyLine); i++)
    {
        if ( i < (Angel.HowManyLine-1) && Angel.Detect[i] == true && Angel.Detect[i+1] == true)
        {
            if (i > 0 && Angel.Detect[i] == true && Angel.Detect[i-1] == false)
            {
                BlockStart = i;
                
                amount_LineBlock++;
            }
            else if (i == 0 && Angel.Detect[0] == true && Angel.Detect[15] == false)
            {
                BlockStart = i;
                amount_LineBlock++;
            }

            BlockEnd = (i+1)%16;
        }
        else if (i == (Angel.HowManyLine-1) && Angel.Detect[i] == true && Angel.Detect[0] == true)
        {
            if (i > 0 && Angel.Detect[i] == true && Angel.Detect[i-1] == false)
            {
                BlockStart = i;
                
                amount_LineBlock++;
            }
            BlockEnd = (i+1)%16;
        }
        else    //no_block_line
        {
            if (Angel.Detect[i] == true)
            {
                amount_no_BlockLine++;
                no_BlockLine[amount_no_BlockLine] = i;
            }
            Angel.sumX += Angel.Detect[i] * cos(deg_radian(Angel.degs[i])); // - deg_data
            Angel.sumY += Angel.Detect[i] * sin(deg_radian(Angel.degs[i])); // - deg_data
        }

        if (amount_LineBlock > 0)
        {
            LineBlocks_START[amount_LineBlock - 1] = (BlockStart);
            LineBlocks_END[amount_LineBlock - 1] = (BlockEnd);
        }
        else
        {
            LineBlocks_START[0] = BlockStart;
            LineBlocks_END[0] = BlockEnd;
        }

        last_amount_LineBlock = amount_LineBlock;
        basic_sumX += Angel.Detect[i] * cos(deg_radian(Angel.degs[i]  + yaw_BNO));
        basic_sumY += Angel.Detect[i] * sin(deg_radian(Angel.degs[i] + yaw_BNO));
        basic_SUM += Angel.Detect[i];
    }
}


void LineRead_Setup()
{
    
}


void LineRead_update()
{
    Angel.sumX = 0;
    Angel.sumY = 0;
    lineover_check = 0;
    amount_LineBlock = 0;
    amount_no_BlockLine = 0;
    all_same_check = 0;

    LineNeed = false;
    Angel_Need = false;
    Side_Need = false;
    //for Side Lines
    for (int i = 0; i < 3; i++)
    {
        SideLineV[i] = false;
    }
    Angel.Left = false;
    Angel.Right = false;
    Angel.Back = false;
    
    Linedata.readData();
    Angel.number_of_detect = 0;
    

    //SideLine_Read
    for (int i = 0; i < 3; i++)
    {
        SideLineV[i] = Linedata.values[2] >> i & 0b01;
    }
    Angel.Right = SideLineV[2];
    Angel.Back = SideLineV[1];
    Angel.Left = SideLineV[0];
    Side_Need = Angel.Left || Angel.Right || Angel.Back;
    if (Side_Need == true)
    {
        Side_time = millis();
    }
    else if ((millis() - Side_time) <= 75)
    {
        Side_Need = true;
    }

    
    //LineNeed = Side_Need;
    
    for (int i = 0; i < 16; i++)
    {
        Angel.Detect[i] = (Linedata.values[0] | (Linedata.values[1] << 8)) >> i & 0b01;
        if (Angel.Detect[i] == true)
        {
            Angel.number_of_detect++;
            //LineNeed = true;
            Angel_Need = true;
            //Angel.sumX +=  cos(deg_radian(Angel.degs[i] - deg_data));
            //Angel.sumY +=  sin(deg_radian(Angel.degs[i] - deg_data));
            Angel.Linedegr = deg_radian(Angel.degs[i]);
        }
    }

    LineNeed = Angel_Need || Side_Need;

    MakingBlock();
    int hairetsu[8] = {0};  //各blockでの含むラインの数を表すもの。（A2~A4→3）
    
    for (int i = 0; i < amount_LineBlock; i++)
    {
        if (LineBlocks_END[i] < LineBlocks_START[i] && LineBlocks_START[0] != 20 && LineBlocks_END[0] != 20)
        {
            hairetsu[i] = ((16 - LineBlocks_START[i]) + LineBlocks_END[i] + 1)%16;
        }
        else if (LineBlocks_START[0] != 20 && LineBlocks_END[0] != 20)
        {
            hairetsu[i] = abs(LineBlocks_END[i] - LineBlocks_START[i] + 1)%16;
        }
        LineBlocks_DEG[i] = 0;
        LineBlocks_sin[i] = 0;
        LineBlocks_cos[i] = 0;
        for (int j = 0; j < hairetsu[i] ; j++)
        {
            LineBlocks_cos[i] += (cos(deg_radian(Angel.degs[(LineBlocks_START[i] + j)%16]))); //- deg_data
            LineBlocks_sin[i] += (sin(deg_radian(Angel.degs[(LineBlocks_START[i] + j)%16]))); //- deg_data
            LineBlocks_DEG[i] += DegRangeChange(Angel.degs[(LineBlocks_START[i] + j)%16], -180);
        }
        LineBlocks_cos[i] /= hairetsu[i];
        LineBlocks_sin[i] /= hairetsu[i];
        LineBlocks_DEG[i] /= hairetsu[i];
        Angel.sumX += LineBlocks_cos[i];
        Angel.sumY += LineBlocks_sin[i];
    }

    float AngelAtan = atan2(Angel.sumY, Angel.sumX);
    
/* 
    for (int i = 0; i < 3; i++)
    {
        SideLineV[i] = Linedata.values[2] >> i & 0b01;
    }
    Angel.Right = SideLineV[0];
    Angel.Back = SideLineV[1];
    Angel.Left = SideLineV[2]; */


    if (LineNeed == true)
    {
        Angel.last_detect_time = millis();
        if ( Angel.number_of_detect == 1 )
        {
            
        }
        else    // else if ( Angel.number_of_detect >= 2 )
        {
            Angel.Linedegr = AngelAtan;
            if ((amount_no_BlockLine) == 2 && abs(int(Angel.Linedegr)) < 10 && abs(int(Line_dis)) <= 0.33 ) //abs(int(Angel.sumX)) == 0 && abs(int(Angel.sumY)) == 0 \___________\ && (Angel.last_detect_time - Angel.old_detect_times[0]) < 500
            {
                //Angel.Linedegr = first_deg; //= Angel.old_Linedegr[0];
                if (Delection_Mode == true)
                {
                    Angel.Linedegr = deg_radian(CameraV.court_deg - 180);
                }
                else
                {
                    Angel.Linedegr = Angel.old_Linedegr[0];
                }
                
                
                //Serial.print("check:");
                //Serial.println(CameraV.court_deg - CameraV.blue_deg);
            }
            else
            {
                Angel.Linedegr = AngelAtan;
            }
        }

        #ifdef First_Change
        if ((Angel.last_detect_time - simple_last_time) > 375 || (abs(Angel.Linedegr - Angel.old_Linedegr[0]) > 45 && abs(Angel.Linedegr - Angel.old_Linedegr[0]) < 120) ) //かえたよ2026/08/24_21:30abs(Angel.Linedegr - Angel.old_Linedegr[0]) < 45 ||
        {
            if (first_detected == false)
            {
                first_deg = Angel.Linedegr;
                first_detected_time = millis();
                first_detected = true;
            }
            
        }
        else    //つまり、<= 1000 のときは、firstDetectedでない。
        {
            first_detected = false;
        }
        #else
        if (first_detected == false && (Angel.last_detect_time - simple_last_time) > 30)
        {
            first_deg = Angel.Linedegr;
            first_detected_time = millis();
            first_detected = true;
        }
        else
        {
            first_detected = false;
        }
        #endif
        
        ////-----------------------------------------------------------------------------
        //if ((Angel.last_detect_time - first_detected_time) > 500)
        //{
            for (int i = 0; i < Angel.HowManyLine; i++)
            {
                if (Angel.old_detect_times[i] > first_detected_time && Angel.last_detect_time > Angel.old_detect_times[i])
                {
                    lineover_check++;
                }
            }
            lineover_check = constrain(lineover_check, 0, 6);
            
        //}
        
        if (lineover_check > 0)
        {
            
            for (int i = 0; i < lineover_check; i++)
            {
                /* 
                Serial.print("old");
                Serial.print(radian_deg(Angel.old_Linedegr[i]));
                Serial.print(", F=");
                Serial.print(radian_deg(first_deg));
                Serial.print("ALLSAME="); 
                Serial.println((radian_deg(Angel.old_Linedegr[i]) - radian_deg(first_deg)));
                */
                if (abs(DegRangeChange(radian_deg(Angel.old_Linedegr[i] - old_first_deg), 180)) < 40) //もし後ろでトレースをしたくなったときは、条件文を追加すること
                {
                    all_same_check++;
                    /* 
                    if ( abs(DegRangeChange(radian_deg(Angel.old_Linedegr[i]), 180)) < 35 && abs(DegRangeChange(radian_deg(Angel.Linedegr), 180)) < 35 ) //!!!!degdataひくべきかも
                    {
                        all_same_check++;
                    } */
                }
            }

            if (all_same_check >= lineover_check && all_same_check > 3)
            {
                Line_trace = true;
            }
            else
            {
                Line_trace = false;
            }
            
            if ( all_same_check == lineover_check && GoalY < Goal_over_dis && Delection_Mode == true && GoalY > 54 && abs(GoalDeg - yaw_BNO) < 8) //----------------------------------------------------------------------------------------------
            {
                if (Line_over == false)
                {
                    lineover_time = millis();
                }
                Line_over = true;
            }
            else
            {
                Line_over = false;
            }
        }
        else
        {
            Line_over = false;
            Line_trace = false;
        }
        //-------------------------------------------------------------------------------------------------------------
    }
    else
    {
        first_detected = false;
        Line_over = false;
    }
    
    switch (amount_no_BlockLine)
    {
    case 0:
        Line_state = Line_States::NoDetected;
        break;

    case 1:
        if (abs(radian_deg(Angel.Linedegr + yaw_BNO)) > 45 && abs(radian_deg(Angel.Linedegr + yaw_BNO)) < 140)
        {
            Line_state = Line_States::SIDE;
            
        }
        else
        {
            Line_state = Line_States::FRONTorBACK;
        }
        break;

    case 2:
        if (abs(radian_deg(Angel.Linedegr) + yaw_BNO) > 45 && abs(radian_deg(Angel.Linedegr) + yaw_BNO) < 140)
        {
            Line_state = Line_States::SIDE;
        }
        else
        {
            Line_state = Line_States::FRONTorBACK;
        }
        break;

    case 3:
        Line_state = Line_States::CORNER;
        break;

    case 4:
        Line_state = Line_States::CORNER;
        break;

    default:
        Line_state = Line_States::NoDetected;
        break;
    }

switch (amount_no_BlockLine)
    {
    case 0:
        Line_state_rel = Line_States::NoDetected;
        break;

    case 1:
        if (abs(radian_deg(Angel.Linedegr)) < 45 || abs(radian_deg(Angel.Linedegr)) > 140)
        {
            Line_state_rel = Line_States::FRONTorBACK;
        }
        else
        {
            Line_state_rel = Line_States::SIDE;
        }
        break;

    case 2:
        if (abs(radian_deg(Angel.Linedegr)) < 45 || abs(radian_deg(Angel.Linedegr)) > 140)
        {
            Line_state_rel = Line_States::FRONTorBACK;
        }
        else
        {
            Line_state_rel = Line_States::SIDE;
        }
        break;

    case 3:
        Line_state_rel = Line_States::CORNER;
        break;

    case 4:
        Line_state_rel = Line_States::CORNER;
        break;

    default:
        Line_state_rel = Line_States::NoDetected;
        break;
    }


/* 
    Serial.print("amount");
    Serial.print(amount_no_BlockLine);
    Serial.print(", LineState=");
    Serial.println(int(Line_state)); */

    Line_trace_check();
/* 
    for (int i = 0; i < Linedata.amountData; i++)
    {
        //Linedata.values[i] = analogRead(Angel.port[i]);
        Angel.sumX = Linedata.values[i] * cos((Angel.degs[i]) * PI / 180);
        Angel.sumY = Linedata.values[i] * sin((Angel.degs[i]) * PI / 180);
    } */
    //Angel.Linedegr = deg_radian(BitChange(Linedata.values[0], Linedata.values[1]));

    //DegRangeChange(radian_deg(Angel.Linedegr) + 180, 180);
    
/* 
    Serial.print("Scos=");
    Serial.print(cos(Angel.Linedegr) * cos(deg_radian(CameraV.court_deg - 180)));
    Serial.print("Ssin=");
    Serial.println(sin(Angel.Linedegr) * sin(deg_radian(CameraV.court_deg - 180))); */

/*     if (Line_state == Line_States::FRONTorBACK)
    {
        if (cos(Angel.Linedegr) * cos(deg_radian(CameraV.court_deg - 180)) < 0)
        {
            is_reverse = true;
        }
        else
        {
            is_reverse = false;
        }
    }
    else if (Line_state == Line_States::SIDE)
    {
        if (sin(Angel.Linedegr) * sin(deg_radian(CameraV.court_deg - 180)) < 0)
        {
            is_reverse = true;
        }
        else
        {
            is_reverse = false;
        }
    }
    else
    {
        is_reverse = false;
    } */
    
    /* 
    Serial.print("cos");
    Serial.print(cos(deg_radian(GoalDeg)));
    Serial.print("GoalDeg=");
    Serial.print(GoalDeg);
    Serial.print("GoalDis=");
    Serial.print(GoalDis);
    */
/* 
    Serial.print(", GoalX=");
    Serial.print(GoalX);
    Serial.print(",SaX=");
    Serial.print(SaveGoalX);
    Serial.print(", GoalY=");
    Serial.print(GoalY);
    Serial.print(",SaY=");
    Serial.print(-SaveGoalY);
    Serial.print("Lineover=");
    Serial.print(lineover_check);
    Serial.print(", allsame=");
    Serial.print(all_same_check); 
    Serial.print(", Line_dis");
    Serial.println(Line_dis);
 */
    int reversed_check = abs(DegRangeChange(radian_deg(Angel.Linedegr), 180) - radian_deg(Angel.old_Linedegr[0]));
    int reversed_CAM = abs(DegRangeChange(radian_deg(Angel.Linedegr), 180) - DegRangeChange(CameraV.court_deg - 180, 180));
    //int reversedf_CAM = abs(DegRangeChange(radian_deg(first_deg), -180) - DegRangeChange(CameraV.court_deg - 180, -180));
    position = 0;

/* 
    if ((Line_dis - last_Line_dis[0]) > 0.1 && first_detected == false)
    {
        is_reverse = true;
    }
    else
    {
        is_reverse = false;
    } */
    if (Delection_Mode == true)
    {
        if (reversed_CAM > 155 && reversed_CAM < 270)
        {
            is_reverse = true;
        }
        else
        {
            is_reverse = false;
        }
    }
    else
    {
        if (reversed_check > 155 && reversed_check < 270 ) //&& is_reverses[0] == false
        {
            is_reverse = true;
        }
        else
        {
            is_reverse = false;
        }
    }
    

    if (CamGoalDetected == true && GoalY > 0 && GoalY < Goal_over_dis && abs(GoalX) < 50 && abs(CameraV.court_deg + yaw_BNO) > 160) //定数は仮！！！(CamGoalDetected == true && GoalDeg < 30 && GoalY < Goal_over_dis) // 0 - 140 - 140
    {
        Angel.Linedegr = deg_radian(GoalDeg);
        position = 1;
    }
    else if (SaveGoalDetected == true && -SaveGoalY > 0 && -SaveGoalY < 50 && abs(GoalX) < 40 && abs(DegRangeChange(CameraV.court_deg + yaw_BNO, 180)) < 30)
    {
        Angel.Linedegr = deg_radian(Save_GoalDeg);
        position = -1;
    }
    else if (Line_state == Line_States::CORNER && Delection_Mode == true)
    {
        if (reversed_CAM > 155 && reversed_CAM < 270 && Delection_Mode == true)
        {
            position = 2;
            Angel.Linedegr = deg_radian(CameraV.court_deg - 180);
            //Angel.Linedegr = deg_radian(radian_deg(Angel.Linedegr) - 180);
        }
    }
    /* 
    else if ( (millis() - first_detected_time) < 500 && first_detected == false && is_reverse ) //もと45(reversed_check < 55 || reversed_check > 150)
    {
        position = 3;
        Angel.Linedegr = first_deg;
        //Angel.Linedegr = deg_radian(CameraV.court_deg - 180);
    }  */
    else if ( is_reverse )
    {
        if (Delection_Mode == 1)
        {
            position = 4;
            Angel.Linedegr = deg_radian(CameraV.court_deg - 180);
        }
        else
        {
            position = 5;
            /* 
            if (is_reverses[0] == false)
            { */
                Angel.Linedegr = deg_radian(DegRangeChange(radian_deg(Angel.Linedegr) - 180, 180));
            //}
        }
    } 


/* 
    if (Line_dis < 0.4)
    {
        Angel.Linedegr = deg_radian(CameraV.court_deg - 180);
    }  */
    
    
    if (Line_over == true)
    {
        digitalWrite(LED1, HIGH);
        lineover_move();
    }
    else
    {
        digitalWrite(LED1, LOW);
    }

    Serial.println();
    if (Angel.old_Linedegr[0] != Angel.Linedegr)
    {
        for (int i = (Angel.HowManyLine - 1) ; i > 0 ; i--)
        {
            //float temp = Angel.old_Linedegr[i];
            //Angel.old_Linedegr[i+1] = temp;
            Angel.old_Linedegr[i] = Angel.old_Linedegr[i-1];
            Angel.old_detect_times[i] = Angel.old_detect_times[i-1];
            is_reverses[i] = is_reverses[i-1];
        }
        Angel.old_Linedegr[0] = Angel.Linedegr;
        Angel.old_detect_times[0] = Angel.last_detect_time;
        is_reverses[0] = is_reverse;
    }

    if (Line_dis != last_Line_dis[0])
    {
        for (int i = (Angel.HowManyLine - 1) ; i > 0 ; i--)
        {
            last_Line_dis[i] = last_Line_dis[i-1];
        }
        last_Line_dis[0] = Line_dis;
        Serial.print("disp");
        Serial.println(last_Line_dis[0]);
        
    }
    

    if (first_deg != old_first_deg)
    {
        old_first_deg = first_deg;
        old_first_detect_time = first_detected_time;
    }
    if (LineNeed)
    {
        simple_last_time = Angel.last_detect_time;
    }
    
}