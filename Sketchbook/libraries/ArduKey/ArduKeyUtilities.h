/*
 * ArduKey - A slim OTP token device based on Arduino.
 *
 * Written by Bastian Raschke <bastian.raschke@posteo.de>
 * Copyright (C) 2014 Bastian Raschke
 * All rights reserved.
 *
 */

#ifndef __ARDUKEY_UTILITIES_H__
#define __ARDUKEY_UTILITIES_H__


class ArduKeyUtilities
{
    public:

        static uint16_t CRC16(const uint8_t values[], size_t length);
        static void convertToHex(const char src[], char dst[], size_t srcSize);
        static void serialDump(const uint8_t values[], size_t length);

};


#endif
