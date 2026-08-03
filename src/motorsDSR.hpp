#pragma once

#include <Arduino.h>

class DSR1202
{
private:
    HardwareSerial *_serial;
    uint32_t _baudrate;
uint32_t _last_command_time = 0; // 最後に命令を受信した時刻を記録
const uint32_t TIMEOUT_THRESHOLD = 500; // 何ミリ秒途切れたら止めるか（500ms程度が一般的）



public:
    // 関数・コンストラクタ
    DSR1202(HardwareSerial *serial, uint32_t baudrate);
    void begin();
    void stop();
    void move(int value_1ch, int value_2ch, int value_3ch, int value_4ch);
    bool isToggleOn();
    void update();

};