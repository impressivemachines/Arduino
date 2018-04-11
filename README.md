# Arduino
Various small Arduino sketches

## readrom - read from EPROM
See http://simonwinder.com for details of shield.
http://simonwinder.com/2015/05/how-to-read-old-eproms-with-the-arduino/

Usage: You have to have a serial connection open at 9600 baud. The program expects a single command which is 'm'. This means list memory. For example if you have a ROM connected you might type "m 0 0400" This will list the memory bytes from location 0 in hex through 0400 in hex. If you want to just look at 16 bytes then you can leave off the end address.

## mousecam - interface with a ADNS-3080 based optical flow camera

This code reads from the camera and outputs data to the terminal. It can capture the motion vectors or else it can dump out whole frames.


