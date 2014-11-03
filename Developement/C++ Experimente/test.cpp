#include <iostream>

// Include integer type aliases
#include <inttypes.h>

using namespace std;



void nullpointerTest(int test[], int* ptr)
{
    if (!test)
    {
        cout << "test nicht ok" << endl;
    }

    if (!ptr)
    {
        cout << "ptr nicht ok" << endl;
    }

    cout << "nullpointerTest: &test = " << &test << endl;
    cout << "nullpointerTest: ptr = " << ptr << endl;
}

/*
 * Some test cases.
 *
 */
int main(int argc, char** argv)
{
    cout << endl;

    nullpointerTest(NULL, NULL);

    cout << "Tests done!" << endl;
}