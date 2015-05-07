# Arduino
Various small Arduino sketches

## readrom - read from EPROM
See http://simonwinder.com for details of shield.

Usage: You have to have a serial connection open at 9600 baud. The program expects a single command which is 'm'. This means list memory. For example if you have a ROM connected you might do:
	m 0 0400
This will list the memory bytes from location 0 in hex through 0400 in hex. If you want to just look at 16 bytes then you can leave off the end address.


