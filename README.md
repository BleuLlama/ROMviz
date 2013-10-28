ROMviz
======

This repository contains tools for dumping and visualizing EPROMS.

Contained in here are the following components:

1. Arduino circuit diagram for creating the EPROM shield
2. Arduino sketch for dumping an EPROM (2708 thru 27512 parts) to Serial (1 kbyte - 64 kbyte)
3. Tool for taking the above serial dump and converting it into a binary ROM-image file
4. Tool for converting binary ROM-image files into picture files (GIF)
5. Tool for converting picture files (GIF) into binary ROM-image files.

# Arduino Stuff

The theory of operation for the shield is basically this:

Since we're not going to be doing random accesses to the EPROM, we can take a shortcut here, by using adder chips.
There are 4 4bit adders (in two chips) which cascade from one to the next, so once one reaches 15 and increments again,
that one resets to 0, and the next one cascaded increments.  The outputs of the adders are connected to the address
lines of the EPROM socket. The data lines of the EPROM are connected directly to data inputs on the Arduino.

1. The sketch will hit the "reset" line of the adders, resetting the address to 0
2. Send down a sentinel via serial ".oOo.oOo."
3. Send down a unique number (milliseconds since poweron)
3. Send down the number of bytes via serial (number of bytes in hex)
4. Next, we will loop for the appropriate number of bytes for the EPROM:
  1. Read in the bits from the Data output of the EPROM
  2. Constitute them back into a single byte
  3. Send the byte (as ascii) via serial

This will dump out the entire ROM to the serial connection.

# Desktop scripts

## Serial Converter

The first of the scripts (named x) will take the output from the above (it reads in a log file of the dumped text)
and detect where the rom images are, and will reconstitute the binary ROM image files, whose filenames are based
on the input text file, and the millisecond time.

## Image conversion tools

There are three scripts here.  

One will take the ROM image binary files and convert them to be a 1 bit (black/white) GIF image
file of appropriate dimensions.  One 8-bit byte read in will become 8 horizontal pixels.

The next script will take a series of these and convert them to be an animated GIF file.

The final script will read in one of these GIF files (or one of your creation) of the appropriate size (unchecked)
and will save out a binary ROM image file, suitable to be burned back to an EPROM device.


