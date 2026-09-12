#include "LineRead.h"

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

float old_first_deg = 0;
unsigned long old_first_detect_time = 0;


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

bool first_detected = 0;
float first_deg = 0;
unsigned long first_detected_time = 0;

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

    Linedis_X = basic_sumY / basic_SUM;
    Linedis_Y = basic_sumX / basic_SUM;
    
    if ( (amount_no_BlockLine - amount_LineBlock) != 0)
    {
        //Linedis_X = amount_no_BlockLine ? Angel.sumY / (amount_no_BlockLine) * (amount_no_BlockLine - amount_LineBlock + 1) : 0;   //(amount / no) 
        //Linedis_Y = amount_no_BlockLine ? Angel.sumX / (amount_no_BlockLine) * (amount_no_BlockLine - amount_LineBlock + 1) : 0;
        Serial.print(Linedis_X);
        Serial.print(", Y=");
        Serial.print(Linedis_Y);
    }
    else
    {
        //Linedis_X = amount_no_BlockLine ? Angel.sumY / (amount_no_BlockLine) : 0;
        //Linedis_Y = amount_no_BlockLine ? Angel.sumX / (amount_no_BlockLine) : 0;
        Serial.print(Linedis_X);
        Serial.print(", Y=");
        Serial.print(Linedis_Y);
    }



    Serial.print("amount=");
    Serial.print(amount_no_BlockLine);
    Serial.print(", sa=");
    Serial.println((amount_no_BlockLine - amount_LineBlock));
    
    //Serial.print((fabs(Linedis_X) / (amount_no_BlockLine)));
    if (Line_state == Line_States::SIDE)
    {
        if (abs(radian_deg(Angel.Linedegr) - ball_deg) < 110 && Line_trace == true && sin(Angel.Linedegr) * sin(deg_radian(ball_deg)) > 0)
        {
            //trace_X = (1 - fabs(Linedis_X));
            if (DegRangeChange(radian_deg(Angel.old_Linedegr[0]), 180) >= 0)
            {
                trace_X = Linedis_X - (0.5);
            }
            else
            {
                trace_X = Linedis_X + (0.5);
            }
            
            trace_Y = 1;
            //Serial.print("TRACE=");
            //Serial.println(radian_deg(atan2(trace_Y, trace_X)));
            Line_trace = true;
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
        basic_sumX += Angel.Detect[i] * cos(deg_radian(Angel.degs[i]));
        basic_sumY += Angel.Detect[i] * sin(deg_radian(Angel.degs[i]));
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

    //Goal_Y
    //移送済み

    //SideLine_Read
    for (int i = 0; i < 3; i++)
    {
        SideLineV[i] = Linedata.values[2] >> i & 0b01;
    }
    Angel.Right = SideLineV[2];
    Angel.Back = SideLineV[1];
    Angel.Left = SideLineV[0];
    Side_Need = Angel.Left || Angel.Right || Angel.Back;
    //LineNeed = Side_Need;
    
    for (int i = 0; i < 16; i++)
    {
        Angel.Detect[i] = (Linedata.values[0] | (Linedata.values[1] << 8)) >> i & 0b01;
        if (Angel.Detect[i] == true)
        {
            Angel.number_of_detect++;
            LineNeed = true;
            Angel_Need = true;
            //Angel.sumX +=  cos(deg_radian(Angel.degs[i] - deg_data));
            //Angel.sumY +=  sin(deg_radian(Angel.degs[i] - deg_data));
            Angel.Linedegr = deg_radian(Angel.degs[i]);
        }
    }

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
            if ((amount_no_BlockLine) == 2 && abs(int(Angel.Linedegr)) == 0 && abs(int(Angel.sumX)) == 0 && abs(int(Angel.sumY)) == 0 && (Angel.last_detect_time - Angel.old_detect_times[0]) < 500 )
            {
                //Angel.Linedegr = first_deg; //= Angel.old_Linedegr[0];
                Angel.Linedegr = deg_radian(CameraV.court_deg - 180);
                Serial.print("check:");
                Serial.println(CameraV.court_deg - CameraV.blue_deg);
            }
            else
            {
                Angel.Linedegr = AngelAtan;
            }
        }

        if ((Angel.last_detect_time - Angel.old_detect_times[0]) > 375 || abs(Angel.Linedegr - Angel.old_Linedegr[0]) < 45 || (abs(Angel.Linedegr - Angel.old_Linedegr[0]) > 45 && abs(Angel.Linedegr - Angel.old_Linedegr[0]) < 120) ) //かえたよ2026/08/24_21:30
        {
            if (first_detected == false)
            {
                first_detected = true;
                first_detected_time = millis();
                first_deg = Angel.Linedegr;
            }
        }
        else    //つまり、<= 1000 のときは、firstDetectedでない。
        {
            first_detected = false;
        }
        
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
            lineover_check = constrain(lineover_check, 0, 5);
            
        //}
        
        if (lineover_check > 1)
        {
            
            for (int i = 0; i < lineover_check; i++)
            {
                Serial.print("old");
                Serial.print(radian_deg(Angel.old_Linedegr[i]));
                Serial.print(", F=");
                Serial.print(radian_deg(first_deg));
                Serial.print("ALLSAME=");
                Serial.println((radian_deg(Angel.old_Linedegr[i]) - radian_deg(first_deg)));
                if (abs(DegRangeChange(radian_deg(Angel.old_Linedegr[i] - old_first_deg), 180)) < 35) //もし後ろでトレースをしたくなったときは、条件文を追加すること
                {
                    all_same_check++;
                    /* 
                    if ( abs(DegRangeChange(radian_deg(Angel.old_Linedegr[i]), 180)) < 35 && abs(DegRangeChange(radian_deg(Angel.Linedegr), 180)) < 35 ) //!!!!degdataひくべきかも
                    {
                        all_same_check++;
                    } */
                }
            }

            if (all_same_check == lineover_check && all_same_check > 2)
            {
                Line_trace = true;
            }
            else
            {
                Line_trace = false;
            }
            
            if ( all_same_check == lineover_check && GoalY < Goal_over_dis && Delection_Mode == true && GoalY > 56 && abs(GoalDeg - yaw_BNO) < 8) //----------------------------------------------------------------------------------------------
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
        if (abs(radian_deg(Angel.Linedegr)) < 45 || abs(radian_deg(Angel.Linedegr)) > 140)
        {
            Line_state = Line_States::FRONTorBACK;
        }
        else
        {
            Line_state = Line_States::SIDE;
        }
        break;

    case 2:
        if (abs(radian_deg(Angel.Linedegr)) < 45 || abs(radian_deg(Angel.Linedegr)) > 140)
        {
            Line_state = Line_States::FRONTorBACK;
        }
        else
        {
            Line_state = Line_States::SIDE;
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

    int reversed_check = abs(DegRangeChange(radian_deg(Angel.Linedegr), 180) - radian_deg(first_deg));
    int reversed_CAM = abs(DegRangeChange(radian_deg(Angel.Linedegr), 180) - DegRangeChange(CameraV.court_deg - 180, 180));
    int reversedf_CAM = abs(DegRangeChange(radian_deg(first_deg), -180) - DegRangeChange(CameraV.court_deg - 180, -180));
    bool is_reverse = false;
    Serial.print("Scos=");
    Serial.print(cos(Angel.Linedegr) * cos(deg_radian(CameraV.court_deg - 180)));
    Serial.print("Ssin=");
    Serial.println(sin(Angel.Linedegr) * sin(deg_radian(CameraV.court_deg - 180)));

    if (Line_state == Line_States::FRONTorBACK)
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
    }


    
    /* 
    Serial.print("cos");
    Serial.print(cos(deg_radian(GoalDeg)));
    Serial.print("GoalDeg=");
    Serial.print(GoalDeg);
    Serial.print("GoalDis=");
    Serial.print(GoalDis);
    */


    Serial.print(", GoalY=");
    Serial.print(GoalY);
    Serial.print("Lineover=");
    Serial.print(lineover_check);
    Serial.print(", allsame=");
    Serial.println(all_same_check); 


    if (CamGoalDetected == true && GoalY < Goal_over_dis && abs(CameraV.court_deg + yaw_BNO) > 160) //定数は仮！！！(CamGoalDetected == true && GoalDeg < 30 && GoalY < Goal_over_dis) // 0 - 140 - 140
    {
        Angel.Linedegr = deg_radian(GoalDeg);
    }
    else if (Line_state == Line_States::CORNER)
    {
        if (reversedf_CAM > 155 && reversedf_CAM < 270 && Delection_Mode == true)
        {
            Angel.Linedegr = deg_radian(CameraV.court_deg - 180);
            //Angel.Linedegr = deg_radian(radian_deg(Angel.Linedegr) - 180);
        }
    }
    else if ( (millis() - first_detected_time) < 500 && first_detected == false && (reversed_check < 55 || reversed_check > 150) ) //もと45
    {
        Angel.Linedegr = first_deg;
        //Angel.Linedegr = deg_radian(CameraV.court_deg - 180);
    }
    else if ( reversed_check > 120 && reversed_check < 270 && (Angel.last_detect_time - Angel.old_detect_times[0]) < 350 )
    {
        if (Delection_Mode == 1)
        {
            Angel.Linedegr = deg_radian(CameraV.court_deg - 180);
        }
        else
        {
            Angel.Linedegr = deg_radian(DegRangeChange(radian_deg(Angel.Linedegr) - 180, 180));
        }
    }

    
    if (Line_over == true)
    {
        digitalWrite(LED1, HIGH);
        lineover_move();
    }
    else
    {
        digitalWrite(LED1, LOW);
    }


    if (Angel.old_Linedegr[0] != Angel.Linedegr)
    {
        for (int i = (Angel.HowManyLine - 1) ; i > 0 ; i--)
        {
            //float temp = Angel.old_Linedegr[i];
            //Angel.old_Linedegr[i+1] = temp;
            Angel.old_Linedegr[i] = Angel.old_Linedegr[i-1];
            Angel.old_detect_times[i] = Angel.old_detect_times[i-1];
        }
        Angel.old_Linedegr[0] = Angel.Linedegr;
        Angel.old_detect_times[0] = Angel.last_detect_time;
    }

    if (first_deg != old_first_deg)
    {
        old_first_deg = first_deg;
        old_first_detect_time = first_detected_time;
    }
    
}