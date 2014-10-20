#ifndef __ARDUKEY_H__
#define __ARDUKEY_H__

// Include ArduKey library files
#include "ArduKey_eeprom.h"
#include "ArduKey_utilities.h"


// AES library configuration
//

// Used AES mode
#define AES_CIPHER_BITS 128

// The size of a block
#define AES_BLOCKSIZE N_BLOCK

// We only work with one block
#define AES_CBC_BLOCKCOUNT 1


#endif