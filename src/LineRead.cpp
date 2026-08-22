#include "LineRead.h"

//const int HowManyLine = 8;
//bool Line[8] = {0, 0, 0, 0, 0, 0, 0, 0};
//bool Lineport[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
bool LineNeed = 0;
const float Linedegs[] = {5.625 *2, 5.625*6, 5.625*10 , 5.625*14, 5.625*18, 5.625*22, 5.625*26, 5.625*30, 5.625*34, 5.625*38, 5.625*42, 5.625*46, 5.625*50, 5.625*54, 5.625*58, 5.625*62};

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

bool first_detected = 0;
float first_deg = 0;
int first_detected_time = 0;

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


void MakingBlock()
{
    amount_no_BlockLine = 0;
    amount_LineBlock = 0;
    BlockStart = 20;
    BlockEnd = 20;
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
        else
        {
            no_BlockLine[amount_no_BlockLine] = i;
            Angel.sumX += Angel.Detect[i] * cos(deg_radian(Angel.degs[i])); // - deg_data
            Angel.sumY += Angel.Detect[i] * sin(deg_radian(Angel.degs[i])); // - deg_data
            amount_no_BlockLine++;
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
    }
}


void LineRead_Setup()
{
    
}

void LineRead_update()
{
    Angel.sumX = 0;
    Angel.sumY = 0;
    LineNeed = false;
    Linedata.readData();
    Angel.number_of_detect = 0;
    
    
    for (int i = 0; i < 16; i++)
    {
        Angel.Detect[i] = (Linedata.values[0] | (Linedata.values[1] << 8)) >> i & 0b01;
        if (Angel.Detect[i] == true)
        {
            Angel.number_of_detect++;
            LineNeed = true;
            //Angel.sumX +=  cos(deg_radian(Angel.degs[i] - deg_data));
            //Angel.sumY +=  sin(deg_radian(Angel.degs[i] - deg_data));
            Angel.Linedegr = deg_radian(Angel.degs[i]);
        }
    }

    MakingBlock();
    int hairetsu[8] = {0};
    
    for (int i = 0; i < amount_LineBlock; i++)
    {
        if (LineBlocks_END[i] < LineBlocks_START[i])
        {
            hairetsu[i] = ((15 - LineBlocks_START[i]) + LineBlocks_END[i] + 1)%16;
        }
        else
        {
            hairetsu[i] = abs(LineBlocks_END[i] - LineBlocks_START[i] + 1)%16;
        }
        LineBlocks_DEG[i] = 0;
        LineBlocks_sin[i] = 0;
        LineBlocks_cos[i] = 0;
        for (int j = 0; j < hairetsu[i] ; j++)
        {
            LineBlocks_cos[i] += (cos(deg_radian(Angel.degs[(LineBlocks_START[i] + j)%16] )) / hairetsu[i]); //- deg_data
            LineBlocks_sin[i] += (sin(deg_radian(Angel.degs[(LineBlocks_START[i] + j)%16] )) / hairetsu[i]); //- deg_data
            LineBlocks_DEG[i] += DegRangeChange(Angel.degs[(LineBlocks_START[i] + j)%16], -180) / hairetsu[i];
        }
        Angel.sumX += LineBlocks_cos[i];
        Angel.sumY += LineBlocks_sin[i];
    }

    float AngelAtan = atan2(Angel.sumY, Angel.sumX);
    
    for (int i = 0; i < 3; i++)
    {
        SideLineV[i] = Linedata.values[2] >> i & 0b01;
    }
    Angel.Right = SideLineV[0];
    Angel.Back = SideLineV[1];
    Angel.Left = SideLineV[2];


    if (LineNeed == true)
    {
        Angel.last_detect_time = millis();
        if ( Angel.number_of_detect == 1 )
        {
            
        }
        else    // else if ( Angel.number_of_detect >= 2 )
        {
            Angel.Linedegr = AngelAtan;
            if (Angel.number_of_detect == 2 && fabs(Angel.Linedegr) <= 0.01 && fabsf(Angel.sumX) <= 0.01 && fabsf(Angel.sumY) <= 0.01 && (Angel.last_detect_time - Angel.old_detect_times[0]) < 500 )
            {
                Angel.Linedegr = Angel.old_Linedegr[0];
                Serial.print("check:");
                Serial.println(CameraV.court_deg - CameraV.blue_deg);
            }
            else
            {
                Angel.Linedegr = AngelAtan;
            }
        }

        if ((Angel.last_detect_time - Angel.old_detect_times[0]) > 375 || (abs(Angel.Linedegr - Angel.old_Linedegr[0]) < 45 || abs(Angel.Linedegr - Angel.old_Linedegr[0]) > 270) )
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
    }
    else
    {
        first_detected = false;
    }
    
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
    
    if ( (millis() - first_detected_time) < 500 && first_detected == false && (reversed_check < 45 || reversed_check > 150) )
    {
        Angel.Linedegr = first_deg;
    }
    else if ( reversed_check > 120 && reversed_check < 270 && (Angel.last_detect_time - Angel.old_detect_times[0]) < 350 )
    {
        if (Delection_Mode == 1)
        {
            Angel.Linedegr = deg_radian(CameraV.court_deg);
        }
        else
        {
            Angel.Linedegr = deg_radian(DegRangeChange(radian_deg(Angel.Linedegr) - 180, 180));
        }
    }

    if (Angel.Linedegr != Angel.old_Linedegr[0])
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
}