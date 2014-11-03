#include <iostream>
using namespace std;


/*
 * Durch übergebenes Argument OHNE Referenz (call-by-value) kann Inhalt NICHT verändert werden!
 *
 */
void test_callByValue(int i)
{
  i++;
}

/*
 * Durch übergebenes Argument als REFERENZ (call-by-reference) kann Inhalt verändert werden!
 *
 */
void test_callByReference(int &i)
{
  i++;
}

/*
 * Some test cases.
 *
 */
int main(int argc, char** argv)
{
  int i = 123;

  cout << "Inhalt via: i = " << i << endl;    // 123

  test_callByValue(i);
  cout << "Inhalt via: i = " << i << endl;    // 123

  test_callByReference(i);
  cout << "Inhalt via: i = " << i << endl;    // 124

  cout << endl;

  return 0;
}