#ifndef __ARDUKEY_H__
#define __ARDUKEY_H__


#define ARDUKEY_BLOCKSIZE 16

#define ARDUKEY_PUBLICID_SIZE 16
#define ARDUKEY_SECRETID_SIZE 6

// Length = 44 chars + '\0' byte
#define ARDUKEY_OTP_SIZE ((ARDUKEY_PUBLICID_SIZE + ARDUKEY_BLOCKSIZE) * 2 + 1)


// Used AES mode
#define AES_CIPHER_BITS 128

#define AES_BLOCKSIZE 16
#define AES_KEYSIZE 16


typedef struct
{
    // The secret identity of the ArduKey
    unsigned char secretId[ARDUKEY_SECRETID_SIZE];

    // The current counter value
    unsigned int counter;

    // The current session counter value
    unsigned int session;

    // The nonvolatile timestamp
    unsigned int timestamp;

    // Pseudo-random entropy
    unsigned int random;

    // The CRC16 checksum of all data
    unsigned int crc;
}
ardukey_token_t;


// Includes all ArduKey library files
#include "ArduKey_eeprom.h"
#include "ArduKey_utilities.h"

#endif