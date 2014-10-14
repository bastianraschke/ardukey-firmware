#include "ArduKey_utilities.h"

// Note: For "Serial" and constants (other types should not be used):
#include <Arduino.h>

/*
 * Dumps an given byte array via Serial output.
 * 
 * @args array: The array to dump.
 * @args length: The length of the array.
 * @return void
 *
 */
void utilities_serialDump(unsigned char array[], unsigned int length)
{
  for (unsigned int i = 0; i < length; i++)
  {
    Serial.print("0x");
    Serial.print(array[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}
