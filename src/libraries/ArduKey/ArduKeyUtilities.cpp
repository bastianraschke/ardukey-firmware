/*
 * ArduKey - A simple OTP device based on Arduino.
 *
 * Written by Bastian Raschke <bastian.raschke@posteo.de>
 * Copyright (C) 2015 Bastian Raschke
 * All rights reserved.
 *
 */

#include "ArduKey.h"
#include "ArduKeyUtilities.h"


/*
 * Calculate the CRC16 (ISO13239) checksum of given data.
 *
 * @args values: The array to calculate checksum of.
 * @args length: The length of processed data.
 * @return uint16_t
 *
 */
uint16_t ArduKeyUtilities::calculateCRC16(const uint8_t values[], size_t length)
{
    uint16_t crc = 0xFFFF;

    // Pointer sanity check
    if ( !values || length == 0 )
    {
        return crc;
    }

    uint16_t x;

    for (int i = 0; i < length; i++)
    {
        crc ^= values[i] & 0xFF;

        for (int j = 0; j < 8; j++)
        {
            x = crc & 1;
            crc >>= 1;

            if (x)
            {
                crc ^= 0x8408;
            }
        }
    }

    return crc;
}

/*
 * Convert a given array to an array that will contain the two hexadecimal representation chars per char.
 *
 * Important:
 * The destination array must be sized N*2 + 1 in comparison to source array (N).
 *
 * Thanks to: "K-ballo" on https://stackoverflow.com/questions/10723403
 *
 * @args dst: The source array we read from.
 * @args src: The result array we write to.
 * @args srcLength: The length of src.
 * @return void
 *
 */
void ArduKeyUtilities::encodeArduHex(const char src[], char dst[], size_t srcLength)
{
    // Pointer sanity check
    if ( !src || !dst || srcLength == 0 )
    {
        return ;
    }

    // Mapping (hexadecimal -> arduhex) table
    const char table[16] =
    {
        'c', 'b', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'n', 'r', 't', 'u', 'v',
    };

    // Source field counter
    int a = 0;

    // Destination field counter
    int b = 0;

    for (int i = 0; i < srcLength; i++)
    {
        char currentSrcByte = src[a++];

        // Important: Do not change the brackets!
        dst[b++] = table[ (currentSrcByte & 0xf0) >> 4 ];
        dst[b++] = table[ (currentSrcByte & 0x0f) >> 0 ];
    }

    // Add Null-Byte at last position
    dst[b] = '\0';
}

/*
 * Dump a given array of data via serial output.
 *
 * @args values: The array to dump.
 * @args length: The length of the array.
 * @return void
 *
 */
void ArduKeyUtilities::serialDump(const uint8_t values[], size_t length)
{
    // Pointer sanity check
    if ( !values || length == 0 )
    {
        return ;
    }

    // Buffer for sprintf
    char buffer[16];

    for (int i = 0; i < length; i++)
    {
        sprintf(buffer, "%02X ", values[i]);
        Serial.print(buffer);
    }

    Serial.println();
}
