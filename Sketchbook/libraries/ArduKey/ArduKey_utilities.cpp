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

/*
 * Calculates the CRC16 checksum of given data.
 * 
 * @args values: The array to calculate checksum of.
 * @args length: The length of processed data.
 * @return unsigned int
 *
 */
unsigned int ArduKeyUtilities::CRC16(unsigned char values[], int length)
{
    unsigned int foo = 0xffff;

    // TODO
    return foo;
}

/*
 * Converts a given array to an array that will contain the two hexadezimal representation chars per char.
 * (thanks to #3 on https://stackoverflow.com/questions/10723403/char-array-to-hex-string-c)
 *
 * Important:
 * The destination array must be sized N*2 + 1 in comparison to source array (N).
 * 
 * @args dst: The source array we read from.
 * @args src: The result array we write to.
 * @args srcSize: The length of src.
 * @return void
 *
 */
void ArduKeyUtilities::convertToHex(char src[], char dst[], int srcSize)
{
  // The transforming table
  // TODO
  const char table[16] = { 'c', 'b', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'n', 'r', 't', 'u', 'v' };

  // Source field counter
  int a = 0;

  // Destination field counter
  int b = 0;

  for (int i = 0; i < srcSize; i++)
  {
    char currentSrcByte = src[a++];

    // Important: Do not change the brackets
    dst[b++] = table[ (currentSrcByte & 0xf0) >> 4 ];
    dst[b++] = table[ (currentSrcByte & 0x0f) >> 0 ];
  }

  dst[b] = '\0';
}