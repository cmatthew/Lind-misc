#!/usr/bin/python2.6

import sys

def main():
	
	if(sys.argv[1] == "" or sys.argv[2] == ""):
		print "usage: ./addTimes.py <filename> <show CMD names 1 or 0>"
		exit(0)
	
	showCMD = int(sys.argv[2])
		
	f = open(sys.argv[1],'r')
	
	total = 0
	cmdList = []
	time = '';
	for line in f.readlines():
		if not line.strip() == "=====":
			if line[0] == 't':
				time = line[1:]
			else:
				lineSplit = line.split()
				if not lineSplit[0] == "":
					total += int(lineSplit[0])
				if not lineSplit[1] == "":
					cmdList.append(lineSplit[1] + ":::" + lineSplit[2])

			
		else:
			if not total == 0:
				if showCMD:
					print "Time: " + repr(total) + ", commands: " + repr(cmdList)
				else:
					print repr(total) + "," +time.strip()
			total = 0
			cmdList = []
			time=""
		



if __name__ == "__main__":
	main()
