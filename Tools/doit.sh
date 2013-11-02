#!/bin/sh
# this works on Mac
# copy from the monitor window to your copy buffer # then run this

#pbpaste > roms.txt
mkdir $1
cp $1.txt $1
cd $1
../../Tools/txt2bins.pl $1.txt
../../Tools/cullbins.pl
../../Tools/bin2gif.pl *.bin
convert -delay 50 -loop 0 *.gif $1.50.gif
cp $1.50.gif ~/Dropbox/dumps
convert -delay 10 -loop 0 *.gif $1.10.gif
cp $1.10.gif ~/Dropbox/dumps

