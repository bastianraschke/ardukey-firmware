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
#include "ArduKey_eeprom.h"

// For sprintf(), Serial:
#include <Arduino.h>
#include <avr/eeprom.h>


/*
 * Dumps the complete EEPROM as hex output via Serial.
 * Thanks to http://playground.arduino.cc/Code/EepromUtil
 * 
 * @return void
 *
 */
void ArduKeyEEPROM::dumpEEPROM()
{
    // The byte counter in a row
    int x = 0;

    uint8_t currentByte;
    char buffer[16];

    for (int n = EEPROM_MIN_ADDRESS; n <= EEPROM_MAX_ADDRESS; n++)
    {
        // Prints the address if it is the first byte in a row
        if (x == 0)
        {
            sprintf(buffer, "%03X: ", n);
            Serial.print(buffer);
        }

        // Reads one byte and write to buffer
        currentByte = eeprom_read_byte((uint8_t *) n);
        sprintf(buffer, "%02X ", currentByte);

        x++;

        if (x == 16)
        {
            x = 0;
            Serial.println(buffer);
        }
        else
        {
            Serial.print(buffer);
        }
    }
}

/*
 * Checks if given address is in a valid range.
 * 
 * @param address The address to check.
 * @return bool
 *
 */
bool ArduKeyEEPROM::isAddressOkay(int address)
{
    return (address >= EEPROM_MIN_ADDRESS && address <= EEPROM_MAX_ADDRESS);
}

/*
 * Reads a given count of bytes from EEPROM.
 *
 * Note: We use the passed pointer style (and not a uint8_t buffer[]) to provide 2 methods:
 * 1) Write to a passed uint8_t buffer[]
 * 2) Write to a unsigned int variable which consists of more than one byte.
 * 
 * @param address The address we start reading from.
 * @param ptr The pointer or array we write to.
 * @param length The length of the data.
 * @return bool
 *
 */
bool ArduKeyEEPROM::getBytes(int address, uint8_t* ptr, int length)
{
    if ( !ArduKeyEEPROM::isAddressOkay(address) || !ArduKeyEEPROM::isAddressOkay(address + length) )
    {
        return false;
    }

    for (int i = 0; i < length; i++)
    {
        // The address passed to eeprom_x_byte() is a normal integer
        // and has to be casted to "real address type" (represented via byte pointer).
        *ptr++ = eeprom_read_byte((uint8_t *) (address + i));
    }

    return true;
}

/*
 * Writes the given array to position (starting at given address) to EEPROM.
 * 
 * Note: We use the passed pointer style (and not a uint8_t buffer[]) to provide 2 methods:
 * 1) Read from a passed uint8_t buffer[]
 * 2) Read from a unsigned int variable which consists of more than one byte.
 *
 * @param address The address we start writing to.
 * @param ptr The pointer or array we read from.
 * @param length The length of the data.
 * @return bool
 *
 */
bool ArduKeyEEPROM::setBytes(int address, const uint8_t* ptr, int length)
{
    if ( !ArduKeyEEPROM::isAddressOkay(address) || !ArduKeyEEPROM::isAddressOkay(address + length) )
    {
        return false;
    }

    if ( !ptr )
    {
        return false;
    }

    for (int i = 0; i < length; i++)
    {
        // The address passed to eeprom_x_byte() is a normal integer
        // and has to be casted to "address type".
        eeprom_write_byte((uint8_t *) (address + i), *ptr++);
    }

    return true;
}

/*
 * Reads the AES key from EEPROM.
 * 
 * @param buffer The buffer array we write to.
 * @return bool
 *
 */
bool ArduKeyEEPROM::getAESKey(uint8_t buffer[AES_KEYSIZE])
{
    return ArduKeyEEPROM::getBytes(EEPROM_AESKEY_POS, buffer, EEPROM_AESKEY_LEN);
}

/*
 * Writes a new AES key to EEPROM.
 * 
 * @param values The array of bytes we write to EEPROM.
 * @return bool
 *
 */
bool ArduKeyEEPROM::setAESKey(uint8_t values[AES_KEYSIZE])
{
    return ArduKeyEEPROM::setBytes(EEPROM_AESKEY_POS, values, EEPROM_AESKEY_LEN);
}

/*
 * Reads the public id from EEPROM.
 * 
 * @param buffer The buffer array we write to.
 * @return bool
 *
 */
bool ArduKeyEEPROM::getPublicId(uint8_t buffer[ARDUKEY_PUBLICID_SIZE])
{
    return ArduKeyEEPROM::getBytes(EEPROM_PUBLICID_POS, buffer, EEPROM_PUBLICID_LEN);
}

/*
 * Writes a new public id to EEPROM.
 * 
 * @param values The array of bytes we write to EEPROM.
 * @return bool
 *
 */
bool ArduKeyEEPROM::setPublicId(uint8_t values[ARDUKEY_PUBLICID_SIZE])
{
    return ArduKeyEEPROM::setBytes(EEPROM_PUBLICID_POS, values, EEPROM_PUBLICID_LEN);
}

/*
 * Reads the secret id from EEPROM.
 * 
 * @param buffer The buffer array we write to.
 * @return bool
 *
 */
bool ArduKeyEEPROM::getSecretId(uint8_t buffer[ARDUKEY_SECRETID_SIZE])
{
    return ArduKeyEEPROM::getBytes(EEPROM_SECRETID_POS, buffer, EEPROM_SECRETID_LEN);
}

/*
 * Writes a new secret id to EEPROM.
 * 
 * @param values The array of bytes we write to EEPROM.
 * @return bool
 *
 */
bool ArduKeyEEPROM::setSecretId(uint8_t values[ARDUKEY_SECRETID_SIZE])
{
    return ArduKeyEEPROM::setBytes(EEPROM_SECRETID_POS, values, EEPROM_SECRETID_LEN);
}

/*
 * Gets the current counter value from EEPROM.
 * 
 * @return uint16_t
 *
 */
uint16_t ArduKeyEEPROM::getCounter()
{
    uint16_t value; // 2 Bytes
    uint8_t* ptr = (uint8_t*) &value; // 1 Byte

    ArduKeyEEPROM::getBytes(EEPROM_COUNTER_POS, ptr, EEPROM_COUNTER_LEN);
    return value;
}

/*
 * Writes the new counter value to EEPROM.
 * 
 * @param value The new counter value.
 * @return void
 *
 */
void ArduKeyEEPROM::setCounter(uint16_t value)
{
    // uint16_t value // 2 Bytes
    uint8_t* ptr = (uint8_t*) &value; // 1 Byte

    ArduKeyEEPROM::setBytes(EEPROM_COUNTER_POS, ptr, EEPROM_COUNTER_LEN);
}