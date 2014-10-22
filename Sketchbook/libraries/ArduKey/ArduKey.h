#ifndef __ARDUKEY_H__
#define __ARDUKEY_H__

// Include ArduKey library files
#include "ArduKey_eeprom.h"
#include "ArduKey_utilities.h"



#define ARDUKEY_BLOCKSIZE 16


// AES library configuration
//

// Used AES mode
#define AES_CIPHER_BITS 128

// The size of a block
#define AES_BLOCKSIZE N_BLOCK

// We only work with one block
#define AES_CBC_BLOCKCOUNT 1


/*
# define YUBIKEY_BLOCK_SIZE 16
# define YUBIKEY_KEY_SIZE 16
# define YUBIKEY_UID_SIZE 6
# define YUBIKEY_OTP_SIZE (2 * YUBIKEY_BLOCK_SIZE +1)
*/
//#define ARDUKEY_UID


#endif