#include <mcp_can.h>
#include <SPI.h>
#define CAN0_INT 2                              // Set INT to pin 2

MCP_CAN CAN0(10); // Set CS to pin 10
byte canData[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const int iButtonPin = 8; // push button pin work as INPUT
const int oLedPin = 4;   // the number of the LED pin work as OUTPUT
int buttonState = 0;      // variable for reading the pushbutton status
byte myCanId = 0x01 ;    // Let my Tranmission Node be have id = 0x100
byte myPartnerCanId = 0x02 ;
byte canSendStat ;        // Holds can send status Data
char msgString[128]; 
long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];

void setup(){
  Serial.begin(115200);
  
  pinMode(iButtonPin, INPUT); //initialize the pushbutton pin as an input
  pinMode(oLedPin, OUTPUT); //initialize the Led pin as an output 
   
  /* 
   *  Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s 
   *  and the masks and filters disabled.
   */

  if(CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK) 
    Serial.println("MCP2515 Initialized Successfully!");
  else 
    Serial.println("Error Initializing MCP2515...");
 
  /*
   * Change to normal mode to allow messages to be transmitted
   */
  CAN0.setMode(MCP_NORMAL);   
  //setAndSendCanData (0,0x00);
}


void loop(){
  
  buttonState = digitalRead(iButtonPin);  // read the state of the pushbutton value
  Serial.print("Button State");
  sprintf(msgString, " 0x%.2X", buttonState);
  Serial.println("\n");
 
  /* 
   * check if the pushbutton is pressed.    
   * if it is, the buttonState is HIGH 
   * turn LED off:  
   */ 
  if (buttonState == HIGH) {
      Serial.println("Button Pushed");
      setAndSendCanData (0,0x01);
  }
  else {
      Serial.println("Button Released");
      setAndSendCanData (0,0x00);
  }
  /* 
   * check if the valid can message.    
   * and can message has data to set Local LED on OR off  
   */ 
  if(!digitalRead(CAN0_INT)){                        // If CAN0_INT pin is low, read receive buffer
    setLedOnROffByCanData();
    dumpCanData();
  }
  delay(100);   // send data per 100ms
}

void setLedOnROffByCanData(){
    CAN0.readMsgBuf(&rxId, &len, rxBuf);            // Read data: len = data length, buf = data byte(s)
    Serial.print("Data from Can Node with ID: 0x");
    Serial.println(rxId, HEX);
    if (rxId == myPartnerCanId){
    if (!rxBuf[0]){
      digitalWrite(oLedPin, LOW);
    }else{
      digitalWrite(oLedPin, HIGH);
      }
  }else{
    Serial.print("I am not getting partner ID : 0x");
    Serial.println(myPartnerCanId, HEX);
  }
}

void dumpCanData(){
   /* 
   * check if the valid can message.    
   * and can message has data to set Local LED on OR off  
   */ 
    for(byte i = 0; i<len; i++){
        sprintf(msgString, " 0x%.2X", rxBuf[i]);
        Serial.print(msgString);
    }
    Serial.println(); 
} 

void setAndSendCanData(int index, byte value){
    setCanData(index,value);
    canSendStat = sendCanData();
    if(canSendStat == CAN_OK){
      Serial.println("Message Sent Successfully!");
    } else {
      Serial.println("Error Sending Message...");
    }
}

void setCanData (int index, byte value){
     canData[index] = value;
}

byte sendCanData(void){
  /*
   *  send data:  
   *  ID = 0x100, 
   *  Standard CAN Frame, 
   *  Data length = 8 bytes, 
   *  'data' = array of data bytes to send 
   */
   return CAN0.sendMsgBuf(myCanId, 0, 8, canData); 
}
