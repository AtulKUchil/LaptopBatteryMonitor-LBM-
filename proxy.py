import serial
import subprocess
import os
import time

s = serial.Serial('/dev/ttyUSB0',baudrate=9600,timeout=1)
#process = subprocess.Popen(['acpi','|','cut','-d',' ','-f','4','|','cut','-c','1-2'],stdout=subprocess.PIPE)
#process = subprocess.Popen(['acpi'],stdout=subprocess.PIPE)
#stdout = process.communicate()[0]
#print(stdout)
#output = process.stdout.readline()
#output = os.popen("acpi | cut -d ' ' -f 4 | cut -c 1-2").read()
while True:
	output = os.popen("acpi | cut -d ' ' -f 4").read()
	n = len(output)
	if n==5:
		output = output[0:2]
	else:
		output = output[0:3]
	out = output.encode('ascii')
	s.write(out)
	time.sleep(60)



