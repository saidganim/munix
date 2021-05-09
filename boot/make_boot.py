#!/usr/bin/python2
import sys
import os
# sys.argv[1] - filename
with open(sys.argv[1], mode='r+b') as file:
    fileContent = file.read()
    size = len(fileContent)
    if (size > 510):
        print 'Too big file for booting'
        sys.exit(1)
    length = 510 - size
    fileContent = (b'\0') * (length) + b'\x55' + b'\xAA' # bootabe device
    file.write(fileContent)
    boot2 = open(sys.argv[2], mode='rb')
    boot2c = boot2.read()
    size = len(boot2c)
    file.write(boot2c) # second stage of the bootloader is just appended
    compl = 512-size%512
    file.write(compl*'\0') # filling with zeros to align
