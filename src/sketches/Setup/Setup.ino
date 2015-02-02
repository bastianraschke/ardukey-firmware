/*
 * ArduKey - A simple OTP device based on Arduino.
 *
 * Written by Bastian Raschke <bastian.raschke@posteo.de>
 * Copyright (C) 2015 Bastian Raschke
 * All rights reserved.
 *
 */

#include <ArduKey.h>

/*
 * Initialize the ArduKey setup.
 *
 * @return bool
 *
 */
bool setupArduKey()
{
    // The used AES key used for symmetric encryption
    const uint8_t aesKey[AES_KEYSIZE] =
    {
        0x7A, 0x18, 0x58, 0x59, 0x2F, 0xCB, 0x76, 0xBD, 0x5E, 0xB2, 0x68, 0x54, 0x21, 0xAE, 0xD4, 0x5E
    };

    // The public identity (must be unique to identify device)
    const uint8_t publicId[ARDUKEY_PUBLICID_SIZE] =
    {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x01
    };

    // The secret identity that is only known by device and auth server
    const uint8_t secretId[ARDUKEY_SECRETID_SIZE] =
    {
        0xB0, 0xD4, 0xA2, 0xD6, 0x9B, 0xC4
    };

    Serial.println("ArduKey setup");
    Serial.println("Writing new values...");
    Serial.println();

    // Write the AES key to EEPROM
    if ( ArduKeyEEPROM::setAESKey(aesKey) == true )
    {
        Serial.println("Wrote new AES key.");
    }
    else
    {
        Serial.println("Error: The AES key could not be written!");
        return false;
    }

    // Write the public id to EEPROM
    if ( ArduKeyEEPROM::setPublicId(publicId) == true )
    {
        Serial.println("Wrote new public id.");
    }
    else
    {
        Serial.println("Error: The public id could not be written!");
        return false;
    }

    // Write the secret id to EEPROM
    if ( ArduKeyEEPROM::setSecretId(secretId) == true )
    {
        Serial.println("Wrote new secret id.");
    }
    else
    {
        Serial.println("Error: The secret id could not be written!");
        return false;
    }

    Serial.println("Resetting counter...");

    // Reset counter value
    // Important: Do not set to zero cause the auth server inits with zero!
    ArduKeyEEPROM::setCounter(0x0001);

    Serial.println("All done.");
    return true;
}

int setupOperationResult = false;

/*
 * The Arduino setup function.
 *
 * @return void
 *
 */
void setup()
{
    Serial.begin(9600);

    // Initialize LED
    pinMode(ARDUKEY_PIN_LED, OUTPUT);

    // Start setup of ArduKey
    setupOperationResult = setupArduKey();

    // Enable LED permanently if operation was successfull
    if ( setupOperationResult == true )
    {
        digitalWrite(ARDUKEY_PIN_LED, HIGH);
    }
}

/*
 * The Arduino loop function.
 *
 * @return void
 *
 */
void loop()
{
    // Blink 2 times per second if operation was unsuccessfull
    if ( setupOperationResult == false )
    {
        digitalWrite(ARDUKEY_PIN_LED, HIGH);
        delay(250);
        digitalWrite(ARDUKEY_PIN_LED, LOW);
        delay(250);
    }
}
