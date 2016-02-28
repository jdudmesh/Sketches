#! /usr/bin/python

import json
import urllib2
import time
import sys
import fs

url = 'http://10.0.1.100:3000/endpoint'
endpoint = "5323010a0acca9ad09000001"
req = None

def main(argv):
	try:
		data = {
		
			"key": "$2a$08$bqaCnknRP46Mvnd9b5t.fOr1HLTvWNjjmS3lvksz6i8xUjBxFiOlm",
			"x": time.time(),
			"y": argv[0]
		}
		
		req = urllib2.Request("%s/%s/data" % (url, endpoint))
		req.add_header('Content-Type','application/json')

		post = json.dumps(data)
		#print post
		response = urllib2.urlopen(req, post)
		
		f = open("/mnt/sd/arduino/res", "w")
		f.write(response)
		f.close()

		print 1
	
	except:
	
		print 0

main(sys.argv[1:])

# curl -v X POST -d '{ "key":"$2a$08$bqaCnknRP46Mvnd9b5t.fOr1HLTvWNjjmS3lvksz6i8xUjBxFiOlm", "x":1395744894.81, "y":50}' -H "Content-Type: application/json" "http://feedrpoint.com/endpoint/5323010a0acca9ad09000001"