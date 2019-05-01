#include <SPI.h>
#include <mcp_can.h>

const int spiCSPin = 10;
int ledHIGH    = 1;
int ledLOW     = 0;

//MCP_CAN CAN(spiCSPin);

MCP_CAN CAN0(spiCSPin);     // Set CS to pin 10

void setup()
{
    Serial.begin(115200);

    if(CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK) Serial.println("MCP2515 Initialized Successfully!");
    else Serial.println("Error Initializing MCP2515...");
    Serial.println("CAN BUS Shield Init OK!");
}

unsigned char stmp[8] = {ledHIGH, 1, 2, 3, ledLOW, 5, 6, 7};
    
void loop()
{   
  Serial.println("In loop");
  CAN0.sendMsgBuf(0x43, 0, 8, stmp);
  delay(1000);
}