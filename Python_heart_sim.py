import serial
import time
from threading import Timer
import schedule
import msvcrt

file_path = 'Test_data/moving_9600.txt'
send_rate_in_seconds = 0.1
receive_rate_in_seconds = 1
ser = serial.Serial("COM7", 9600)

def send_data():
	line = file.readline()
	if not line:
		ser.write("Could not Read Line from text\n")
	# print ("Sending " + line)
	ser.write(line.encode())

def receive_data():
    print("Arduino Sent")
    num_bytes_in_waiting = ser.in_waiting
    if (num_bytes_in_waiting > 0):
    	try:
    		print (str(ser.read(num_bytes_in_waiting).decode()))
    	except Exception as e:
    		print(e)
    		pass

file = open(file_path)
schedule.every(send_rate_in_seconds).seconds.do(send_data)
schedule.every(receive_rate_in_seconds).seconds.do(receive_data)

while 1:
	schedule.run_pending()
	if msvcrt.kbhit():
		if ord(msvcrt.getch()) == 27:
		    break

print("Exiting")
file.close()
ser.close()