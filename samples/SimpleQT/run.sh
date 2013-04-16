#!/bin/bash

#You need admin account(password)` to access libUSB
#export LD_LIBRARY_PATH=/your_libapbase.so_and_libmidlib2.so_directory
#./SimpleQt&

#sample bash script
echo $LD_LIBRARY_PATH
env LD_LIBRARY_PATH=./:$LD_LIBRARY_PATH ./SimpleQt&
