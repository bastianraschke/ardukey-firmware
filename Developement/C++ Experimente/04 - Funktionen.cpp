#include <iostream>

using namespace std;


unsigned int test_getUInt()
{
  return 0;
}

/*
 * Some test cases.
 *
 */
int main(int argc, char** argv)
{
  cout << endl;

  unsigned int value = test_getUInt();
  cout << "value = " << value << endl;

  value++;
  cout << "value = " << value << endl;

  return 0;
}