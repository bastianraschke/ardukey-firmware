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
#include <AES.h>
#include <TimerOne.h>


AES aes;

ardukey_token_t token;
ardukey_otp_t otp;

int previousButtonState = HIGH;

/*
 * Increments the non-volatile counter value.
 *
 * @return void
 *
 */
void incrementCounter()
{
    token.counter += 1;
    ArduKeyEEPROM::setCounter(token.counter);
}

/*
 * Increments the volatile session counter value.
 *
 * @return void
 *
 */
void incrementSessionCounter()
{
    // Checks if the session counter reached the maximum possible value (255)
    if ( token.session == 0xFF )
    {
        token.session = 0x00;

        // Increments the non-volatile counter instead to indicate change
        incrementCounter();
    }
    else
    {
        token.session += 1;
    }
}

/*
 * Increments the timestamp value.
 *
 * @return void
 *
 */
void incrementTimestamp()
{
    // Combines high and low part to 24 Byte integer
    uint32_t timestamp = (token.timestamp_h << 8) | (token.timestamp_l << 0);
    timestamp += 1;

    token.timestamp_h = timestamp >> 8; // High (left) part
    token.timestamp_l = timestamp >> 0; // Low (right) part
}

/*
 * Contains all ArduKey initalizing processes.
 * 
 * @return void
 *
 */
void initializeArduKey()
{
    uint8_t aesKey[AES_KEYSIZE] = {0};
    uint8_t publicId[ARDUKEY_PUBLICID_SIZE] = {0};
    uint8_t secretId[ARDUKEY_SECRETID_SIZE] = {0};

    // Reads the AES key from EEPROM and sets AES library preferences
    ArduKeyEEPROM::getAESKey(aesKey);
    aes.set_key(aesKey, AES_CIPHER_BITS);

    // Reads the public id from EEPROM and sets to OTP struct
    ArduKeyEEPROM::getPublicId(publicId);
    memcpy(otp.publicId, publicId, sizeof(publicId));

    // Reads the secret id from EEPROM and sets to token struct
    ArduKeyEEPROM::getSecretId(secretId);
    memcpy(token.secretId, secretId, sizeof(secretId));

    // Gets current counter value
    token.counter = ArduKeyEEPROM::getCounter();

    // Initializes session counter
    token.session = 0x00;

    // Initializes timestamp
    token.timestamp_h = 0x0000;
    token.timestamp_l = 0x00;

    // Initializes pseudo random number generator with "random" analog pin noise
    uint16_t randomSeedValue = analogRead(0);
    randomSeed(randomSeedValue);

    // Increments counter
    incrementCounter();

    // Initializes timer library for updating timestamp
    Timer1.initialize();
    Timer1.attachInterrupt(incrementTimestamp, TIMERONE_TIMESTAMPUPDATE);
}

/*
 * Generates a new ready-to-output OTP.
 *
 * @args result The OTP.
 * @return bool
 *
 */
bool generateOneTimePad(char result[ARDUKEY_OTP_SIZE])
{
    // Gets some random entropy (range 0..66535)
    token.random = random(65536);

    // Calculates CRC16 checksum of raw token
    // (only the first 14 Bytes cause we do not want to include the checksum itself)
    token.crc = ArduKeyUtilities::CRC16((uint8_t*) &token, ARDUKEY_BLOCKSIZE - 2);

    #ifdef ARDUKEY_DEBUG
        // Prints raw token
        Serial.println("Raw token:");
        ArduKeyUtilities::serialDump((uint8_t*) &token, sizeof(token));
    #endif

    // The buffer for encrypted raw token
    uint8_t cipher[AES_BLOCKSIZE] = {0};

    // Encrypts the raw token
    if ( aes.encrypt((uint8_t*) &token, cipher) != 0 )
    {
        return false;
    }

    // Copy encrypted raw token to OTP struct
    memcpy(otp.encryptedRawToken, cipher, sizeof(cipher));

    #ifdef ARDUKEY_DEBUG
        // Prints OTP struct
        Serial.println("Complete OTP:");
        ArduKeyUtilities::serialDump((uint8_t*) &otp, sizeof(otp));
    #endif

    // Converts full OTP (public id + encrypted raw token)
    ArduKeyUtilities::convertToHex((char *) &otp, result, ARDUKEY_PUBLICID_SIZE + ARDUKEY_BLOCKSIZE);

    // Increments session counter
    incrementSessionCounter();

    return true;
}

/*
 * The Arduino setup method.
 * 
 * @return void
 *
 */
void setup()
{
    Serial.begin(57600);

    #ifdef ARDUKEY_DEBUG
        // Only needed for Arduino Leonardo and Micro:
        // Do nothing until the Serial is not open
        // while (!Serial);
        Serial.println("Initializing ArduKey...");
    #endif

    initializeArduKey();

    // Configures button pin as an input and enable the internal 20K Ohm pull-up resistor
    pinMode(ARDUKEY_PIN_BUTTON, INPUT_PULLUP);
}

/*
 * The Arduino loop method.
 * 
 * @return void
 *
 */
void loop() 
{
    char otp[ARDUKEY_OTP_SIZE] = "";

    // Gets current button state
    int buttonState = digitalRead(ARDUKEY_PIN_BUTTON);

    if ( buttonState != previousButtonState && buttonState == HIGH )
    {
        // Initializes control over the keyboard
        Keyboard.begin();

        generateOneTimePad(otp);
        Keyboard.print(otp);

        // Ends keyboard control
        Keyboard.end();
    }
  
    previousButtonState = buttonState;
}