#!/usr/bin/python2.6

import sys

def main():
	
	#argument 1 is the filename you want to add the times for
	#argument 2 is either 1 or 0. If it is 1, then you will see WHAT COMMANDS were retrieved from the ps call
	#so you would see something like:
	#    30000 python2.6 python2.6 etc
	#*************THE SECOND ARGUMENT SHOULD BE 0 IF YOU PLAN ON PUTTING THE DATA INTO MATLAB***************
	if(sys.argv[1] == "" or sys.argv[2] == ""):
		print "usage: ./addTimes.py <filename> <show CMD names 1 or 0>"
		exit(0)
	
	showCMD = int(sys.argv[2])
		
	f = open(sys.argv[1],'r')#who needs error checking?
	
	total = 0
	cmdList = []#will hold the commands of the current PS (ONLY IF arg2 = 1)!!
	time = '';#will hold the start time
	for line in f.readlines():
		#I deliminate ps' by 5 ='s, so once we see the deliminator, we know we have a new ps
		if not line.strip() == "=====":
			#timestamps start with t, if it is anyting else, that means we are showing commands
			if line[0] == 't':
				time = line[1:]
			else:
				#split the command, add to list
				lineSplit = line.split()
				if not lineSplit[0] == "":
					total += int(lineSplit[0])
				if not lineSplit[1] == "":
					cmdList.append(lineSplit[1] + ":::" + lineSplit[2])

		#once we see the =====, we want to print out the total of the previous ps
		else:
			#dont worry if total is 0
			if not total == 0:
				#if we are showing commands, we want to show the total, and then the processes that made up that total
				if showCMD:
					print "Time: " + repr(total) + ", commands: " + repr(cmdList)
				#else just print the total time and start time!
				else:
					print repr(total) + "," +time.strip()
			#reset values
			total = 0
			cmdList = []
			time=""
		



if __name__ == "__main__":
	main()
