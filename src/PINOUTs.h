#pragma once

#include <Arduino.h>

//Kicker
const int FET1 = 2;
const int FET2 = 3;

//Chatch_Sensor
const int Catch_PIN = 30;
/* 光変調フォトIC真理値
    - 光ON⇒LOW
    - 光OFF⇒HIGH
    https://akizukidenshi.com/catalog/g/g106321/
*/


//LED
const int LED1 = 4;
const int LED2 = 5;
const int LED3 = 6;
const int LED4 = 9;


//PWM for Dribller
const int PWM = 8;

#define Senter 12    //Decide
#define buttom 11    //Down
#define front 10     //Up
#define back 33

const int UP_BUTTTON = 12;
const int ENTER_BUTTON = 13;
const int BACK_BUTTON = 11;
const int DOWN_BUTTON = 10;
const int TOGGLE_PIN = 33;

const int LCD_SCL = 16;
const int LCD_SDA = 17;

const int GYRO_SDA = 18;
const int GYRO_SCL = 19;
const int GYRO_RESET = 23;

const int Buzzer_PIN = 22;

//予備ports
