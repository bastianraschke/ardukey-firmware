/*
 * ArduKey - A slim OTP token device based on Arduino.
 *
 * Written by Bastian Raschke <bastian.raschke@posteo.de>
 * Copyright (C) 2014 Bastian Raschke
 * All rights reserved.
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
    // The used AES key used for symmetric encryption
    const uint8_t aesKey[AES_KEYSIZE] =
    {
        // CHANGE THIS:
        0x7A, 0x18, 0x58, 0x59, 0x2F, 0xCB, 0x76, 0xBD, 0x5E, 0xB2, 0x68, 0x54, 0x21, 0xAE, 0xD4, 0x5E
    };

    // The public identity (must be unique to identiy device)
    const uint8_t publicId[ARDUKEY_PUBLICID_SIZE] =
    {
        // CHANGE THIS:
        0x00, 0x00, 0x00, 0x00, 0x00, 0x01
    };

    // The secret identity that is only known by device and Auth server 
    const uint8_t secretId[ARDUKEY_SECRETID_SIZE] =
    {
        // CHANGE THIS:
        0xB0, 0xD4, 0xA2, 0xD6, 0x9B, 0xC4
    };

    Serial.println("ArduKey setup");
    Serial.println("Writing new values...");
    Serial.println();

    // Writes the AES key to EEPROM
    if ( ArduKeyEEPROM::setAESKey(aesKey) == true )
    {
        Serial.println("Your new AES key:");
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
        Serial.println("Your new public id:");
        ArduKeyUtilities::serialDump(publicId, ARDUKEY_PUBLICID_SIZE);
        Serial.println();
    }
    else
    {
        Serial.println("Error: The public id could not be written!");
    }

    // Writes the secret id to EEPROM
    if ( ArduKeyEEPROM::setSecretId(secretId) == true )
    {
        Serial.println("Your new secret id:");
        ArduKeyUtilities::serialDump(secretId, ARDUKEY_SECRETID_SIZE);
        Serial.println();
    }
    else
    {
        Serial.println("Error: The secret id could not be written!");
    }

    Serial.println("Resetting persitent counter...");

    // Resets counter value
    ArduKeyEEPROM::setCounter(0x0000);

    Serial.println("All done.");
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
    // Nothing to do ;)
}
