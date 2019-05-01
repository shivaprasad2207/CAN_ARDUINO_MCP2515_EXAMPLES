#include <SPI.h>
#include "mcp_can.h"

unsigned long canId;
const int spiCSPin = 10;
const int ledPin = 2;
boolean ledON = 1;

MCP_CAN CAN(spiCSPin);

void setup()
{
    Serial.begin(115200);
    pinMode(ledPin,OUTPUT);

   if(CAN.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK) Serial.println("MCP2515 Initialized Successfully!");
    else Serial.println("Error Initializing MCP2515...");
    Serial.println("CAN BUS Shield Init OK!");
}


void loop()
{
    unsigned char len = 0;
    unsigned char buf[8];

        CAN.readMsgBuf(&canId, &len, buf);


        Serial.println("-----------------------------");
        Serial.print("Data from ID: 0x");
        Serial.println(canId, HEX);
        Serial.println(len);
        for(int i = 0; i<len; i++)
        {
            Serial.print("LALAL");
            Serial.print(buf[i]);
            Serial.print("\t");
            if(ledON && i==0)
            {

                digitalWrite(ledPin, buf[i]);
                ledON = 0;
                delay(500);
            }
            else if((!(ledON)) && i==4)
            {

                digitalWrite(ledPin, buf[i]);
                ledON = 1;
            }
        }
        Serial.println();
        delay(2000);
    
}