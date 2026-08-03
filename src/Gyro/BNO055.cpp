#include "BNO055.h"
//注意！：通信中にSeeeduinoXiaoやBNO055などの電子部品に触れると燃えるので、絶対に触れないこと！！

//#define DEBUG

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire); //I2Cのアドレス定義

static float Reset = 0;
float new_yaw = 0;
float new_check = 0;
float yaw_BNO = 0;
int BNObegin = 0;
unsigned long last_OKtime = 0;
//unsigned long time = 0;
float degcheck = 0;
float detectdeg = 0;

float pastdeg = 0;

class data_t
{
    public:
    uint16_t main;

    uint8_t getHigh(){
        return((main>>8) & 0xFF);
    }
    uint8_t getLow(){
        return(main & 0xFF);
    }

};


void BNO_setup() {
    //while(!Serial);
    //Serial.begin(115200);
    //pinMode(GYRO_RESET,INPUT_PULLDOWN); //内部プルアップを有効に
    //pinMode(LED_BUILTIN, OUTPUT);
    //pinMode(LED3, OUTPUT);
    Serial.println("BNO_BEGIN");
    //下記のOPERATION_MODE_IMUPLUSを追加してみました。どっちのほうがいいか、比較検討してみてください。
    while (!bno.begin(OPERATION_MODE_IMUPLUS) && BNObegin < 6) {
        Serial.println("BNO055 was not found!");
        BNObegin++;
        delay(1000);
    }
    bno.setExtCrystalUse(true); //必要かどうかわからんけど、性能向上するだろうか、いや、しない。
    float CalibTime = millis();
    
    uint8_t sys, gyro, accel, mag;
    bno.getCalibration(&sys, &gyro, &accel, &mag);
    while( gyro < 2 && millis() - CalibTime < 2000){
        bno.getCalibration(&sys, &gyro, &accel, &mag);
        delay(100);
    }
    
}

static float angle(float Angle){
    while (Angle < -180) Angle += 360;
    while (Angle >  180) Angle -= 360;
    return Angle;
}

static int intia(int minus){
    while (minus < 0)
    {
        minus += 360;
    }
    return minus;
}

/*
float timecheck(){
    if ( fabs(degcheck - yaw_BNO) == 0 ){
        if ( millis() - last_OKtime > 1000 ){
            Serial.print("oh died!?");
            degcheck = yaw_BNO;
            return false;
        }
        else{
            
            return true;
        }
    }
    else{
        if ( millis() - last_OKtime > 1000 ){
        degcheck = yaw_BNO;
        return true;
        }
        else{
            return true;
        }
    }
}
*/

void BNO_update() {
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER); //BNO055からオイラー角を入手

    //Tx_Rxの関数を定義する
    //data_t data1;
    //data_t data2;

//再起動（リスタート）
    new_yaw = euler.x();

    if ( fabs(new_check - new_yaw) <= 0.01 )
    {
        if ( millis() - last_OKtime > 1000 )
        {
            #ifdef DEBUG
                Serial.print("oh died!?");
            #endif
            degcheck = yaw_BNO;
        }
    }
    else
    {
        if ( millis() - last_OKtime > 1000 )
        {
            degcheck = yaw_BNO;
        }
    }




    if ( isnan(new_yaw) || fabs(new_check - new_yaw) <= 0.05 )
    {
        if ( millis() - last_OKtime > 500 )
        {
            degcheck = yaw_BNO;
            #ifdef DEBUG
                Serial.print("error!!");

                //I2Cをやり直す（I2Cをリスタート）
                /* 
                Wire.end();
                delay(10);
                Wire.begin(); */

/*                 if ( !bno.begin(OPERATION_MODE_IMUPLUS ) )
                {
                    Serial.print("BNO055 could't restart");
                    
                }
                else
                {
                    Serial.print("BNO recovered!!");
                    bno.setExtCrystalUse(true);
                } */
            #endif

            new_yaw = euler.x();
            detectdeg += degcheck - new_yaw; //新10_旧0
            //Serial.print(offset);

            last_OKtime = millis(); /*ここじゃないと、二個目のifの意味がなくなるよ*/
            new_check = new_yaw;
        }
    }
    else
    {
        if ( millis() - last_OKtime > 1000 )
        {
            new_check = new_yaw;
        }
        last_OKtime = millis();
    }


    yaw_BNO = new_yaw + detectdeg;


    //float pitch = euler.y();
    //float roll = euler.z();
    bool Button = digitalRead(GYRO_RESET);
//boolとは、０か１か（true or false）を表す変数。intより更に処理が簡潔なので、処理速度の向上が見込まれる。




//リセットボタン
    if (Button == 0){
        Reset = yaw_BNO;
        //Serial.print("Button");
        Serial.print(Button);
        digitalWrite(LED3, LOW);
    }else{
        digitalWrite(LED3, HIGH);
    }
    yaw_BNO = yaw_BNO - Reset;


//TxRxデータ保存
    //data1.main = uint16_t(intia(yaw_BNO));
//DAC変換
    yaw_BNO = angle(yaw_BNO);
    int dacvalue = (yaw_BNO + 180) * 1023 / 360;

//LED
/*     if ( fabs(yaw_BNO) < 10 )
    {
        digitalWrite(LED_BUILTIN, LOW);
        digitalWrite(LED2, HIGH);
    }
    else if ( fabs(yaw_BNO) > 170 )
    {
        digitalWrite(LED_BUILTIN, HIGH);
        digitalWrite(LED2, LOW);
    }
    else
    {
        digitalWrite(LED_BUILTIN, HIGH);
        digitalWrite(LED2, HIGH);
    } */

//DAC転送
    //analogWrite(DAC_PIN, dacvalue); //TJ3Bcoreに転送する


    #ifdef DEBUG
    //TxRx転送
    /*
        Serial1.write(0xFF);
        Serial1.write(data1.    High());
        Serial1.write(data1.getLow());
        Serial1.write(0xAA); */

        //Serial.print(Reset);
        Serial.print("角度:" );
        Serial.println(yaw_BNO);
        /*
        Serial.print("value=");
        Serial.print(dacvalue);
        Serial.print(","); 
        Serial.print(pitch); 
        Serial.print(","); 
        Serial.print(roll);
        Serial.println(",");
        */
    #endif
    
    //delay(10); 

}