#include <mcp_can.h>
#include <SPI.h>

MCP_CAN CAN0(10); // Set CS to pin 10
byte canData[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const int iButtonPin = 8; // push button pin work as INPUT
const int iButtonPin2 = 7; // push button pin work as INPUT

const int oLedPin = 4;   // the number of the LED pin work as OUTPUT
const int oLedPin2 = 2;   // the number of the LED pin work as OUTPUT
int buttonState = 0;      // variable for reading the pushbutton status
int buttonState2 = 0;      // variable for reading the pushbutton status
byte myCanId = 0x01 ;    // Let my Tranmission Node be have id = 0x100
byte canSendStat ;        // Holds can send status Data
char msgString[128]; 


void setup(){
  Serial.begin(115200);
  
  pinMode(iButtonPin, INPUT); //initialize the pushbutton pin as an input
  pinMode(iButtonPin2, INPUT); //initialize the pushbutton pin as an input
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
  int idx[2] = {0,1};
  byte vals[2] = {0x00,0x00}; 
  setAndSendCanData (idx,vals,2);
}

void loop(){
  
  buttonState = digitalRead(iButtonPin);  // read the state of the pushbutton value
  buttonState2 = digitalRead(iButtonPin2);  // read the state of the pushbutton value
  sprintf(msgString, " 0x%.2X", buttonState);
  Serial.print(msgString);
  /* 
   * check if the pushbutton is pressed.    
   * if it is, the buttonState is HIGH 
   * turn LED off:  
   */ 

    if (buttonState == HIGH && buttonState2 == LOW) {
      Serial.println("Button1 Pushed and Button2 is Released");
      digitalWrite(oLedPin, HIGH);
      digitalWrite(oLedPin2, LOW);
      int idx[2] = {0,1};
      byte vals[2] = {0x01,0x00}; 
      setAndSendCanData (idx,vals,2);
      //setAndSendCanData ([0,1],[0x01,0x00],2);
    }else if (buttonState == LOW && buttonState2 == HIGH) {
      Serial.println("Button2 Pushed and Button1 is Released");
      digitalWrite(oLedPin, LOW);
      digitalWrite(oLedPin2, HIGH);
      int idx[2] = {0,1};
      byte vals[2] = {0x00,0x01}; 
      setAndSendCanData (idx,vals,2);
      //setAndSendCanData ([0,1],[0x00,0x01],2);
    }else{
      Serial.println("Button2 Released and Button1 is Released");
      digitalWrite(oLedPin, LOW);
      digitalWrite(oLedPin2, LOW);
      int idx[2] = {0,1};
      byte vals[2] = {0x00,0x00}; 
      setAndSendCanData (idx,vals,2);
      //setAndSendCanData ({0,1},{0x00,0x00},2);
    }
    
  delay(100);   // send data per 100ms
}

void setAndSendCanData(int index[], byte value[], int len){
    setCanData(index,value,len);
    canSendStat = sendCanData();
    if(canSendStat == CAN_OK){
      Serial.println("Message Sent Successfully!");
    } else {
      Serial.println("Error Sending Message...");
    }
}

void setCanData (int index[], byte value[],int len){
   for (int i = 0 ; i < len ; i++){
        canData[index[i]] = value[i];
   }     
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
