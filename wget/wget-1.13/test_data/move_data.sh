#!/bin/bash


cat $1 | grep time | cut -d : -f 2 > data$1
scp data$1 andib@u-puppy.csc.uvic.ca:~/test_data_lind/.

