#ifndef __ARDUKEY_EEPROM_H__
#define __ARDUKEY_EEPROM_H__


//
// Absolute min and max eeprom addresses.
// Actual values are hardware-dependent.
//
// These values can be changed e.g. to protect
// eeprom cells outside this range.
//
const unsigned int EEPROM_MIN_ADDRESS = 0;
const unsigned int EEPROM_MAX_ADDRESS = 511;



#define EEPROM_AESKEY_POS		0
#define EEPROM_AESKEY_LEN		16

#define EEPROM_COUNTER_POS	(EEPROM_AESKEY_POS + EEPROM_AESKEY_LEN)
#define EEPROM_COUNTER_LEN	2


extern unsigned char* eeprom_readBytes(int address, int length);
extern void eeprom_writeBytes(int address, unsigned int* values, int length);

extern unsigned int eeprom_getCounter();



extern unsigned char* eeprom_getAESKey();
extern void eeprom_setAESKey(unsigned char aesKey[]);



#endif
