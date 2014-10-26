#ifndef __ARDUKEY_H__
#define __ARDUKEY_H__


/*
 * ArduKey general definitions.
 *
 */

#define ARDUKEY_BLOCKSIZE 16

#define ARDUKEY_PUBLICID_SIZE 6
#define ARDUKEY_SECRETID_SIZE 6

// Length: 44 chars + '\0' char
#define ARDUKEY_OTP_SIZE ((ARDUKEY_PUBLICID_SIZE + ARDUKEY_BLOCKSIZE) * 2 + 1)

/*
 * AES library definitions.
 *
 */

// Used AES mode
#define AES_CIPHER_BITS 128

#define AES_BLOCKSIZE 16
#define AES_KEYSIZE 16


/*
 * ArduKey type definitions.
 *
 */

typedef struct
{
    // The secret identity of the ArduKey
    // 6 Bytes
    unsigned char secretId[ARDUKEY_SECRETID_SIZE];

    // The current counter value
    // 2 Bytes
    unsigned int counter;

    // The current session counter value
    // 2 Bytes
    unsigned int session;

    // The nonvolatile timestamp
    // 2 Bytes
    unsigned int timestamp;

    // Pseudo-random entropy
    // 2 Bytes
    unsigned int random;

    // The CRC16 checksum of all data
    // 2 Bytes
    unsigned int crc;
}
ardukey_token_t;

typedef struct
{
    // The public id part
    // 6 Bytes
    unsigned char publicId[ARDUKEY_PUBLICID_SIZE];

    // The encrypted raw token.
    // 16 Bytes
    unsigned char encryptedRawToken[AES_BLOCKSIZE];

}
ardukey_otp_t;


// Includes all ArduKey library files
#include "ArduKey_eeprom.h"
#include "ArduKey_utilities.h"


#endif