#include <iostream>

using namespace std;


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


  char test1[] =
  {
    0xFF, 'a', 'b', 97, 0x20,
  };

  char test2[] = "abcd";

  cout << "sizeof(test1) = " << sizeof(test1) << endl;
  cout << "sizeof(test2) = " << sizeof(test2) << endl;





  cout << endl;

  return 0;
}