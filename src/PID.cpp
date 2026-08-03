#include "PID.hpp"

PID::PID(float kp,float ki, float kd, float lpf_alpha)
    : _kp(kp),_ki(ki), _kd(kd), _lpf_alpha(lpf_alpha)
{
    _last_time = micros();
}

void PID::setGains(float kp, float ki, float kd)
{
    _kp = kp;
    //_ki = ki;
    _kd = kd;
}

void PID::setLPF(float alpha)
{
    _lpf_alpha = constrain(alpha, 0.01f, 1.0f);
}

void PID::setDeadband(float band)
{
    _deadband = fabsf(band); //fabsf →　絶対値
}

void PID::useP(bool use) { _useP = use; }
void PID::useD(bool use) { _useD = use; }
//void PID::useI(bool use) { _useI = use; }
void PID::process(float val, float target, bool angle)
{
    uint32_t now = micros();
    float dt = (float)(now - _last_time) / 1000000.0f; // 秒単位に変換
    if (dt <= 0)
        dt = 0.001f; // ゼロ除算防止
    _last_time = now;

    _value = val;

    float error = angle ? getDiffDeg(target, _value) : (target - _value); //三項演算子　ifとelseを１文にまとめて使えるもの　？と：を使う

    if (fabsf(error) < _deadband)
        error = 0.0f;
    _p_power = error * _kp; //PIDのPの部分を計算
/*     if (_useI) 
    {
        _integral += error * dt; // 誤差に時間をかけて積み上げる
        // 積分が溜まりすぎると、目標を超えても止まらなくなるので制限をかける
        _integral = constrain(_integral, -_i_limit, _i_limit); 
    }
    float _i_power = _integral * _ki; //PIDのIの部分を計算 */
    float diff = angle ? getDiffDeg(_value, _old_value) : (_value - _old_value); 

    float raw_d_speed = diff / dt;
    float raw_d_power = raw_d_speed * _kd;

    _d_filtered = (_lpf_alpha * raw_d_power) + ((1.0f - _lpf_alpha) * _d_filtered);
     //lps →　ローパスフィルター　微分計算はセンサーのノイズに非常に弱いから
    _d_power = _d_filtered;

    _old_value = _value; //PIDのDの部分を計算

    float total_power = 0.0f;
    if (_useP)
        total_power += _p_power;
/*     if (_useI)
        total_power += _i_power;  */
    if (_useD)
        total_power += - _d_power * 0.1f;  // Kdを負の値で渡している場合は加算、正なら減算

    _output = constrain(total_power, -100.0f, 100.0f);//constrain →　制限
}

float PID::output() const
{
    return _output;
}

void PID::reset(float current_val)
{
    _old_value = current_val;
    _d_filtered = 0.0f;
    _integral = 0.0f;
    _output = 0.0f;
    _last_time = micros();
}

float PID::getDiffDeg(float target, float current)
{
    float diff = target - current;
    while (diff > 180.0f)
        diff -= 360.0f;
    while (diff < -180.0f)
        diff += 360.0f;
    return diff;
}