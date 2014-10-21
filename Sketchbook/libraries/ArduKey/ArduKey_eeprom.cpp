#include "ArduKey_eeprom.h"
#include <avr/eeprom.h>
#include "Arduino.h"

/*
 * Class based on EpromUtils:
 * http://playground.arduino.cc/Code/EepromUtil
 *
 */


/*
 * Dumps the complete EEPROM as hex output via Serial.
 * 
 * @return void
 *
 */
void ArduKeyEEPROM::dumpEEPROM()
{
  // The byte counter in a row
  int x = 0;

  unsigned char currentByte;
  char buffer[16];

  for (int n = EEPROM_MIN_ADDRESS; n <= EEPROM_MAX_ADDRESS; n++)
  {
    // Prints the address if it is the first byte in a row
    if (x == 0)
    {
      sprintf(buffer, "%03X: ", n);
      Serial.print(buffer);
    }

    // Reads one byte and write to buffer
    currentByte = eeprom_read_byte((unsigned char *) n);
    sprintf(buffer, "%02X ", currentByte);

    x++;

    if (x == 16)
    {
      x = 0;
      Serial.println(buffer);
    }
    else
    {
      Serial.print(buffer);
    }
  }

}

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
 * Note: We use the passed pointer style (and not a unsigned char buffer[]) to provide 2 methods:
 * 1) Write to a passed unsigned char buffer[]
 * 2) Write to a unsigned int variable which consists of more than one byte.
 * 
 * @param startAddress The address we start reading from.
 * @param ptr The pointer or array we write to.
 * @param length The length of the data.
 * @return bool
 *
 */
bool ArduKeyEEPROM::getBytes(int startAddress, unsigned char* ptr, int length)
{
  if ( !ArduKeyEEPROM::isAddressOkay(startAddress) || !ArduKeyEEPROM::isAddressOkay(startAddress + length) )
  {
    return false;
  }

  for (int i = 0; i < length; i++)
  {
    // The address passed to eeprom_x_byte() is a normal integer and has to be casted to "real address type" (represented via byte pointer).
    *ptr++ = eeprom_read_byte((unsigned char *) (startAddress + i));
  }

  return true;
}

/*
 * Writes the given array to position (starting at given address) to EEPROM.
 * 
 * Note: We use the passed pointer style (and not a unsigned char buffer[]) to provide 2 methods:
 * 1) Read from a passed unsigned char buffer[]
 * 2) Read from a unsigned int variable which consists of more than one byte.
 *
 * @param startAddress The address we start writing to.
 * @param values The pointer or array we read from.
 * @param length The length of the data.
 * @return bool
 *
 */
bool ArduKeyEEPROM::setBytes(int startAddress, unsigned char* ptr, int length)
{
  if ( !ArduKeyEEPROM::isAddressOkay(startAddress) || !ArduKeyEEPROM::isAddressOkay(startAddress + length) )
  {
    return false;
  }

  for (int i = 0; i < length; i++)
  {
    // The address passed to eeprom_x_byte() is a normal integer and has to be casted to "real address type" (represented via byte pointer).
    eeprom_write_byte((unsigned char *) (startAddress + i), *ptr++);
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
  unsigned int value; // 16 Bits
  unsigned char* ptr = (unsigned char*) &value; // 8 Bits

  ArduKeyEEPROM::getBytes(EEPROM_COUNTER_POS, ptr, EEPROM_COUNTER_LEN);
  return value;
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
  // value = 16 Bits
  // ptr = 8 Bits
  unsigned char* ptr = (unsigned char*) &value;

  ArduKeyEEPROM::setBytes(EEPROM_COUNTER_POS, ptr, EEPROM_COUNTER_LEN);
}