#pragma once

#include <Arduino.h>
#include <math.h>

#include "IR_read.h"
#include "Screen.h"
#include "Camera.h"
#include "PID.hpp"


extern float moveDeg;


void move_setup();
void move_loop();

extern int MoveSpeed;

extern float ball_deg;
extern int ball_dis;

extern int Goal_movedeg;
extern int speedmix;