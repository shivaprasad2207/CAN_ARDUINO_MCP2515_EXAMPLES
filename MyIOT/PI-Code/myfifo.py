import os, time, sys
import time
import string
import random

class MyFifo():
    def __init__(self, **kwargs ):
        self.fifoWrite =  kwargs['wFile']
        self.fifoRead = kwargs['rFile']
        self.fifoWriteAck = kwargs['wFile']+'Ack'
        self.fifoReadAck = kwargs['rFile']+'Ack'
        self.fpW = self._createFifoFiles(self.fifoWrite, "w+")
        self.fpWAck = self._createFifoFiles(self.fifoWriteAck, "a+")
        self.fpR = self._createFifoFiles(self.fifoRead, "a+")
        self.fpRAck = self._createFifoFiles(self.fifoReadAck, "w+")
        self.dataSeekLen = 100
        self.ackSeekLen = 20
        self.ack = 0
        self.readLine = ''

    def _createFifoFiles(self,file,mode):
        return open(file, mode)

    def __idGenerator(self,size=6,chars=string.ascii_uppercase + string.digits):
            size=6
            chars=string.ascii_uppercase + string.digits
            myRand = ''.join(random.choice(chars) for _ in range(size))
            return myRand

    def writeToFifo(self,data):
            msg = {}
            msg.update(data)
            msg['ACK'] = self.__idGenerator()
            formatedData =  "%s" % "-".join('%s:%s' % pair for pair in msg.items())
            if len(formatedData) > 100 :
                print ("message length above 100 character")
                #print "message length above 100 character"
                return False
            self.fpW.seek(0, 0)
            self.fpW.write(formatedData)
            self.fpW.flush()
            time.sleep(0.50)
            ack = self.fpWAck.readline()[:-1]
            confirm = False
            if ack in msg['ACK']:
                    self.fpW.seek(0)
                    self.fpW.truncate()
                    confirm = True
            return confirm

    def readFromFifo(self):
            self.fpR.seek(0,0)
            self.readLine = self.fpR.readline()[:-1]
            while 'ACK' not in  self.readLine:
                    self.readLine = self.fpR.readline()[:-1]
            words = self.readLine.split('-')
            ackS = None
            for w in words:
                    if 'ACK' in w:
                            ackS = w
            self.fpRAck.write(ackS.split(':')[1])
            self.fpRAck.flush()
            self.fpRAck.seek(0)
            self.fpRAck.truncate()
            return self.readLine

