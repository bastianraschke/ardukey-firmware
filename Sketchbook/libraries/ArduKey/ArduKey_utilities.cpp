/*
 * ArduKey - A slim OTP token device based on Arduino.
 *
 * Written by Bastian Raschke <bastian.raschke@posteo.de>
 * Copyright (C) 2014 Bastian Raschke
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1) Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2) Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "ArduKey.h"
#include "ArduKey_utilities.h"

// For sprintf(), Serial:
#include <Arduino.h>


/*
 * Dumps an given array of data via Serial output.
 * 
 * @args values: The array to dump.
 * @args length: The length of the data.
 * @return void
 *
 */
void ArduKeyUtilities::serialDump(const uint8_t values[], int length)
{
    // Buffer for sprintf
    char buffer[16];

    for (int i = 0; i < length; i++)
    {
        sprintf(buffer, "0x%02X ", values[i]);
        Serial.print(buffer);
    }

    Serial.println();
}

/*
 * Calculates the CRC16 (CRC-CCITT "0xFFFF") checksum of given data.
 * (thanks to "Antonio Pires" on https://stackoverflow.com/questions/10564491
 * and "ckielstra" on http://www.ccsinfo.com/forum/viewtopic.php?t=24977)
 *
 * @args values: The array to calculate checksum of.
 * @args length: The length of processed data.
 * @return uint16_t
 *
 */
uint16_t ArduKeyUtilities::CRC16(const uint8_t values[], int length)
{
    uint8_t x;
    uint16_t crc = 0xFFFF;

    for (int i = 0; i < length; i++)
    {
        x = crc >> 8 ^ values[i];
        x ^= x >> 4;
        crc = (crc << 8) ^ ((uint16_t) (x << 12)) ^ ((uint16_t) (x << 5)) ^ ((uint16_t) x);
    }

    return crc;
}

/*
 * Converts a given array to an array that will contain the two hexadezimal
 * representation chars per char.
 * (thanks to "K-ballo" on https://stackoverflow.com/questions/10723403)
 *
 * Important:
 * The destination array must be sized N*2 + 1 in comparison to source array (N).
 * 
 * @args dst: The source array we read from.
 * @args src: The result array we write to.
 * @args srcLength: The length of src.
 * @return void
 *
 */
void ArduKeyUtilities::convertToHex(const char src[], char dst[], int srcLength)
{
    // The special transforming table
    // TODO: Own name and table
    const char table[16] =
    {
        'c', 'b', 'd', 'e', 'f', 'g', 'h', 'i',
        'j', 'k', 'l', 'n', 'r', 't', 'u', 'v',
    };

    // Source field counter
    int a = 0;

    // Destination field counter
    int b = 0;

    for (int i = 0; i < srcLength; i++)
    {
        char currentSrcByte = src[a++];

        // Important: Do not change the brackets
        dst[b++] = table[ (currentSrcByte & 0xf0) >> 4 ];
        dst[b++] = table[ (currentSrcByte & 0x0f) >> 0 ];
    }

    dst[b] = '\0';
}