import time
import serial as sl

ser=sl.Serial("/dev/ttyACM0",9600)
ser.reset_input_buffer()

while(True):
	try:
	#----------RECIEVE DATA----------
		if ser.in_waiting>0:
			recieve=ser.readline().decode('utf-8').rstrip()

	#----------SEND DATA----------
		value=recieve
		ser.write(bytearray(f"{value}\n",encoding='utf-8'))
		time.sleep(0.1)
	except Exception as e:
		print(e)
