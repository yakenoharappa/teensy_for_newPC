#include <Arduino.h>
#include <EEPROM.h>
#include "PINOUTs.h"
#include "ATACKER.h"


void setup() 
{
  // put your setup code here, to run once:
    Serial.begin(115200);
    Serial1.begin(115200);
    Serial2.begin(115200);
    Serial3.begin(115200);
    Serial7.begin(115200);
	//pinMode(GYRO_RESET, INPUT_PULLUP);
	pinMode(LED1, OUTPUT);
	pinMode(LED4, OUTPUT);
	pinMode(LED3, OUTPUT);
	pinMode(LED4, OUTPUT);

	pinMode(GYRO_RESET, INPUT_PULLUP);
	pinMode(BACK_BUTTON, INPUT_PULLUP);
	pinMode(DOWN_BUTTON, INPUT_PULLUP);
	pinMode(UP_BUTTTON, INPUT_PULLUP);
	pinMode(ENTER_BUTTON, INPUT_PULLUP);
	
	ATACKER_setup();
}


void loop() 
{
  	// put your main code here, to run repeatedly:
	
	//digitalWrite(LED4, LOW);
	//digitalWrite(LED3, HIGH);
	
	/* Teensyフラッシュメモリアクセス
  		https://www.pjrc.com/teensy/td_libs_EEPROM.html */
	ATACKER_loop();

	//Serial.print(digitalRead(ENTER_BUTTON));
	//Serial.print(digital)
	
}