#include "ArduKey_eeprom.h"
#include <avr/eeprom.h>


/*
 * Checks if given address is in a valid range.
 * 
 * @param address The address to check.
 * @return bool
 *
 */
bool ArduKeyEEPROM::isAddressOkay(int address)
{
  return (address >= EEPROM_MIN_ADDRESS && address <= EEPROM_MAX_ADDRESS);
}

/*
 * Reads a given count of bytes from EEPROM.
 * 
 * @param startAddress The address we start reading from.
 * @param length The count of bytes we read.
 * @return array<unsigned char>
 *
 */
unsigned char* ArduKeyEEPROM::getBytes(int startAddress, int length)
{
  /*
  if ( !ArduKeyEEPROM::isAddressOkay(startAddress) || !ArduKeyEEPROM::isAddressOkay(startAddress + length) )
  {
    return ;
  }
  */

  // WARNING: Without the "static" the ADDRESS from variable would be passed outside instead the data!
  static unsigned char* returnValue;

  for (int i = 0; i < length; i++)
  {
    // The address passed to eeprom_write_byte() is a normal integer like "123"
    // and has to be casted to "real address type" (represented via byte pointer).
    returnValue[i] = eeprom_read_byte((unsigned char *) (startAddress + i));
  }

  return returnValue;
}

/*
 * Writes the given array to position (starting at given address) to EEPROM.
 * 
 * @param startAddress The address we start writing to.
 * @param values The array of bytes we write.
 * @param length The length of the array.
 * @return void
 *
 */
void ArduKeyEEPROM::setBytes(int startAddress, unsigned char* values, int length)
{
  if ( !ArduKeyEEPROM::isAddressOkay(startAddress) || !ArduKeyEEPROM::isAddressOkay(startAddress + length) )
  {
    return ;
  }

  for (int i = 0; i < length; i++)
  {
    // The address passed to eeprom_write_byte() is a normal integer like "123"
    // and has to be casted to "real address type" (represented via byte pointer).
    eeprom_write_byte((unsigned char *) (startAddress + i), values[i]);
  }
}





















/*
 * Reads the current AES key from EEPROM.
 * 
 * @return array<unsigned char>
 *
 */
unsigned char* ArduKeyEEPROM::getAESKey()
{

  unsigned char* readed = ArduKeyEEPROM::getBytes(EEPROM_AESKEY_POS, EEPROM_AESKEY_LEN);


	return readed;
}

/*
 * Writes a new AES key to EEPROM.
 * 
 * @param aesKey The new AES key.
 * @return void
 *
 */
void ArduKeyEEPROM::setAESKey(unsigned char* aesKey)
{
	ArduKeyEEPROM::setBytes(EEPROM_AESKEY_POS, aesKey, EEPROM_AESKEY_LEN);
}













/*
 * Gets the current counter value from EEPRO.
 * 
 * @return unsigned int
 *
 */
unsigned int ArduKeyEEPROM::getCounter()
{
  unsigned char* readed = ArduKeyEEPROM::getBytes(EEPROM_COUNTER_POS, EEPROM_COUNTER_LEN);
  return (unsigned int) readed[0];
}

/*
 * Writes the new counter value to EEPROM.
 * 
 * @param value The new counter value.
 * @return void
 *
 */
void ArduKeyEEPROM::setCounter(unsigned int value)
{

}