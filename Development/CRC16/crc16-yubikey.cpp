#include <iostream>
using namespace std;

#include <inttypes.h>
#include <stdio.h>


uint16_t calculateCRC16(const uint8_t values[], size_t length)
{
    uint16_t crc = 0xFFFF;

    // Pointer sanity check
    if ( !values || length == 0 )
    {
        return crc;
    }

    uint16_t x;

    for (int i = 0; i < length; i++)
    {
        crc ^= values[i] & 0xFF;

        for (int j = 0; j < 8; j++)
        {
            x = crc & 1;
            crc >>= 1;

            if (x)
            {
                crc ^= 0x8408;
            }
        }
    }

    return crc;
}

int main(int argc, char** argv)
{
    cout << endl;

    // CRC16 calculation problems:
    // see: http://forum.yubico.com/viewtopic.php?f=2&t=69

    uint8_t values[6] = {0x55, 0xaa, 0x00, 0xff, 0x4a, 0x34};

    // Calculate checksum:
    uint16_t c_crc = ~calculateCRC16(values, sizeof(values) - 2);

    // Verify checksum:
    uint16_t v_crc = calculateCRC16(values, sizeof(values));

    printf("0x%04X\n", c_crc); // 0x344A = OK
    printf("0x%04X\n", v_crc); // 0xF0B8 = OK
}
