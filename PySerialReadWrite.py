import serial
from time import sleep

ser = serial.Serial('COM4', 9600)


cmd = "CMDsetCanData:1:8:1:1:5:6:7:8:9:10:11\r"
#cmd = "CMDsetCanData:1:8:0:1:5:6:7:8:9:10:11\r"
#cmd = "CMDgetCanData\r"  # Use this command to get candata
while True:
     ser.write(cmd.encode()) # Convert the decimal number to ASCII then send it to the Arduino
     print (ser.readline()) # Read the newest output from the Arduino
     sleep(1) # Delay for one tenth of a second
