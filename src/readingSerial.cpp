#include "readingSerial.h"

readingSerial::readingSerial(int number, uint8_t StartC, uint8_t endC, int amount)
    : checkV(amount) , values(amount)
{
    readingSerial::SerialNumber = number;
    readingSerial::Start = StartC;
    readingSerial::End = endC;
    readingSerial::amountData = max(amount, 1);
    
}

void readingSerial::readData()
{
    while (Serials[SerialNumber]->available() >= amountData + 2 )
    {
        //Serial.println("IN1Loop");
        while ( Serials[SerialNumber]->available() >= readingSerial::amountData * 2  &&  Serials[SerialNumber]->peek() != readingSerial::Start )
        {
            //Serial.println("in2loop");
            Serials[SerialNumber]->read();
        }
        
        readingSerial::checkV[0] = Serials[SerialNumber]->read();
        if (uint8_t(readingSerial::checkV[0]) == readingSerial::Start)
        {
            for (int i = 0; i < readingSerial::amountData; i++)
            {
                readingSerial::checkV[i] = Serials[SerialNumber]->read();
            }

            /* 
            dController.two = Serials[SerialNumber]->read();
            dController.three = Serials[SerialNumber]->read();
            dController.four = Serials[SerialNumber]->read();
            dController.five = Serials[SerialNumber]->read();
            dController.six = Serials[SerialNumber]->read();
            dController.fin = Serials[SerialNumber]->read();
            */

            if (uint8_t(Serials[SerialNumber]->read()) == readingSerial::End)
            {
                for (int i = 0; i < readingSerial::amountData; i++){
                    //Serial.println("IN_values");
                    readingSerial::values[i] = readingSerial::checkV[i];
                    //Serial.println(uint8_t(readingSerial::values[i]));
                }

            /* 
                dGood.one = dController.one;
                dGood.two = dController.two;
                dGood.three = dController.three;
                dGood.four = dController.four;
                dGood.five = dController.five;
                dGood.six = dController.six;
                dGood.fin = dController.fin; */
                break;
            }
        }
    }
}