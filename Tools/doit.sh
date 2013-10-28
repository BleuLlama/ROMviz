#!/bin/sh
# this works on Mac
# copy from the monitor window to your copy buffer # then run this

pbpaste > roms.txt
perl Tools/txt2bins.pl roms.txt
perl Tools/bin2gif.pl *.bin
convert -delay 10 -loop 0 *.gif Animated.gif
open -a Safari Animated.gif

