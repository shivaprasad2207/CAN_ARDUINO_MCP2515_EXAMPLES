import smbus
import time
bus = smbus.SMBus(1)


# This is the address we setup in the Arduino Program
address = 0x04

def writeNumber(myList):
        bus.write_i2c_block_data(address,1,myList)
        return -1

def readNumber():
        number = bus.read_i2c_block_data(address,4)
        return number

while True:
        myList = [0,0,0,0,0,0,0,0]
        op = raw_input ("set Led [on|off] ==>")
        if str(op) == "on" :
                myList[2] = myList[3] = myList[4] = 1
        else:
                myList[2] = myList[3] = myList[4] = 0
        writeNumber(myList)
        time.sleep(1)

