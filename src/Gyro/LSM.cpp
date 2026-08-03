#include "Gyro.h"

//------------------------
// 設定
//------------------------
auto& mySerial = Serial;
auto& myWire = Wire;

//#define resetpin 1

//BNO LSM Combining.
bool LSM_STATUS = 0;
uint try_LSM_setup = 0;
double gz_LSM = 0;





#define outputpin 0

float theta = 0.0;

// 時間計測用
unsigned long lastTime;

// ジャイロオフセット
float gyroOffset = 0.0;

float ax_Offset = 0.0f;
float ay_Offset = 0.0f;

uint8_t lsm6_addr = 0x6A;

// レジスタ
constexpr uint8_t REG_WHO_AM_I = 0x0F;
constexpr uint8_t REG_CTRL2 = 0x11;
constexpr uint8_t REG_OUTZ_L_G = 0x26;
constexpr uint8_t REG_OUTX_L_G = 0x22;  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
constexpr uint8_t REG_CTRL1 = 0x10;
//constexpr uint8_t REG_CTRL2 = 0x11;
constexpr uint8_t REG_CTRL6 = 0x15;
constexpr uint8_t REG_CTRL8 = 0x17;
float vx = 0;
float vy = 0;

float gx = 0;
float gy = 0;
float gz = 0;
float ax = 0;
float ay = 0;
float az = 0;
float xx = 0;
float xy = 0;

float dt = 0;

void writeReg(uint8_t reg, uint8_t val)
{
    myWire.beginTransmission(lsm6_addr);
    myWire.write(reg);
    myWire.write(val);
    myWire.endTransmission();
}

void readRegs(uint8_t startReg, uint8_t* buf, uint8_t len) 
{
    myWire.beginTransmission(lsm6_addr);
    myWire.write(startReg);
    myWire.endTransmission(false);
    myWire.requestFrom(lsm6_addr, len);
    for (uint8_t i = 0; i < len; i++) 
    { 
        buf[i] = myWire.read(); 
    }
}

bool lsm6_init()
{
    myWire.beginTransmission(lsm6_addr);
    myWire.write(REG_WHO_AM_I);
    myWire.endTransmission(false);

    myWire.requestFrom(lsm6_addr, (uint8_t)2);
    //readRegs(REG_OUTX_L_G, buf, 12);
    
    if (!myWire.available())
    {
        LSM_STATUS = false;
        return false;
    }

    uint8_t who = myWire.read();

    if (who != 0x70)
    {
        LSM_STATUS = false;
        return false;
    }

    // ジャイロ 2000dps
    writeReg(REG_CTRL1, 0b00001001);
    //writeReg(REG_CTRL2, 0b00001001);
    writeReg(REG_CTRL6, 0b00000100);
    writeReg(REG_CTRL8, 0b10000010);
    writeReg(REG_CTRL2, 0x9C);

    LSM_STATUS = true;
    return true;
}

//------------------------
// Z軸ジャイロ読み取り
//------------------------
float readGyroZ()
{
    myWire.beginTransmission(lsm6_addr);
    myWire.write(REG_OUTZ_L_G);
    myWire.endTransmission(false);

    myWire.requestFrom(lsm6_addr, (uint8_t)12);

    if (myWire.available() < 2)
        return 0;

    uint8_t low = myWire.read();
    uint8_t high = myWire.read();

    int16_t raw = (high << 8) | low;

    // 2000dps設定
    return raw * 0.0045;
}

void readAcceleration()
{
    uint8_t buf[14];
    int16_t buf2[7];
    readRegs(REG_OUTX_L_G, buf, 12);
    for (int i = 0; i < 6; i++)
    {
        buf2[i] = (buf[i * 2 + 1] << 8 | buf[i * 2]); 
    }
    //gx = static_cast<float>(buf2[0]) * 0.07;
    //gy = static_cast<float>(buf2[1]) * 0.07;
    gz = static_cast<float>(buf2[2]) * 0.0045;

    ax = static_cast<float>(buf2[3]) * 0.000244 / 4;
    ay = static_cast<float>(buf2[4]) * 0.000244 / 4;
    az = static_cast<float>(buf2[5]) * 0.000244 / 4;
}


void LSM_setup()
{
    myWire.begin();
    //Serial.begin(250000);

    //pinMode(resetpin, INPUT_PULLDOWN);

    //analogWriteResolution(10);
    Serial.println("LSMbeefe");
    
    while ( !lsm6_init() && try_LSM_setup < 5 )
    {
        Serial.println("LSM6DSV16X not found!");
        LSM_STATUS = false;
        try_LSM_setup ++;
        delay(800);
    }
    
    if (LSM_STATUS == true)
    {
        Serial.println("LSM6DSV16X Ready.");
    }
    else
    {
        Serial.println("LSM6DSV16X not found!");
    }
    

    //------------------------
    // オフセット測定
    //------------------------
    Serial.println("Calibrating... Keep sensor still.");

    float sum_gz = 0;
    float sum_ax = 0;
    float sum_ay = 0;

    for (int i = 0; i < 500; i++)
    {
        sum_gz += readGyroZ();
        readAcceleration();
        sum_ax += ax;
        sum_ay += ay;
        delay(2);
    }

    gyroOffset = sum_gz / 500.0;
    ax_Offset = sum_ax / 500.0;
    ay_Offset = sum_ay /500.0;
    

    Serial.print("Gyro Offset = ");
    Serial.println(gyroOffset);

    lastTime = micros();
}


void LSM_update()
{
    //Serial.println("in_loop");
    //------------------------
    // dt計算
    //------------------------
    unsigned long now = micros();
    dt = (now - lastTime) / 1000000.0f;
    

    //------------------------
    // ジャイロ取得
    //------------------------
    //gz = readGyroZ();
    readAcceleration();

    // オフセット除去
    gz -= gyroOffset;
    ax -= ax_Offset;
    ay -= ay_Offset;
    
    //------------------------
    // 積分
    //------------------------
    lastTime = now;
    theta += gz * dt;
    vx += (ax * 9.80665) * dt;
    vy += (ay * 9.80665) * dt;
    xx += vx;
    xy += vy;


    //x += 1/2 *ax * dt;
    //v = v0 + a * t;

    //------------------------
    // 0～360°
    //------------------------
    while (theta >= 360.0)
        theta -= 360.0;

    while (theta < 0.0)
        theta += 360.0;

    //------------------------
    // リセット
    //------------------------
    if (digitalRead(GYRO_RESET) == 0)
    {
        theta = 0;
        vx = 0;
        vy = 0;
        xx = 0;
        xy = 0;
    }

    //------------------------
    // アナログ出力
    //------------------------
    //int omega_out = (int)(theta * 1023.0 / 360.0);

    //analogWrite(outputpin, omega_out);


    //------------------------
    // シリアル表示
    //------------------------

/*     Serial.print("Theta: ");
    Serial.print(theta);

    Serial.print("  Gz: ");
    Serial.print(gz); */

/*     Serial.print("  dt:");
    Serial.println(dt, 6); */

    #ifdef DEBUG_LSM
        Serial.print("ax:");
        Serial.print(ax);

        Serial.print(", Ay:");
        Serial.print(ay);

        Serial.print(", Z:");
        Serial.println(az);
        
        Serial.print("vx:");
        Serial.print(vx);

        Serial.print(", vy:");
        Serial.print(vy);

        Serial.print(", Theta:");
        Serial.println(theta);

/*         Serial.print("xx:");
        Serial.print(xx);

        Serial.print(", xy:");
        Serial.println(xy); */
    #endif
    gz_LSM = gz;

}