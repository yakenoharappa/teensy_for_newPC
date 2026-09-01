#pragma once

#include <Arduino.h>
#include <math.h>

#include "IR_read.h"
#include "Screen.h"
#include "Camera.h"
#include "PID.hpp"
#include "move.h"

//extern float moveDeg;


void DEFENCE_setup();
void DEFENCE_loop();

//extern int MoveSpeed;