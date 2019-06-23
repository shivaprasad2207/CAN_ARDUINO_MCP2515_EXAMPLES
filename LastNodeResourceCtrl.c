#include <mcp_can.h>
#include <SPI.h>
#define CAN0_INT 2                              // Set INT to pin 2

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
const int ledPins[2] = {3,4};
const int numOfLeds = 2;
const int fanPins[2] = {5,7};
const int numOfFans = 2;
char msgString[128]; 
char resourceTypeIdx = 3;
char opTypeIdx = 4;
char resourceIdIdx = 5;
char resourceDataIdx = 6;

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
  
  for (int i = 0 ; i < numOfLeds ; i++)  
    pinMode(ledPins[i], OUTPUT);

  for (int i = 0 ; i < numOfFans ; i++)  
    pinMode(fanPins[i], OUTPUT);

  Serial.println("MCP2515 Library Receive Example...");
}

void loop(){
  if(!digitalRead(CAN0_INT)){                        // If CAN0_INT pin is low, read receive buffer
  
    CAN0.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)
    for(byte i = 0; i<len; i++){
        sprintf(msgString, " 0x%.2X", rxBuf[i]);
        Serial.print(msgString);
    }    
    Serial.println();
    
    Serial.print("Data from ID: 0x");
    Serial.println(rxId, HEX);
    switch (rxBuf[resourceTypeIdx]){
        case 1: ledOperation();
                Serial.println("Got Led operations");
                break;
        case 2: Serial.println("Got Fan operations");
                fanOperation();
                Serial.println("Got Fan operations");
                break;
        default: Serial.println("Dont know on which Resource operations");
                 break;
    }
    
  }
}

void ledOperation(){
    switch (rxBuf[opTypeIdx]){
        case 0x02: ledPutOperation();
                Serial.println("Got Led PUT operations");
                break;
        case 0x01: ledGetOperation();
                Serial.println("Got LED GET operations");
                break;
        default: Serial.println("Dont know on which REST operation index it is not 0x01 or 0x02");
                 break;
    }
}

void ledPutOperation(){
    if (rxBuf[resourceIdIdx] > 0 && rxBuf[resourceIdIdx] <= numOfLeds ){
      if (rxBuf[resourceDataIdx])  
        digitalWrite(ledPins[rxBuf[resourceIdIdx]-1], HIGH);      
      else
        digitalWrite(ledPins[rxBuf[resourceIdIdx]-1], LOW);  
    }else{
      Serial.println("Dont know which LED to operate");  
    }
}


void fanOperation(){
    switch (rxBuf[opTypeIdx]){
        case 0x02: fanPutOperation();
                Serial.println("Got Fan PUT operations");
                break;
        case 0x01: fanGetOperation();
                Serial.println("Got Fan GET operations");
                break;
        default: Serial.println("Dont know on which REST operation index it is not 0x01 or 0x02");
                break;
    }
}

void fanPutOperation(){
    if (rxBuf[resourceIdIdx] > 0 && rxBuf[resourceIdIdx] <= numOfFans ){
      Serial.println("Entering into FAN fanPutOperation ");
      if (rxBuf[resourceDataIdx])  
        digitalWrite(fanPins[rxBuf[resourceIdIdx]-1], HIGH);      
      else
        digitalWrite(fanPins[rxBuf[resourceIdIdx]-1], LOW);  
    }else{
      Serial.println("Dont know which FAN to operate");  
    }
}

void ledGetOperation(){
  
}
void fanGetOperation(){
}