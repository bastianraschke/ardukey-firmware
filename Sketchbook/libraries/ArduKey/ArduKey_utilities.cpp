#include "ArduKey_utilities.h"

// Note: For Serial library and constants:
#include <Arduino.h>

/*
 * Dumps an given byte array via Serial output.
 * 
 * @args array: The array to dump.
 * @args length: The length of the array.
 * @return void
 *
 */
void ArduKeyUtilities::serialDump(unsigned char array[], int length)
{
  for (int i = 0; i < length; i++)
  {
    Serial.print("0x");
    Serial.print(array[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}
