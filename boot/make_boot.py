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
    leng = 510 - size
    fileContent = (b'\0') * (leng) + b'\x55' + b'\xAA' # bootabe device
    file.write(fileContent)
