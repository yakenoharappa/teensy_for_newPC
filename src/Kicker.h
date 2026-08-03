#pragma once
#include <Arduino.h>
#include "PINOUTs.h"
//#include "global.h"
//#include "readController.h"
#include "Screen.h"

extern unsigned long LastKickedTime;
enum class Kicker { Charge, CanUse, Kicking };


void Kicker_Setup();
void Kick();
void Kicker_end();
void Kicker();

extern unsigned long LastKickedTime;
extern bool KickerCharge;
//extern uint CoolTime;