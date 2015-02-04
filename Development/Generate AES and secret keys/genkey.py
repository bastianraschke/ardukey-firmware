#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import random

if ( len(sys.argv) != 2 ):
    length = 16
else:
    length = int(sys.argv[1])

rawString = ''
arrayOutput = ''

## Generate random nonce
for _ in range(length):
    c = random.SystemRandom().randint(0, 255)
    rawString = rawString + str(hex(c))[2:]
    arrayOutput = arrayOutput + hex(c) + ', '


print('Raw string:  ' + rawString)
print('Array style: ' + arrayOutput)
