#include "Kicker.h"

unsigned long LastKickedTime = millis();
bool KickerCharge = 0;
const int CoolTime = 1000;



void Kicker_Setup()
{
    pinMode(FET1, OUTPUT);
    pinMode(FET2, OUTPUT);
    pinMode(Catch_PIN, INPUT);
    LastKickedTime = millis();
}

void Kicker()
{
    //if ()    //要変更
    if ( KickerOnOff == true && digitalRead(Catch_PIN) == 0 )
    {
        Kick();
    }
    else if ( (millis() - LastKickedTime) >= 300 )
    {
        Kicker_end();
    } 
}


void Kick()
{
    if ( (millis() - LastKickedTime) > CoolTime && KickerCharge == 1 )  //Kick!!!
    {
        digitalWrite(FET2, HIGH);
        digitalWrite(FET1, LOW);
        LastKickedTime = millis();
        KickerCharge = 0;
        digitalWrite(LED4, HIGH);
        digitalWrite(LED1, LOW);
    }
    else if ( (millis() - LastKickedTime) <= CoolTime && (millis() - LastKickedTime) >= 300 )   //充電
    {
        digitalWrite(FET2, LOW);
        digitalWrite(FET1, HIGH);
        KickerCharge = 0;
        digitalWrite(LED4, LOW);
        digitalWrite(LED1, LOW);
    }
    else if ( (millis() - LastKickedTime) >= CoolTime )      // >Cooltimeで、Kicker == 0    //充電完了！
    {
        digitalWrite(FET2, LOW);
        digitalWrite(FET1, LOW);
        KickerCharge = 1;
        digitalWrite(LED4, LOW);
        digitalWrite(LED1, HIGH);
    }
    
/* 
    if (digitalRead(FET2) == HIGH)
    {
        digitalWrite(FET2, LOW);
        digitalWrite(FET1, HIGH);
        digitalWrite(LED4, LOW);
    } */
}

void Kicker_end()
{
    if ( (millis() - LastKickedTime) >= CoolTime )
    {
        digitalWrite(FET2, LOW);
        digitalWrite(FET1, LOW);
        digitalWrite(LED4, LOW);
        digitalWrite(LED1, HIGH);
    }
    else
    {
        digitalWrite(FET2, LOW);
        digitalWrite(FET1, HIGH);
        digitalWrite(LED4, LOW);
        digitalWrite(LED1, LOW);
    }
}