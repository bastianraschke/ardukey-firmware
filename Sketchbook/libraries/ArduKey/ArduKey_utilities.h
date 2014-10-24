#ifndef __ARDUKEY_UTILITIES_H__
#define __ARDUKEY_UTILITIES_H__


class ArduKeyUtilities
{
  public:
    static void serialDump(unsigned char array[], int length);
    static void serialDump(char array[], int length);

    static void convertToHex(const char src[], char dst[], int srcSize);
};


#endif
