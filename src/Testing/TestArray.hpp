#ifndef TEST_ARRAY_HPP_INCLUDED
#define TEST_ARRAY_HPP_INCLUDED

#include <Containers/Array.hpp>
#include "define.h"
using namespace gce;


BEGIN_TEST( Array )

    std::cout << "===== Test Array =====" << std::endl;

    // Test constructors and initialisations
    Array<int, 5> arr1;
    bool defaultConstructorOk = true;
    for (uint64 i = 0; i < arr1.size(); ++i) {
        if (arr1[i] != 0) {
            defaultConstructorOk = false;
            break;
        }
    }
    PrintTestResult("Default Constructor", defaultConstructorOk);

    Array<int, 5> arr2(arr1);
    bool copyConstructorOk = arr2 == arr1;
    PrintTestResult("Copy Constructor", copyConstructorOk);

    Array<int, 3> arr3 = { 1, 2, 3 };
    bool valueConstructorOk = true;
    for (uint64 i = 0; i < arr3.size(); ++i) {
        if (arr1[i] != i+1) {
            defaultConstructorOk = false;
            break;
        }
    }
    PrintTestResult("Constructor with a specific value", valueConstructorOk);

    // Test Accessor
    arr1[0] = 10;
    arr1[1] = 20;
    arr1[2] = 30;
    arr1[3] = 40;
    arr1[4] = 50;

    bool accessorOk = (arr1[0] == 10 && arr1[1] == 20 && arr1[2] == 30 &&
        arr1[3] == 40 && arr1[4] == 50);
    PrintTestResult("Accessor []", accessorOk);

    // Test At()
    bool atOk = (arr1.at(2) == 30 );
    PrintTestResult("At()", atOk);

    // Test Front() et Back()
    bool frontBackOk = (arr1.front() == 10 && arr1.back() == 50);
    PrintTestResult("Front() et Back()", frontBackOk);

    // Test Data()
    int* data = arr1.data();
    bool dataOk = (data[0] == 10 && data[1] == 20 && data[2] == 30);
    PrintTestResult("Data()", dataOk);

    // Test Empty() et Size()
    Array<float, 0> emptyArr;
    Array<float, 3> nonEmptyArr;
    bool emptySizeOk = (emptyArr.empty() && !nonEmptyArr.empty() &&
        emptyArr.size() == 0 && nonEmptyArr.size() == 3);
    PrintTestResult("Empty() et Size()", emptySizeOk);

    // Test Fill()
    Array<int, 3> fillArr;
    fillArr.fill(42);
    bool fillOk = (fillArr[0] == 42 && fillArr[1] == 42 && fillArr[2] == 42);
    PrintTestResult("Fill()", fillOk);

    // Test Swap()
    Array<int, 3> swapArr1;
    Array<int, 3> swapArr2;
    swapArr1.fill(1);
    swapArr2.fill(2);
    swapArr1.swap(swapArr2);
    bool swapOk = (swapArr1[0] == 2 && swapArr2[0] == 1);
    PrintTestResult("Swap()", swapOk);

    // Test Iterators
    Array<char, 4> iterArr;
    iterArr[0] = 'a'; iterArr[1] = 'b'; iterArr[2] = 'c'; iterArr[3] = 'd';

    std::string forwardIter;
    for (auto it = iterArr.begin(); it != iterArr.end(); ++it) {
        forwardIter += *it;
    }

    std::string reverseIter;
    for (auto it = iterArr.rbegin(); it != iterArr.rend(); --it) {
        reverseIter += *it;
    }

    bool iterOk = (forwardIter == "abcd" && reverseIter == "dcba");
    PrintTestResult("Iterators", iterOk);

    // Test comparators
    Array<int, 3> compArr1;
    Array<int, 3> compArr2;
    Array<int, 3> compArr3;
    compArr1.fill(5);
    compArr2.fill(5);
    compArr3.fill(10);

    bool eqOk = (compArr1 == compArr2 && !(compArr1 == compArr3));
    PrintTestResult("operator==", eqOk);

    bool neOk = (compArr1 != compArr3 && !(compArr1 != compArr2));
    PrintTestResult("operator!=", neOk);


    // Test ToArray()
    int fillArr2[] = { 1,3,5 };
	//auto list = Array<int, 3>::ToArray( 1, 2, 3 );
    // auto list2 = Array( { fillArr2 } );

    //if (list[0] == 1 && list[1] == 2 && list[2] == 3) 
    //{
    //    PrintTestResult("ToArray() with multiple elements", true);
    //}
    //else 
    //{
    //    PrintTestResult("ToArray() with multiple elements", false);
    //}

    // if (list2[0] == fillArr2[0] && list2[1] == fillArr2[1] && list2[2] == fillArr2[2]) 
    // {
        // PrintTestResult("ToArray() with C-style array", true);
    // }
    // else 
    // {
        // PrintTestResult("ToArray() with C-style array", false);
    // }

    // Test Get()
    Array<int, 3> arr = { 1, 2, 3 };
	arr[0] = 1;
    auto lol = arr[2];

    if (lol == 3) 
    {
        PrintTestResult("Get() function", true);
    }
    else 
    {
        PrintTestResult("Get() function", false);
    }

    std::cout << "===== End Test =====" << std::endl;

END_TEST_MAIN_FUNCTION( Array )

static void PrintTestResult(const char* testName, bool success) { std::cout << (success ? "[OK] " : "[ERREUR] ") << testName << std::endl; }

END_TEST( Array )


#endif