#pragma once
#include <Arduino.h>

class PID
{
public:
    PID(float kp, float ki, float kd, float lpf_alpha = 0.2f);

    // 設定変更用
    void setGains(float kp,float ki, float kd);
    void setLPF(float alpha);
    void setDeadband(float band); // 誤差がこの値以下なら出力を0にする

    void useP(bool use);
    void useD(bool use);
    void useI(bool use);
    void process(float val, float target, bool angle = false); // bool →　true か　false　かを表す

    float output() const;
    void reset(float current_val);

private:
    float _kp, _ki, _kd;
    float _lpf_alpha;
    float _deadband = 0.0f;

    float _value = 0.0f;
    float _old_value = 0.0f;
    float _p_power = 0.0f;  //  比例　→　目標値よりも遠いほど大きくなる
    float _integral = 0.0f; // 誤差の積み上げ用
    float _i_limit = 50.0f; // 積分が溜まりすぎないための制限(アンチワインドアップ)
    float _d_power = 0.0f;  //
    float _d_filtered = 0.0f;  //
    float _output = 0.0f;

    bool _useP = true;
    bool _useD = true;
    bool _useI = true;
    uint32_t _last_time = 0;

    // 角度の差分計算
    float getDiffDeg(float a, float b);
};
