#include <Wire.h>
#include <mcp_can.h>
#include <SPI.h>
#define SLAVE_ADDRESS 0x04
#define LED  13
int number = 0;
byte buf [10] = {0};
char msgDBG [250] = {0};
byte canDataToPartner [8] = {0};
byte canDataToMe [8] = {0};
int canDataLength = 8;
byte partnerCanId = 0;  
byte myCanId = 0x01;

MCP_CAN CAN0(10);

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  Serial.println("Ready!");

  if(CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK) 
    Serial.println("MCP2515 Initialized Successfully!");
  else 
    Serial.println("Error Initializing MCP2515...");
   CAN0.setMode(MCP_NORMAL);
    
}
void loop() {
  delay(100);
}
void receiveData(int byteCount) {
  int i = 0 ;
  while (Wire.available()) {
    buf[i++] = Wire.read();
  }
  partnerCanId = buf[6]; 
  for (int j = 1 ; j < i ; j ++){
    if( j <= canDataLength ) canDataToPartner[j] = buf[j];
    sprintf(msgDBG, "CanId = %d , idx = %d, Data = %d", partnerCanId,  j , canDataToPartner[j]);
    Serial.println(msgDBG);
  }
  CAN0.sendMsgBuf(myCanId, 0, 8, canDataToPartner); 
}

void sendData() {
  
  for (int i = 0 ; i < canDataLength ; i++){
     canDataToMe[i] = i + 7;      
  }
  partnerCanId = 45;
  
  Wire.write(125);
  Wire.write(partnerCanId);
  for (int i = 0 ; i < canDataLength ; i++){
      Wire.write(canDataToMe[i]);        
  }
  Wire.write(111);
  
}