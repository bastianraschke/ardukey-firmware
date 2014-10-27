#ifndef __ARDUKEY_EEPROM_H__
#define __ARDUKEY_EEPROM_H__


// Limit for Arduino EEPROM (may be different on the Arduino models).
#define EEPROM_MIN_ADDRESS 0
#define EEPROM_MAX_ADDRESS 512 - 1


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
        static bool setBytes(int startAddress, uint8_t* ptr, int length);

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