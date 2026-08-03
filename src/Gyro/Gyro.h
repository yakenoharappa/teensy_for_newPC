#pragma once

#include <Arduino.h>
#include <Wire.h>
#include "PINOUTs.h"

#define DEBUG

//const int GYRO_RESET = 1;

#ifdef DEBUG
    //#define DEBUG_LSM
#endif


void BNO_setup();
void BNO_update();

void LSM_update();
void LSM_setup();

void Gyro_setup();
void Gyro_update();

extern float yaw_BNO;
extern float pastdeg;


extern bool LSM_STATUS;
extern float dt;
extern float theta;
extern double gz_LSM;


extern float deg_data;