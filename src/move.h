#pragma once

#include <Arduino.h>
#include <math.h>

#include "IR_read.h"
#include "Screen.h"
#include "Camera.h"


extern float moveDeg;


void move_setup();
void move_loop();