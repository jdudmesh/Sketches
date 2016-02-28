#!/usr/bin/env python
# encoding: utf-8
"""
untitled.py

Created by John Dudmesh on 2014-04-02.
Copyright (c) 2014 __MyCompanyName__. All rights reserved.
"""

import sys
import os
import serial
import time
import json
import urllib2
import sys
import threading

url = 'http://10.0.1.100:3000/endpoint'
endpoint = "5323010a0acca9ad09000001"

OPEN = 0
CLOSED = 1

def main():
	
	ser = serial.Serial(
	    port='/dev/tty.usbmodem1d11',
	    baudrate=9600,
	    parity=serial.PARITY_NONE,
	    stopbits=serial.STOPBITS_ONE,
	    bytesize=serial.EIGHTBITS
	)
	
	if ser.isOpen() == False :
		ser.open()
	
	ser.readline()
	
	doorIsOpen = False
	
	while 1	:
		line = ser.readline()
		f = line.split(",");
		level = int( ((float(f[0]) - 510.0) / 450.0) * 100.0 )
		door = int(f[1])
				
		data = {
			"key": "$2a$08$bqaCnknRP46Mvnd9b5t.fOr1HLTvWNjjmS3lvksz6i8xUjBxFiOlm",
			"x": time.time(),
			"y": level
		}
		print data
		post = json.dumps(data)
		req = urllib2.Request("%s/%s/data" % (url, endpoint))
		req.add_header('Content-Type','application/json')
		response = urllib2.urlopen(req, post)
		print response.read()
		
		time.sleep(10)


if __name__ == '__main__':
	main()

