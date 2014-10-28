#include <iostream>

using namespace std;



unsigned int crc16(const unsigned char data_p[], unsigned char length){
    unsigned char x;
    unsigned short crc = 0xFFFF;

    int i = 0;

    while (length--){
        x = crc >> 8 ^ data_p[i++];
        x ^= x>>4;
        crc = (crc << 8) ^ ((x << 12)) ^ ((x <<5)) ^ (x);

        crc &= 0xffff;
    }
    return crc;
}


uint16_t
yubikey_crc16 (const uint8_t * buf, size_t buf_size)
{
  uint16_t m_crc = 0xffff;

  while (buf_size--)
    {
      int i, j;
      m_crc ^= (uint8_t) * buf++ & 0xFF;
      for (i = 0; i < 8; i++)
    {
      j = m_crc & 1;
      m_crc >>= 1;
      if (j)
        m_crc ^= 0x8408;
    }
    }

  return m_crc;
}



/*
unsigned int crc_1021(unsigned int old_crc, int8 data)
{
  unsigned int crc;
  unsigned int x;

  x = ((old_crc>>8) ^ data) & 0xff;
  x ^= x>>4;

  crc = (old_crc << 8) ^ (x << 12) ^ (x <<5) ^ x;

  crc &= 0xffff;      // enable this line for processors with more than 16 bits

  return crc;
}
*/


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