#include <iostream>

using namespace std;


void test_getSizeOfArray(int array[])
{
  cout << "test_getSizeOfArray(int array[]): Größe von array: sizeof(array) = " << sizeof(array) << endl;
}

void test_modifyPassedArray(int array[], int length)
{
  for (int i = 0; i < length; i++)
  {
  	array[i] = 4;
  }
}

/*
 * Some test cases.
 *
 */
int main(int argc, char** argv)
{
  cout << endl;

  int arr1[] = {1, 2, 3, 4};

  cout << "        arr1 = " <<        arr1 << " (Adresse des 1. Arrayelementes)" << endl;
  cout << "       &arr1 = " <<       &arr1 << " (Adresse des 1. Arrayelementes)" << endl;
  cout << "       *arr1 = " <<       *arr1 << " (Zellen-Inhalt des 1. Arrayelementes)" << endl;

  cout << "      *&arr1 = " <<      *&arr1 << " (Adresse des 1. Arrayelementes)" << endl;
  cout << "      &*arr1 = " <<      &*arr1 << " (Adresse des 1. Arrayelementes)" << endl;

  cout << "     arr1[0] = " <<     arr1[0] << " (Zellen-Inhalt des 1. Arrayelementes)" << endl;
  cout << "    &arr1[0] = " <<    &arr1[0] << " (Adresse des 1. Arrayelementes)" << endl;
//cout << "    *arr1[0] = " <<    *arr1[0] << " (geht nicht)" << endl;
  cout << "   *&arr1[0] = " <<   *&arr1[0] << " (Zellen-Inhalt des 1. Arrayelementes)" << endl;
  cout << endl;

  cout << " *(arr1 + 1) = " << *(arr1 + 1) << " (Zellen-Inhalt des 1. Arrayelementes)" << endl;
  cout << endl;


  int arr2[] = {1, 2, 3, 4};

  // Größe vom Non-Pointer-Style-Array mit sizeof() ermitteln
  cout << "Größe von arr2: sizeof(arr2) = " << sizeof(arr2) << endl;
  cout << "Anzahl der Elemente von arr2: sizeof(arr2) / sizeof(int) = " << sizeof(arr2) / sizeof(int) << endl;

  // Größe von statischem Array durch Weitergabe ermitteln
  // WICHTIG: Da die Größe des Arrays NICHT übergeben wird, folgt die falsche Ausgabe.
  test_getSizeOfArray(arr2);
  cout << endl;


  int arr3[16];

  // Siehe oben, nur mit kompletter Initialisierung:
  // int arr3[16] = {0};

  // Auch möglich:
  // int* arr3 = new int[16];

  cout << "     arr3 = " << arr3 << " (Adresse des 1. Arrayelementes)" << endl;
  cout << " &arr3[0] = " << &arr3[0] << " (Adresse des 1. Arrayelementes)" << endl;
  cout << "    &arr3 = " << &arr3 << " (Wenn Pointer: Adresse des Pointers - sonst: Adresse des 1. Arrayelementes)" << endl;
  cout << endl;

  // Funktioniert nicht:
  // int* arr3 = NULL;

  cout << "Größe von arr3: sizeof(arr3) = " << sizeof(arr3) << endl;
  test_modifyPassedArray(arr3, 4);

  cout << "arr3[0] = " << arr3[0] << endl;
  cout << "arr3[1] = " << arr3[1] << endl;
  cout << "arr3[2] = " << arr3[2] << endl;
  cout << "arr3[3] = " << arr3[3] << endl;
  cout << "arr3[4] = " << arr3[4] << " (ACHTUNG: uninitialisiert)" << endl;
  cout << "arr3[5] = " << arr3[5] << " (ACHTUNG: uninitialisiert)" << endl;
  cout << endl;

  // Wenn Array-Objekt oben erzeugt wurde, muss der Speicher wieder freigegeben werden:
  // delete[] arr3;


  return 0;
}