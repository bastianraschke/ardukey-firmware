#include <iostream>

using namespace std;



unsigned int crc16(const unsigned char data_p[], unsigned char length){
    unsigned char x;
    unsigned short crc = 0xFFFF;

    int i = 0;

    while (length--){
        x = crc >> 8 ^ data_p[i++];
        x ^= x>>4;
        crc = (crc << 8) ^ ((unsigned short)(x << 12)) ^ ((unsigned short)(x <<5)) ^ ((unsigned short)x);
    }
    return crc;
}

/*
 * Some test cases.
 *
 */
int main(int argc, char** argv)
{
    cout << endl;



    unsigned char values[2] = { 0xAA, 0xBB };

    cout << crc16(values, 2) << endl;

}