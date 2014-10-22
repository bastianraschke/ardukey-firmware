#include <iostream>

using namespace std;








void convertIntToHexString(unsigned char dst[], const unsigned char src[], int srcSize)
{
  // The transforming table
  const char trans[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

  // Source field counter
  int a = 0;

  // Destination field counter
  int b = 0;

  for (int i = 0; i < srcSize; i++)
  {
    const unsigned char currentSrcByte = src[a++];

    dst[b++] = trans[ (currentSrcByte & 0xf0) >> 4 ];
    dst[b++] = trans[ (currentSrcByte & 0x0f) >> 0 ];
  }

  dst[b] = '\0';
}

void convertIntToEqlString(unsigned char dst[], const unsigned char src[], int srcSize)
{
  // The transforming table
  const char trans[16] = { 'c', 'b', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'n', 'r', 't', 'u', 'v' };

  // Source field counter
  int a = 0;

  // Destination field counter
  int b = 0;

  for (int i = 0; i < srcSize; i++)
  {
    const unsigned char currentSrcByte = src[a++];

    dst[b++] = trans[ (currentSrcByte & 0xf0) >> 4 ];
    dst[b++] = trans[ (currentSrcByte & 0x0f) >> 0 ];
  }

  dst[b] = '\0';
}

/*
 * Some test cases.
 *
 */
int main(int argc, char** argv)
{
  cout << endl;

  // Anscheinend ist es egal ob signed/unsigned bei einem char
  char trans1[] = "0123456789abcdef";
  unsigned char trans2[] = "0123456789abcdef";

  cout << "trans1 = " << trans1[0] << endl;
  cout << "trans2 = " << trans2[0] << endl;
  cout << endl;


  unsigned char test[] =
  {
    0xFF, 'a', 'b', 97, 0x20,
  };

  unsigned char buffer[10];
  convertIntToEqlString(buffer, test, sizeof(test));

  for (int i = 0; i < sizeof(test); i++)
  {
    cout << "test[" << i << "] = " << test[i] << endl;
  }
  cout << endl;

  for (int i = 0; i < sizeof(buffer); i++)
  {
    cout << "buffer[" << i << "] = " << buffer[i] << endl;
  }
  cout << endl;

  return 0;
}