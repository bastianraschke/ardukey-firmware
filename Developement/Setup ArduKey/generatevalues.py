#!/usr/bin/env python

import random


aeskey = ""

for i in range(0, 16):
    aeskey += "0x%02X, " % random.randrange(0xFF)

print(aeskey[:-2])


publicId = ""

for i in range(0, 6):
    publicId += "0x%02X, " % random.randrange(0xFF)

print(publicId[:-2])


secretId = ""

for i in range(0, 6):
    secretId += "0x%02X, " % random.randrange(0xFF)

print(secretId[:-2])
