#include <mcp_can.h>
#include <SPI.h>
#define CAN0_INT 2                              

byte myCanId = 0x04 ;    
byte myPartnerCanId = 0x02 ;
byte canSendStat ;        
char msgString[128]; 
long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
byte canData[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
int canLen = 8;

int led_1 = 4 ;
int led_2 = 5 ;
int led_3 = 6;
int led_4 = 7;
int numOfLed = 4;

int led_status[4] = {0,0,0,0};

MCP_CAN CAN0(10);                               

void setup(){
  Serial.begin(115200);
  if(CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");
  CAN0.setMode(MCP_NORMAL);     
  pinMode(CAN0_INT, INPUT);    
  pinMode(led_1, OUTPUT);pinMode(led_2, OUTPUT);
  pinMode(led_3, OUTPUT);pinMode(led_4, OUTPUT);
  Serial.println("MCP2515 Library Receive Example...");
}

void loop(){
  readCanData();
  processCanData();
  switchLeds();
  prepareCanData();
  dumpProceData();
  dumpPreparedCanData();
  sendCanData();
  delay (100);
}



void readCanData(){
    CAN0.readMsgBuf(&rxId, &len, rxBuf);            
  Serial.println("Dumping Can data Received");
  dumpCanData();
  
}

void switchLeds(){
      if (rxBuf[0] == 1) digitalWrite(led_1, HIGH); else digitalWrite(led_1, LOW);
      if (rxBuf[1] == 1) digitalWrite(led_2, HIGH); else digitalWrite(led_2, LOW);
      if (rxBuf[2] == 1) digitalWrite(led_3, HIGH); else digitalWrite(led_3, LOW);
      if (rxBuf[3] == 1) digitalWrite(led_4, HIGH); else digitalWrite(led_4, LOW);
}


void prepareCanData(){
  int i = 0;
  for (i = 0; i < numOfLed; i++)
    if (led_status[i] == 1) canData[canLen - i] = 1 ; else canData[canLen - i] = 0;
}


void processCanData(){
  int i = 0 ;
   for ( i = 0 ; i < 4 ; i++)
      if (rxBuf[i] == 1) led_status[i] = 1; else led_status[i] = 0;
}

void dumpProceData(){
     Serial.println("****************=**************"); 
    for(byte i = 0; i < numOfLed; i++){
        sprintf(msgString, " 0x%.2X", led_status[i]);
        Serial.print(msgString);
    }
    Serial.println(); 
} 

void dumpCanData(){
    Serial.println("dUMPING rECVD cAN dATA"); 
    for(byte i = 0; i < len; i++){
        sprintf(msgString, " 0x%.2X", rxBuf[i]);
        Serial.print(msgString);
    }
    Serial.println(); 
} 

void dumpPreparedCanData(){
     Serial.println("=============dUMPING PREPARED dATA==========="); 
    for(byte i = 0; i < canLen; i++){
        sprintf(msgString, " 0x%.2X", canData[i]);
        Serial.print(msgString);
    }
    Serial.println(); 
} 


byte sendCanData(void){
   return CAN0.sendMsgBuf(myCanId, 0, 8, canData); 
}
