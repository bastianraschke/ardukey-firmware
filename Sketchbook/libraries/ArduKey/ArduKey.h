/*
 * ArduKey - A slim OTP token device based on Arduino.
 *
 * Written by Bastian Raschke <bastian.raschke@posteo.de>
 * Copyright (C) 2014 Bastian Raschke
 * All rights reserved.
 *
 */

#ifndef __ARDUKEY_H__
#define __ARDUKEY_H__

// Include integer type aliases
#include <inttypes.h>

// Include default types like size_t
#include <stdlib.h>

// For sprintf(), Serial:
#include <Arduino.h>


// ArduKey debugging
//

// Uncomment to enable debugging:
#define ARDUKEY_DEBUG 1


// ArduKey general definitions
//

#define ARDUKEY_PIN_BUTTON 2

#define ARDUKEY_BLOCKSIZE 16

#define ARDUKEY_PUBLICID_SIZE 6
#define ARDUKEY_SECRETID_SIZE 6

// Length: 44 chars + '\0' char
#define ARDUKEY_OTP_SIZE ((ARDUKEY_PUBLICID_SIZE + ARDUKEY_BLOCKSIZE) * 2 + 1)


// TimerOne library definitions
//

// Update interval of timestamp in µs
// 8 Hz in this case (1000000 / 8)
#define TIMERONE_TIMESTAMPUPDATE 125000


// AES library definitions
//

#define AES_CIPHER_BITS 128
#define AES_BLOCKSIZE 16
#define AES_KEYSIZE 16


// ArduKey special type definitions
//

// The raw token structure
typedef struct
{
    // The secret identity of the ArduKey
    // 6 Bytes
    uint8_t secretId[ARDUKEY_SECRETID_SIZE];

    // The current counter value
    // 2 Bytes
    uint16_t counter;

    // The current session counter value
    // 1 Byte
    uint8_t session;

    // The non-volatile timestamp (high part)
    // 2 Bytes
    uint16_t timestamp_h;

    // The non-volatile timestamp (low part)
    // 1 Byte
    uint8_t timestamp_l;

    // Pseudo-random entropy
    // 2 Bytes
    uint16_t random;

    // The CRC16 checksum of all data
    // 2 Bytes
    uint16_t crc;
}
ardukey_token_t;

// The final OTP structure
typedef struct
{
    // The public id part
    // 6 Bytes
    uint8_t publicId[ARDUKEY_PUBLICID_SIZE];

    // The encrypted raw token.
    // 16 Bytes
    uint8_t encryptedRawToken[AES_BLOCKSIZE];

}
ardukey_otp_t;


// Includes all ArduKey library files
#include "ArduKeyEEPROM.h"
#include "ArduKeyUtilities.h"

#endif
