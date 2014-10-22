#include <iostream>
using namespace std;


/*
 * Durch übergebenes Argument als REFERENZ kann Inhalt verändert werden!
 * Call-By-Reference
 *
 */
void test_modifyReferencePassedVariable(int &i)
{
  cout << "test_modifyReferencedVariable(&i): Inhalt via: i = " << i << endl;
  i++;
}

/*
 * Some test cases.
 *
 */
int main(int argc, char** argv)
{
  int i = 123;

  // Übergebe Integer als Referenz
  cout << "Inhalt via: i = " << i << endl;    // 123
  test_modifyReferencePassedVariable(i);
  cout << "Inhalt via: i = " << i << endl;    // 124
  cout << endl;

  return 0;
}