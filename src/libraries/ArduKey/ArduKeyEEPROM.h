/*
 * ArduKey - A simple OTP device based on Arduino.
 *
 * Written by Bastian Raschke <bastian.raschke@posteo.de>
 * Copyright (C) 2014 Bastian Raschke
 * All rights reserved.
 *
 */

#ifndef __ARDUKEY_EEPROM_H__
#define __ARDUKEY_EEPROM_H__


// Limit for EEPROM (different on the Arduino models / Atmel chips)
// Note: 512 Bytes should be work on usual models
#define EEPROM_MIN_ADDRESS 0
#define EEPROM_MAX_ADDRESS 511

// Position and length of AES key in EEPROM
#define EEPROM_AESKEY_POS 0
#define EEPROM_AESKEY_LEN 16

// Position and length of counter in EEPROM
#define EEPROM_COUNTER_POS (EEPROM_AESKEY_POS + EEPROM_AESKEY_LEN)
#define EEPROM_COUNTER_LEN 2

// Position and length of public id in EEPROM
#define EEPROM_PUBLICID_POS (EEPROM_COUNTER_POS + EEPROM_COUNTER_LEN)
#define EEPROM_PUBLICID_LEN 6

// Position and length of secret id in EEPROM
#define EEPROM_SECRETID_POS (EEPROM_PUBLICID_POS + EEPROM_PUBLICID_LEN)
#define EEPROM_SECRETID_LEN 6

class ArduKeyEEPROM
{
    public:

        static void dumpEEPROM();

        static bool getBytes(int startAddress, uint8_t* ptr, size_t length);
        static bool setBytes(int startAddress, const uint8_t* ptr, size_t length);

        static bool getAESKey(uint8_t buffer[AES_KEYSIZE]);
        static bool setAESKey(const uint8_t values[AES_KEYSIZE]);

        static bool getPublicId(uint8_t buffer[ARDUKEY_PUBLICID_SIZE]);
        static bool setPublicId(const uint8_t values[ARDUKEY_PUBLICID_SIZE]);

        static bool getSecretId(uint8_t buffer[ARDUKEY_SECRETID_SIZE]);
        static bool setSecretId(const uint8_t values[ARDUKEY_SECRETID_SIZE]);

        static uint16_t getCounter();
        static void setCounter(uint16_t value);

    private:

        static bool isAddressOkay(int address);
};


#endif
