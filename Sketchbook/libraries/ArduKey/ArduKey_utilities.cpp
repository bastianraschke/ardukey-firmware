#include "ArduKey_utilities.h"
#include <Arduino.h>

/*
 * Dumps an given array of data via Serial output.
 * 
 * @args values: The array to dump.
 * @args length: The length of the data.
 * @return void
 *
 */
void ArduKeyUtilities::serialDump(unsigned char values[], int length)
{
	char buffer[16];

  for (int i = 0; i < length; i++)
  {
    sprintf(buffer, "0x%02X ", values[i]);
    Serial.print(buffer);

  }
  Serial.println();
}