#include <iostream>

// Include integer type aliases
#include <inttypes.h>

using namespace std;


uint16_t timestamp_h = 0x0000;
uint8_t timestamp_l = 0x00;

uint32_t incrementTimestamp()
{
    uint32_t currentTimestamp = (timestamp_h << 8) | (timestamp_l << 0);

    currentTimestamp++;

    timestamp_h = currentTimestamp >> 8;
    timestamp_l = currentTimestamp >> 0;

    return currentTimestamp;
}

/*

h = 1111 1111 1111 1111
l = 0000 0001

c = 0000 0000 1111 1111 1111 1111 0000 0001
*/


/*
 * Some test cases.
 *
 */
int main(int argc, char** argv)
{
    cout << endl;

    for (uint32_t i = 100000000; i < 100000010; i++)
    {
      uint32_t testValue = incrementTimestamp();

      if (i != testValue)
      {
          cout << i << " != " << testValue << endl;
      }
    }

    cout << "Tests done!" << endl;


}