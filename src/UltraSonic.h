#pragma once

class UltraSonic
{

public:

    static const int amount = 4;
    int pin[amount] = {5, 6, 7, 8};
    float value[amount];

};

extern UltraSonic MyUSonic;
//const int UltraSonicPIN[HowManyUltraSonic] = {5, 6, 7, 8};
