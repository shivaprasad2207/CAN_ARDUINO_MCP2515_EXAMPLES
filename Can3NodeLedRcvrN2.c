#include <mcp_can.h>
#include <SPI.h>
#define CAN0_INT 2                              // Set INT to pin 2

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
const int ledPin = 4;
char msgString[128]; 

MCP_CAN CAN0(10);                               // Set CS to pin 10


void setup()
{
  Serial.begin(115200);
  
  // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if(CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");
  
  CAN0.setMode(MCP_NORMAL);     // Set operation mode to normal so the MCP2515 sends acks to received data.

  pinMode(CAN0_INT, INPUT);     // Configuring pin for /INT input
  pinMode(ledPin, OUTPUT);
  Serial.println("MCP2515 Library Receive Example...");
}

void loop(){
  if(!digitalRead(CAN0_INT)){                        // If CAN0_INT pin is low, read receive buffer
  
    CAN0.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)
    Serial.print("Data from ID: 0x");
    Serial.println(rxId, HEX);
    if (rxId == 0x01){
      if (!rxBuf[1]){
        digitalWrite(ledPin, LOW);
      }else{
        digitalWrite(ledPin, HIGH); 

      }
    }
    for(byte i = 0; i<len; i++){
        sprintf(msgString, " 0x%.2X", rxBuf[i]);
        Serial.print(msgString);
    }
        
    Serial.println();
  }
}