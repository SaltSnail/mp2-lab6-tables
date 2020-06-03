// runtime_error.cpp
// compile with: /EHsc /GR
#include <iostream>
#include <typeinfo>
#include <stdio.h> 
#include <string>
#include <time.h>
#include "Table.h"

using namespace std;
class PositiveNumber
{
    int positiveNumber;

public:
    PositiveNumber(int num) :positiveNumber(num)
    {
        if (num < 0)
            throw exception("error");
    }
};
int main()
{
    /*
    //string a = "1";
    int a = -1;
    auto& ti1 = typeid(a);
    cout << ti1.name() << endl;
    // runtime_error
    try
    {
        PositiveNumber n(a);
    }
    catch (exception &e)
    {
        cerr << "Caught " << e.what() << endl;
        cerr << "Type " << typeid(e).name() << endl;
    };
    Table<int, int> test1(3);
    test1.Add(0, 3);
    test1.Add(1, 1);
    test1.Add(2, 2);
    for (int i = 0; i < 3; i++)
        cout << test1.Find(i) << " ";
    SortTable<int, int> sortTest1(test1);
    sortTest1.Delete(1);
    try { sortTest1.Find(1); }
    catch (...)
    {
        cerr << "\nCaught exeption\n";
    }
    cout << sortTest1.Find(0) << endl;
    
    */

    HashTable<int, int> ht(10);
    ht.Add(1, 11);
    ht.Add(2, 22);
    cout << *ht.Find(1);
    cout << endl << *ht.Find(2);
    //ht.Delete(1);
    
}