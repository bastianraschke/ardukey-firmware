#include "ArduKey_eeprom.h"
#include <avr/eeprom.h>


// TODO: unsigned char vs unsigned int



/*
 * Erases the complete EEPROM.
 * 
 * @param value The value which will be written to all EEPROM memory cells.
 * @return void
 *
 */
void eeprom_eraseAll(unsigned char value = 0xFF)
{
  for (unsigned int address = EEPROM_MIN_ADDRESS; address <= EEPROM_MAX_ADDRESS; address++)
  {
  	// TODO: Use wrapped function eeprom_writeBytes()?
    eeprom_write_byte((unsigned char *) address, value);
  }
}

/*
//
// Returns true if the address is between the
// minimum and maximum allowed values,
// false otherwise.
//
// This function is used by the other, higher-level functions
// to prevent bugs and runtime errors due to invalid addresses.
//
boolean eeprom_is_addr_ok(int addr) {
  return ((addr >= EEPROM_MIN_ADDR) && (addr <= EEPROM_MAX_ADDR));
}
*/



/*
 * Reads a given count of bytes from EEPROM.
 * 
 * @param address The address we start reading from.
 * @param length The count of bytes we read.
 * @return array<unsigned char>
 *
 */
unsigned char* eeprom_readBytes(int address, int length)
{
  unsigned char returnValue[length];
  int i = 0;

  while (length--)
  {
    returnValue[i++] = eeprom_read_byte((unsigned char*) address);
    address++;
  }

  return returnValue;
}

/*
 * Writes the given array to position (starting at given address) to EEPROM.
 * 
 * @param address The address we start writing to.
 * @param values The array of bytes we write.
 * @param length The length of the array.
 * @return void
 *
 */
void eeprom_writeBytes(int address, unsigned int* values, int length)
{
  int i = 0;

  while (length--)
  {
    eeprom_write_byte((unsigned char *) address, values[i++]);
    address++;
  }
}




/*
 * Gets the current counter value of ArduKey.
 * 
 * @return unsigned int
 *
 */
unsigned int eeprom_getCounter()
{
	unsigned char* readed = eeprom_readBytes(EEPROM_COUNTER_POS, EEPROM_COUNTER_LEN);
	return (unsigned int) readed[0];
}





unsigned char* eeprom_getAESKey()
{
	//unsigned char readed[EEPROM_AESKEY_LEN] = eeprom_readBytes(EEPROM_AESKEY_POS, EEPROM_AESKEY_LEN);

	unsigned char* readed = eeprom_readBytes(EEPROM_AESKEY_POS, EEPROM_AESKEY_LEN);
	return readed;
}

void eeprom_setAESKey(unsigned char aesKey[])
{
	eeprom_writeBytes(EEPROM_AESKEY_POS, (unsigned int*) aesKey, EEPROM_AESKEY_LEN);
}
