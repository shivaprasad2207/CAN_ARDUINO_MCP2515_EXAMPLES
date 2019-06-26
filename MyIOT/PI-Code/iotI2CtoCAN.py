import myfifo
#import smbus
#import time
#bus = smbus.SMBus(1)


LED_RESRC_INDEX = 1
FAN_RESRC_INDEX = 2

GET_OP_ID = 1
PUT_OP_ID = 2

# This is the address we setup in the Arduino Program
address = 0x04

f = myfifo.MyFifo( rFile='wFile', wFile='rFile' )

canId = 1
dataD = {}
myList = [0,0,0,0,0,0,0,0]

def writeNumber():
        try:
                #bus.write_i2c_block_data(address,1,myList)
                print (myList)
        except IOError:
                pass
        return -1

def readNumber():
        #number = bus.read_i2c_block_data(address,4)
        #return number
        pass

def getLedStatus ( id, status=None ):
        myList[2] = LED_RESRC_INDEX
        myList[3] = GET_OP_ID
        myList[4] = id
        print ("getting satus of led id " + str(id))
        print (myList)

def putLedStatus ( id , status):
        myList[2] = LED_RESRC_INDEX
        myList[3] = PUT_OP_ID
        myList[4] = int(id)
        if status == "ON":
                myList[5] = 1
        else:
                myList[5] = 0

        myList[6] = 1
        print ("setting status of led id to " + str(id) + status)
        print (myList)
        writeNumber()

def getFanStatus ( id ,status=None):
        myList[2] = FAN_RESRC_INDEX
        myList[3] = GET_OP_ID
        myList[4] = id
        print ("getting satus of Fan id " + str(id))
        print (myList)

def putFanStatus(id , status):
        myList[2] = FAN_RESRC_INDEX
        myList[3] = PUT_OP_ID
        myList[4] = int(id)
        if status == "ON":
                myList[5] = 1
        else:
                myList[5] = 0
        myList[6] = 1
        print ("setting status of led id to " + str(id) + status)
        print (myList)
        writeNumber()

dispatchData = {
                "led":{
                        "GE":getLedStatus,
                        "PUT":putLedStatus
                        #"PU":putLedStatus
                      },
                "fan":{
                        "GE":getFanStatus,
                        "PUT":putFanStatus
                        #"PUT":putFanStatus
                      }
                }

def dispatchFunction(data):
        print (data['RESRC'])
        print(data['OP'])
        dispatchData[data['RESRC']][data['OP']](data['RESRC_ID'],data['DATA'])


while True:
        data =  f.readFromFifo()
        for words in data.split('-'):
                word = words.split(':')
                dataD[word[0]] = word[1]
        dispatchFunction(dataD)
