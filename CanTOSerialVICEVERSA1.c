
#include <mcp_can.h>
#include <SPI.h>

int incomingByte = 0;
char inputStringBuf[100];
char cmdBuf[100] ;
int inputStringLen = 0;
char *cmnds[] = {"getCanData","setCanData"}; 
char *cmdPrompt = "CMD";
int cmdPromptLen = 3 ;
int cmdsLen = 10;
int RWFLAG = 0;
int WFLAG = 1;
int RFLAG = 2;
int canSendACK = 0;
char delemeter = ':';
int canPocketLen = 34;
char canPocket[34] = {0};
int canDataStrIdx = 13;
int canId = 0;
int canLen = 0;
byte canDataByte[8] = {0};
unsigned char canDataRscv [8] = {0};
unsigned long int canIDrscv = 0  ;
unsigned char canLenRscv1;
unsigned char canLenRscv = 8;
char sendCanPocket[34] = {0};

MCP_CAN CAN0(10); // Set CS to pin 10

void setup() {
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps

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
  
}


void loop(){
  int ret;
  char localBuf[250] = {0};
  readSerialData();
  if (inputStringLen > cmdPromptLen){ 
     ret  = analyseSerialData();
    switch (ret){
      case 7: Serial.println("CMD len les then 3");
               break;   
      case 6: Serial.println("CMD operation not defined one ");
               break;   
      case 0: RWFLAG = WFLAG;
               break;   
      case 1:  RWFLAG = RFLAG;
               readCanDataFromSerial();
               writeCanDataToCanBus(); 
               sprintf(localBuf,"Can Send Return Value %d", canSendACK);           
               Serial.println(localBuf );
               break;   
      case 2: Serial.println(" Not able to analyse the defined CMD OP " );
               break;
      case 3: Serial.println(" Can Data not rightly Formated " );
               break;      
      default: Serial.println(" I am not expecting anything here" );
               break;             
    }
  }else{
      if (RWFLAG == WFLAG){
          writeCanDataToSerial();
      }else if (RWFLAG == RFLAG){
        Serial.println(" Here canSendACK data should be shown." );  
        RWFLAG = 0;
      }else{
         Serial.println(" Can Shell Prompt#>" );  
      } 
   }
  delay(1000);
}


void writeCanDataToCanBus(){
  //for (int i = 0 ; i < 8 ; i++)  Serial.println(canDataByte[i]);
  canSendACK = CAN0.sendMsgBuf(canId, 0, canLen, canDataByte); 
}

void writeCanDataToSerial(){
  readCanDataFromCanBus();
  Serial.println(sendCanPocket);
}

 void readCanDataFromCanBus(){ 
    CAN0.readMsgBuf(&canIDrscv, &canLenRscv, canDataRscv,&canLenRscv1);  
    sprintf (sendCanPocket,":%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:%d",canIDrscv,canLenRscv,canLenRscv1,canDataRscv[0],canDataRscv[1],
                                                            canDataRscv[2],canDataRscv[3],canDataRscv[4],canDataRscv[5],
                                                            canDataRscv[6],canDataRscv[7]
                                                            );
 }

void readCanDataFromSerial(){
   int j = 0 ;
  for (int i = canDataStrIdx; i <  canPocketLen ; i++,j++)
    canPocket[j] = inputStringBuf[i];
  canPocket[j] = 0; 
  formatCanData();
}

void formatCanData ( ){
  int canDataInt[8] = {0}; 
  sscanf(canPocket,":%d:%d:%d:%d:%d:%d:%d:%d:%d:%d:",&canId,&canLen,&canDataInt[0],&canDataInt[1],
                        &canDataInt[2],&canDataInt[3],&canDataInt[4],&canDataInt[5],&canDataInt[6],
                        &canDataInt[7]);
  //Serial.println(canId);
  //Serial.println(canLen);
  for (int i = 0; i < 8 ; i++)   canDataByte[i] = (byte)canDataInt[i] ;
}


void readSerialData() {
  int i = 0;
  while(Serial.available() > 0) {
    incomingByte = Serial.read(); // read the incoming byte:
    inputStringBuf[i++] = incomingByte;
  }
  inputStringLen = i-1;
}


int analyseSerialData(){
   int i = 0;
   int j = 0;
   int cntr = 0;
   int byteCnt = 0;
   int byteIdx = 0;
   char cmdString[11] = {0};
   char buf[100] = {0};
   
   for ( i = 0 ; i < cmdPromptLen ; i++)
    if  (inputStringBuf[i] != cmdPrompt[i])
      return 7;
   
   for ( j = 0 ; j < cmdsLen ; j++)
    cmdString[j] = inputStringBuf[i++]  ; 
   
   if ( j != cmdsLen )
      return 6;
   cmdString[j] = 0;
   
   if (strcmp(cmnds[0], cmdString) == 0)
    return 0;
   else if (strcmp(cmnds[1], cmdString) == 0){    
    return 1;
   }
   else
    return 2;
}