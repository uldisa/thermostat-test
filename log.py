import serial
import time
import sys
import decimal
ser = serial.Serial('/dev/ttyS12',9600,timeout=0)

while True:
	now=time.gmtime()
	if(time.strftime("%S",now)=="00"):
		ser.write('ping')
		data=ser.read(1000)
		a=data.rstrip('\n').split(',')
		if len(a)<2:
			continue
		output='%s'%time.strftime("%Y-%m-%d %H:%M:%S",now)
		for i in a:
			if len(i)==1:
				output+="\t%s"%i
			else:
				output+="\t%s"%float(i)
		output+='\n'
		sys.stdout.write(output)
		sys.stdout.flush()
	time.sleep(.5)
