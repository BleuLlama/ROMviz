ROMviz
======

This repository contains tools for dumping and visualizing EPROMS.

Contained in here are the following components:

1. Arduino sketch for dumping an EPROM (2708 thru 27512 parts) to Serial (1 kbyte - 64 kbyte)
2. Arduino circuit diagram for creating the EPROM shield
3. Tool for taking the above serial dump and converting it into a binary ROM-image file
4. Tool for converting binary ROM-image files into picture files (GIF)
5. Tool for converting picture files (GIF) into binary ROM-image files.

# Arduino Stuff

The theory of operation for the shield is basically this:

Since we're not going to be doing random accesses to the EPROM, we
can take a shortcut here, by using adder chips.  There are 4 4bit
adders (in two chips) which cascade from one to the next, so once
one reaches 15 and increments again, that one resets to 0, and the
next one cascaded increments.  The outputs of the adders are connected
to the address lines of the EPROM socket. The data lines of the
EPROM are connected directly to data inputs on the Arduino.

1. The sketch will hit the "reset" line of the adders, resetting the address to 0
2. Send down a sentinel via serial ".oOo.oOo."
3. Send down a unique number (milliseconds since poweron)
3. Send down the number of bytes via serial (number of bytes in hex)
4. Next, we will loop for the appropriate number of bytes for the EPROM:
  1. Read in the bits from the Data output of the EPROM
  2. Constitute them back into a single byte
  3. Send the byte (as ascii) via serial

This will dump out the entire ROM to the serial connection.

The code for this is in the "Arduino/ROM_Reader" folder.  Just
double-click the .ino file contained within and it will open it up,
ready to be programmed to a device.

## Parts required for construction:

1. Arduino board of some kind
2. two 74ls393 four bit adder ICs with sockets
3. 28 pin EPROM socket
4. Perfboard, Headers, (or prototyping shield, etc)
5. LEDs, 470 ohm resistors
6. wire and patience

The schematic diagrams for how to hook everything up are included in
the "Arduino/schematics" folder.


- 

# Desktop scripts

These are scripts to be run on your host computer to convert files around
in different ways.

There are a few prerequisites for using these:

- perl (scripts are written in perl)
- ImageMagick tools ("convert") (image conversions)

They have only been tested on Mac OS X 10.7 and 10.8.

## txt2bins.pl - Serial log to binary converter

The first of the scripts (txt2bins.pl) will take the output from
the above (it reads in a log file of the dumped text) and detect
where the rom images are, and will reconstitute the binary ROM image
files, whose filenames are based on the input text file, and the
millisecond time.

Usage:  perl txt2bins.pl serialLog.txt


## Image-based conversion tools

The basic process for both of these tools is the same.  Both of
them will work with an image file (GIF used, for ease of animations).
Both of them work with ROM files that are bit-for-bit the same as 
what was in the EPROM chip itself.  

The EPROM data is packed 8 bits per byte.  The GIF data is unpacked
to be one bit per pixel (one bit per byte, as it were), white or
black, indicating a 1 or a 0 in the original EPROM.

So if there are 100 bytes of data in the ROM, there are 100*8 or
800 pixels on the display.


### bin2gif.pl - Convert Binary ROM files to GIF images

This perl script will take the ROM image binary files and convert
them to be a 1 bit (black/white) GIF image file of appropriate
dimensions.  One 8-bit byte read in will become 8 horizontal pixels.

It will then convert them all into one animated gif file.

Usage:  perl bin2gif.pl file1.bin file2.bin ...


### gif2bin.pl - Convert GIF images to Binary ROM files

This perl script reads in one of these GIF files (or one of your
creation) of the appropriate size (unchecked) and will save out a
binary ROM image file, suitable to be burned back to an EPROM device.

Note: this might work with file formats other than GIF, but that
is untested.

Usage: gif2bin.pl file1.gif file2.gif ...
