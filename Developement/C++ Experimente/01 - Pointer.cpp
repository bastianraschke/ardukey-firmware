#include <iostream>

using namespace std;


/*
 * Es wird ein Zeiger übergeben, mit dem man direkt bis zum Aufrufer rumspielen kann.
 *
 */
void test_modifyPassedPointer(int* ptr)
{
  cout << "test_modifyPassedPointer(*ptr): *ptr = " << *ptr << " (der Zellen-Inhalt von ptr)" << endl;
  cout << "test_modifyPassedPointer(*ptr):  ptr = " <<  ptr << " (die Adresse von ptr)" << endl;

  // ACHTUNG: Klammern sind wichtig, damit der Inhalt und NICHT die Adresse geändert wird!
  *ptr = (*ptr) + 1;
  (*ptr)++;
}

/*
 * Some test cases.
 *
 */
int main(int argc, char** argv)
{
  cout << endl;

  // Zuweisung eines einfachen Integers auf i:
  int i = 123;

  cout << "   i = " <<   i << " (der Zellen-Inhalt von i)" << endl;   // Gebe den Zellen-Inhalt von i = Wert von i
  cout << "  &i = " <<  &i << " (die Adresse von i)" << endl;         // Gebe die Adresse von i = Adresse von i
  cout << endl;

  // Zeiger auf eine Adresse (Verlinkung) einer Integer-Variable:
  // MERKE: Ein Variablen-Inhalt (außer NULL => Null-Zeiger) ist einem Zeiger nicht zuweisbar!
  int* j = &i;

  cout << "   j = " <<   j << " (die Adresse von i)" << endl;         // Gebe den Zellen-Inhalt von j = Adresse von i
  cout << "  &j = " <<  &j << " (die Adresse von j)" << endl;         // Gebe die Adresse von j = Adresse von j
  cout << "  *j = " <<  *j << " (der Zellen-Inhalt von i)" << endl;   // Gebe den Zellen-Inhalt auf das der Zeiger zeigt = Wert von i
  cout << endl;

  cout << " &*j = " << &*j << " (die Adresse von i)" << endl;         // Gebe die Adresse des Zellen-Inhalts auf das der Zeiger zeigt = Adresse von i
  cout << " *&j = " << *&j << " (die Adresse von i)" << endl;         // -||-
  cout << endl;

  // Hinweis: Das & (Adress-Zeichen) und * (Deferenzierung) heben sich immer gegenseitig auf!

  // Zeiger auf einen Zeiger auf eine Adresse einer Integer-Variable:
  int** k = &j;

  cout << "  *k = " <<  *k << " (die Adresse von i)" << endl;
  cout << " **k = " << **k << " (der Zellen-Inhalt von i)" << endl;   // Gebe den Zellen-Inhalt auf das der Zeiger zeigt = Wert von i
  cout << endl;


  // Zuweisung eines einfachen Integers auf a und ein Zeiger b auf a
  int a = 456;
  int* b = &a;

  cout << "   a = " <<   a << " (der Zellen-Inhalt von a)" << endl;   // Gebe den Zellen-Inhalt von a = Wert von a
  cout << "  *b = " <<  *b << " (der Zellen-Inhalt von a)" << endl;   // Gebe den Zellen-Inhalt auf das der Zeiger zeigt = Wert von a
  test_modifyPassedPointer(b);
  cout << "   a = " <<   a << " (der Zellen-Inhalt von a)" << endl;   // Gebe den Zellen-Inhalt von a = Wert von a
  cout << endl;

  return 0;
}