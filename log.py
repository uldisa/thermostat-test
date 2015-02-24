import serial
import time
import sys
import decimal
#ser = serial.Serial('/dev/ttyS12',9600,timeout=0)
ser = serial.Serial('/dev/ttyS8',9600,timeout=0)
last=None

while True:
	now=time.gmtime()
	if(now != last):
		#if(time.strftime("%S",now).endswith("5")):
		if(int(time.strftime("%S",now))%1 == 0):
			ser.write('ping')
			data=''
			for i in 'kartupelis':
				data+=ser.read(1000)
				if(data == '' or data.endswith('\n')):
					break
				
			if(not data.endswith('\n')):
				continue
			a=data.rstrip('\n').split(',')
			if len(a)<2:
				continue
			last=now
			output='%s'%time.strftime("%Y-%m-%d %H:%M:%S",now)
			for i in a:
				if len(i)==1:
					output+="\t%s"%i
				else:
					try:
						output+="\t%s"%float(i)
					except:
						sys.stdout.write("Error converting <%s>\n"%i)
						output+="\t%s"%i

			output+='\n'
			sys.stdout.write(output)
			sys.stdout.flush()
	time.sleep(.5)
