#include "ArduKey_eeprom.h"
#include <avr/eeprom.h>


/*
unsigned char* eeprom_readBytes(int address, int length)
{
  unsigned char* returnValue;
  while (length--)
  {
    *returnValue++ = eeprom_read_byte(address++);
  }
}
*/


/*
 * Gets the current counter value of ArduKey.
 * 
 * @return unsigned int
 *
 */
unsigned int eeprom_readCounter()
{
  return 0;
}

