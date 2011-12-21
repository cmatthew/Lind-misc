#!/bin/bash
set -o xtrace
python2.6 $REPY_PATH/repy.py --safebinary $REPY_PATH/restrictions.lind $REPY_PATH/lind_server.py $@
