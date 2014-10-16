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
 * @param buffer The array of bytes we write to.
 * @param length The count of bytes we read.
 * @return bool
 *
 */
bool ArduKeyEEPROM::getBytes(int startAddress, unsigned char buffer[], int length)
{
  if ( !ArduKeyEEPROM::isAddressOkay(startAddress) || !ArduKeyEEPROM::isAddressOkay(startAddress + length) )
  {
    return false;
  }

  for (int i = 0; i < length; i++)
  {
    // The address passed to eeprom_x_byte() is a normal integer and has to be casted to "real address type" (represented via byte pointer).
    buffer[i] = eeprom_read_byte((unsigned char *) (startAddress + i));
  }

  return true;
}

/*
 * Writes the given array to position (starting at given address) to EEPROM.
 * 
 * @param startAddress The address we start writing to.
 * @param values The array of bytes we write.
 * @param length The length of the array.
 * @return bool
 *
 */
bool ArduKeyEEPROM::setBytes(int startAddress, unsigned char values[], int length)
{
  if ( !ArduKeyEEPROM::isAddressOkay(startAddress) || !ArduKeyEEPROM::isAddressOkay(startAddress + length) )
  {
    return false;
  }

  for (int i = 0; i < length; i++)
  {
    // The address passed to eeprom_x_byte() is a normal integer and has to be casted to "real address type" (represented via byte pointer).
    eeprom_write_byte((unsigned char *) (startAddress + i), values[i]);
  }

  return true;
}

/*
 * Reads the current AES key from EEPROM.
 * 
 * @param buffer The array of bytes we write to.
 * @return bool
 *
 */
bool ArduKeyEEPROM::getAESKey(unsigned char buffer[])
{
  return ArduKeyEEPROM::getBytes(EEPROM_AESKEY_POS, buffer, EEPROM_AESKEY_LEN);
}

/*
 * Writes a new AES key to EEPROM.
 * 
 * @param values The array of bytes we write.
 * @return bool
 *
 */
bool ArduKeyEEPROM::setAESKey(unsigned char values[])
{
  return ArduKeyEEPROM::setBytes(EEPROM_AESKEY_POS, values, EEPROM_AESKEY_LEN);
}





/*
 * Gets the current counter value from EEPROM.
 * 
 * @return unsigned int
 *
 */
unsigned int ArduKeyEEPROM::getCounter()
{
  unsigned char buffer[2];
  ArduKeyEEPROM::getBytes(EEPROM_COUNTER_POS, buffer, EEPROM_COUNTER_LEN);

  return (unsigned int) buffer[0];
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