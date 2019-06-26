import os, time, sys
from flask import Flask
from flask import request
import time
import myfifo
import json

f = myfifo.MyFifo(rFile='rFile', wFile='wFile')


data = {
        'OP':'',
        'RESRC':'',
        'RESRC_ID':'',
        'DATA': {}
      }

app = Flask(__name__)

def writeToI2C():
        f.writeToFifo(data)


def readFromI2C():
        pass

@app.route("/iot/led/<string:ledId>", methods=['GET','PUT'])
def led(ledId):
        print(json.dumps(request.data.decode("utf-8"), indent=4, sort_keys=True))
        data['OP'] = request.method
        data['RESRC'] = 'led'
        data['RESRC_ID'] = ledId
        if request.method == 'PUT':
                dataDict = json.loads(request.data)
                data['DATA'] = dataDict['action']
                print ("I am Here")
                if writeToI2C():
                        return "Led Data Sent"
                else:
                        return "Led Data NOT Sent"
        else:
                if writeToI2C():
                        return "Led Data Sent"
                else:
                        return "Led Data NOT Sent"


@app.route("/iot/fan/<string:fanId>", methods=['GET','PUT'])
def fan(fanId):
        print(json.dumps(request.data.decode("utf-8"), indent=4, sort_keys=True))
        data['OP'] = request.method
        data['RESRC'] = 'fan'
        data['RESRC_ID'] = fanId
        if request.method == 'PUT':
                dataDict = json.loads(request.data)
                data['DATA'] = dataDict['action']
                print ("I am Here")
                if writeToI2C():
                        return "Fan Data Sent"
                else:
                        return "Fan Data NOT Sent"
        else:
                if writeToI2C():
                        return "Fan Data Sent"
                else:
                        return "Fed Data NOT Sent"




if __name__ == "__main__":
        app.run(debug=True)
        #app.run(debug=True,host='192.168.1.7', port=5001)