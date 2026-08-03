#pragma once
#include <Arduino.h>

static HardwareSerial *Serials[] = { NULL, &Serial1, &Serial2, &Serial3, &Serial4, &Serial5, &Serial6, &Serial7};
const int MOTORSerial = 1;
const int USSerial = 1;
const int LineSerial = 2;
//const int ControllerSerial = 2;
const int IRSerial = 7;

const int cameraSerial = 3;

