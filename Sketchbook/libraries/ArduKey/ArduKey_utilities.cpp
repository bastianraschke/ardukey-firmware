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

void ArduKeyUtilities::serialDump(char values[], int length)
{
  ArduKeyUtilities::serialDump((unsigned char*) values, length);
}

/*
 * Converts a given array to an array that will contain the two hexadezimal representation chars per char.
 * (thanks to #3 on https://stackoverflow.com/questions/10723403/char-array-to-hex-string-c)
 *
 * // Example:
 * char src[2] = { 0xAA, 0xBB };
 *
 * // will converted to:
 * char dst[5] = { 'a', 'a', 'b', 'b', '\0' };
 *
 * // respectively:
 * char dst[5] = "aabb";
 * 
 * @args dst: The array to convert (const = read only).
 * @args src: The result array.
 * @args srcSize: The length of src.
 * @return void
 *
 */
void ArduKeyUtilities::convertToHex(const char src[], char dst[], int srcSize)
{
  // The transforming table
  const char trans[16] = { 'c', 'b', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'n', 'r', 't', 'u', 'v' };

  // Source field counter
  int a = 0;

  // Destination field counter
  int b = 0;

  for (int i = 0; i < srcSize; i++)
  {
    char currentSrcByte = src[a++];

    // Important: Never change the brackets
    dst[b++] = trans[ (currentSrcByte & 0xf0) >> 4 ];
    dst[b++] = trans[ (currentSrcByte & 0x0f) >> 0 ];
  }

  dst[b] = '\0';
}