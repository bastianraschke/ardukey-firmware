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

#ifndef __ARDUKEY_EEPROM_H__
#define __ARDUKEY_EEPROM_H__


// Limit for Arduino EEPROM (may be different on the Arduino models)
// 512 Bytes should be the lowest limit of usual Arduino models (the ATmega168).
// Refering to: http://arduino.cc/en/pmwiki.php?n=Reference/EEPROM
#define EEPROM_MIN_ADDRESS 0
#define EEPROM_MAX_ADDRESS 512 - 1

// Data position definitions
//

#define EEPROM_AESKEY_POS 0
#define EEPROM_AESKEY_LEN 16

#define EEPROM_COUNTER_POS (EEPROM_AESKEY_POS + EEPROM_AESKEY_LEN)
#define EEPROM_COUNTER_LEN 2

#define EEPROM_PUBLICID_POS (EEPROM_COUNTER_POS + EEPROM_COUNTER_LEN)
#define EEPROM_PUBLICID_LEN 6

#define EEPROM_SECRETID_POS (EEPROM_PUBLICID_POS + EEPROM_PUBLICID_LEN)
#define EEPROM_SECRETID_LEN 6

class ArduKeyEEPROM
{
    public:

        static void dumpEEPROM();

        static bool getBytes(int startAddress, uint8_t* ptr, int length);
        static bool setBytes(int startAddress, const uint8_t* ptr, int length);

        static bool getAESKey(uint8_t buffer[AES_KEYSIZE]);
        static bool setAESKey(uint8_t values[AES_KEYSIZE]);

        static bool getPublicId(uint8_t buffer[ARDUKEY_PUBLICID_SIZE]);
        static bool setPublicId(uint8_t values[ARDUKEY_PUBLICID_SIZE]);

        static bool getSecretId(uint8_t buffer[ARDUKEY_SECRETID_SIZE]);
        static bool setSecretId(uint8_t values[ARDUKEY_SECRETID_SIZE]);

        static uint16_t getCounter();
        static void setCounter(uint16_t);

    private:

        static bool isAddressOkay(int address);
};


#endif