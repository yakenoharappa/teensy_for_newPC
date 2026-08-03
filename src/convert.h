#pragma once

#include <Arduino.h>
#include <math.h>
//deg(度数法) → radian
float deg_radian(int deg);

//radian → deg(度数法)
int radian_deg(float radian);

//基準方向変換
int changedeg(int value, int change_range);

//角度範囲変換
int DegRangeChange(int before, int which);

//8bit→16bit(Serial関係)
int BitChange(uint8_t v1, uint8_t v2);