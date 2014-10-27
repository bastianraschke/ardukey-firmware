#ifndef __ARDUKEY_UTILITIES_H__
#define __ARDUKEY_UTILITIES_H__


class ArduKeyUtilities
{
    public:

        static void serialDump(const uint8_t values[], const int length);

        static uint16_t CRC16(const uint8_t values[], const int length);
        static void convertToHex(const char src[], char dst[], const int srcSize);

};


#endif
