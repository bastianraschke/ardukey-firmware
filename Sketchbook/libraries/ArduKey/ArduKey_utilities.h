#ifndef __ARDUKEY_UTILITIES_H__
#define __ARDUKEY_UTILITIES_H__


class ArduKeyUtilities
{
  public:
    static void serialDump(unsigned char values[], int length);

    static unsigned int CRC16(unsigned char values[], int length);
    static void convertToHex(char src[], char dst[], int srcSize);

};


#endif
