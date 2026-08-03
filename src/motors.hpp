#pragma once

#include <Arduino.h>

#include <Wire.h>
#include "PID.hpp"
#include "PINOUTs.h"
#include "Serial.h"
#include "Screen.h"
#include "IR_read.h"
//#include "motor"
#include "motorsDSR.hpp"
#include "move.h"
#include "LineRead.h"
#include "Camera.h"
#include "move.h"
//#include "MotorDriver.h"
//#include "global.h"
//#include "readController.h"
//#include "Screen.h"
#include "Kicker.h"
//#include "MotorDriver.h"
//#include "motor_convert.h"
//#include "motors.hpp"
//#include "PID.hpp"


#define SecondRobot

bool motorsInit(HardwareSerial *serial, uint32_t baudrate);
void motorsSetDegPosition(float deg_1ch, float deg_2ch, float deg_3ch, float deg_4ch); // モータの位置をセット
void motorsSetMoveSign(int sign_1ch, int sign_2ch, int sign_3ch, int sign_4ch);        // 移動のための符号をセット
void motorsSetPdSign(int sign_1ch, int sign_2ch, int sign_3ch, int sign_4ch);          // PD制御のための符号をセット
// 制御系
void motorsStop();
void motorsDirectMove(int value_1ch, int value_2ch, int value_3ch, int value_4ch);
void motorsPidProcess(PID *pd, float deg, float target);
void motorsMove(float deg, float power);               // PD出力範囲は-100~100
void motorsPdMove();                                   // PD出力範囲は-100~100


void motors_Setup();
void motors_Update();