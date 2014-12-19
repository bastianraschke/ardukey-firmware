/*
 * ArduKey - A simple OTP device based on Arduino.
 *
 * Written by Bastian Raschke <bastian.raschke@posteo.de>
 * Copyright (C) 2014 Bastian Raschke
 * All rights reserved.
 *
 */

#include "ArduKey.h"
#include "ArduKeyUtilities.h"


/*
 * Calculates the CRC16 (CRC-CCITT "0xFFFF") checksum of given data.
 *
 * Thanks to: "Antonio Pires" on https://stackoverflow.com/questions/10564491
 * Thanks to: "ckielstra" on http://www.ccsinfo.com/forum/viewtopic.php?t=24977
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

    uint8_t x;

    for (int i = 0; i < length; i++)
    {
        x = (crc >> 8) ^ values[i];
        x = x ^ (x >> 4);

        crc = (crc << 8) ^ (x << 12) ^ (x << 5) ^ x;
        crc = crc & 0xFFFF;
    }

    return crc;
}

/*
 * Converts a given array to an array that will contain the two hexadezimal representation chars per char.
 *
 * Thanks to: "K-ballo" on https://stackoverflow.com/questions/10723403
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

        // Important: Do not change the brackets
        dst[b++] = table[ (currentSrcByte & 0xf0) >> 4 ];
        dst[b++] = table[ (currentSrcByte & 0x0f) >> 0 ];
    }

    // Add Null-Byte at last position
    dst[b] = '\0';
}

/*
 * Dumps an given array of data via Serial output.
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
