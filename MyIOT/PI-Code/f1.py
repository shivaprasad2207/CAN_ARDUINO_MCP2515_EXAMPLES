import myfifo


f = myfifo.MyFifo(rFile='rFile', wFile='wFile')
while True:
        print (f.readFromFifo())
        m = input("Enter msg------------------------------->>>>>>>>>]] $")
        if f.writeToFifo({'msg':m}):
                print ("Data written to fifo")
        else:
                print ("Data Not written to fifo")
