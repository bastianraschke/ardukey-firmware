/*
 * ArduKey - A OTP token device based on Arduino.
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
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <ArduKey.h>
#include <AES.h>

AES aes;

ardukey_token_t token;
ardukey_otp_t otp;

/*
 * The setup method.
 * 
 * @return void
 *
 */
void setup()
{
    Serial.begin(57600);
    initializeArduKey();
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
    token.session = 0x0000;

    // Initializes timestamp
    token.timestamp = 0xDDEE;

    // Initializes pseudo random number generator
    randomSeed(analogRead(0));

    // Increments counter
    ArduKeyEEPROM::setCounter(token.counter + 1);
}

/*
 * Generates a new ready-to-output OTP.
 *
 * @args result: The OTP.
 * @return bool
 *
 */
bool generateOneTimePad(char result[ARDUKEY_OTP_SIZE])
{
    // Sets current timestamp
    // TODO
    token.timestamp = 0xDDEE;

    // Sets some random entropy (range 0..66535)
    token.random = random (65536);

    // Calculates CRC16 checksum of raw token
    // (only the first 14 Bytes cause we do not want to include the checksum itself)
    token.crc = ArduKeyUtilities::CRC16((uint8_t*) &token, ARDUKEY_BLOCKSIZE - 2);

    #ifdef ARDUKEY_DEBUG
        // Prints raw token
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
        // DEBUG: Print OTP struct
        ArduKeyUtilities::serialDump((uint8_t*) &otp, sizeof(otp));
    #endif

    // Converts full OTP (public id + encrypted raw token)
    ArduKeyUtilities::convertToHex((char *) &otp, result, ARDUKEY_PUBLICID_SIZE + ARDUKEY_BLOCKSIZE);

    // Increments session counter
    token.session++;

    return true;
}

/*
 * The loop method.
 * 
 * @return void
 *
 */
void loop() 
{
    char otp[ARDUKEY_OTP_SIZE] = "";

    // Generate one OTP per 4 seconds
    generateOneTimePad(otp);
    Serial.println(otp);

    Serial.println();
    delay(4000);
}