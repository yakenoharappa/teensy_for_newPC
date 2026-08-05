#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <fonts/FreeSerifBold9pt7b.h>


#include "PINOUTs.h"
//#include "global.h"
//#include "UltraSonic.h"
#include "convert.h"
//#include "Jyunya.h"
//#include "readController.h"
#include "Kicker.h"
#include "Camera.h"

#include "Gyro/Gyro.h"

#include "IR_read.h"
#include "LineRead.h"
#include "LineMove.h"


#include "PINOUTs.h"
//#include "Screen.h"
#include "UltraSonic.h"

//ButtonのPINを設定する。


//Screen.cppで使用する && 外部で使用する可能性のある変数たち
extern int LeftRight;
extern long LastTouched;
extern bool KickerOnOff;


#define SCREEN_WIDTH 128    //画面ピクセル幅
#define SCREEN_HEIGHT 64    //画面ピクセル高さ

#define FONT_HEIGHT 8

extern Adafruit_SSD1306 display;
enum class Status { Menu, Gyro, IR, Line, Kicker, ONKicker, Runnning, Controller, Speed, UltraSonic, Timer, TimeSelect,  Image, Camera, Mode};
enum class Menu_layout { Mode, Sensor, Camera, Speed, Kicker, Image };
enum class Sensor_layout { Gyro, IR, Line, Kicker, UltraSonic };



class Screen_Gyro
{

public:
    float degf;
    int degi;
    float cosGf(){
        return cosf(deg_radian(degf));
    }
    float sinGf(){
        return sinf(deg_radian(degf));
    }

    float cosGi(){
        return cosf(deg_radian(degi));
    }
    float sinGi(){
        return sinf(deg_radian(degi));
    }
};


//extern float yaw_BNO;

extern Screen_Gyro BNOdeg;
extern Screen_Gyro MIXdeg;
extern Screen_Gyro LSMdeg;

extern int MotorSpeed;

void Screen_setup();
void Screen_Update();
void ClearScreen();