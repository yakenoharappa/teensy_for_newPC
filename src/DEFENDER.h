#pragma once

#include <Arduino.h>
#include "move.h"
#include "LineMove.h"
#include "motors.hpp"
#include "Gyro/Gyro.h"
#include "Camera.h"
#include "Screen.h"
#include "PINOUTs.h"
#include "LineRead.h"
#include "Kicker.h"
#include "DEFENCE.h"

void DEFENDER_setup();
void DEFENDER_loop();
