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

#include <ArduKey.h>


/*
 * Contains all ArduKey setup processes.
 * 
 * @return void
 *
 */
void setupArduKey()
{
    const uint8_t aesKey[AES_KEYSIZE] =
    {
        0x7A, 0x18, 0x58, 0x59, 0x2F, 0xCB, 0x76, 0xBD, 0x5E, 0xB2, 0x68, 0x54, 0x21, 0xAE, 0xD4, 0x5E
    };

    const uint8_t publicId[ARDUKEY_PUBLICID_SIZE] =
    {
        0x0A, 0x61, 0xF3, 0x2C, 0x5D, 0xB4
    };

    const uint8_t secretId[ARDUKEY_SECRETID_SIZE] =
    {
        0xB0, 0xD4, 0xA2, 0xD6, 0x9B, 0xC4
    };

    Serial.println("Writing new values...");
    Serial.println();

    // Writes the AES key to EEPROM
    if ( ArduKeyEEPROM::setAESKey(aesKey) == true )
    {
        Serial.println("The new AES key:");
        ArduKeyUtilities::serialDump(aesKey, AES_KEYSIZE);
        Serial.println();
    }
    else
    {
        Serial.println("Error: The AES key could not be written!");
    }

    // Writes the public id to EEPROM
    if ( ArduKeyEEPROM::setPublicId(publicId) == true )
    {
        Serial.println("The new public id:");
        ArduKeyUtilities::serialDump(publicId, ARDUKEY_PUBLICID_SIZE);
        Serial.println();
    }
    else
    {
        Serial.println("Error: The AES key could not be written!");
    }

    // Writes the secret id to EEPROM
    if ( ArduKeyEEPROM::setSecretId(secretId) == true )
    {
        Serial.println("The new secret id:");
        ArduKeyUtilities::serialDump(secretId, ARDUKEY_SECRETID_SIZE);
        Serial.println();
    }
    else
    {
        Serial.println("Error: The AES key could not be written!");
    }

    Serial.println("Resetting counter...");

    // Resets counter value
    ArduKeyEEPROM::setCounter(0x0000);

    Serial.println();
}

/*
 * The Arduino setup method.
 * 
 * @return void
 *
 */
void setup()
{
    Serial.begin(9600);

    // Wait for serial port to connect
    while (!Serial)
    {
        ;
    }

    Serial.println("Setuping ArduKey...");
    setupArduKey();
}

/*
 * The Arduino loop method.
 * 
 * @return void
 *
 */
void loop() 
{

}
