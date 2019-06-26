import myfifo
f = myfifo.MyFifo( rFile='wFile', wFile='rFile' )
while True:
        m = input("Enter msg------------------------------->>>>>>>>>]] $")
        if f.writeToFifo({'msg':m}):
                print ("Data written to fifo")
        else:
                print ("Data Not written to fifo")
        print (f.readFromFifo())