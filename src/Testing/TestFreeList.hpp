#ifndef TEST_FREELIST_HPP_INCLUDED
#define TEST_FREELIST_HPP_INCLUDED

#include <Containers/FreeList.hpp>
#include <Render.h>
#include <Engine.h>
#include "define.h"
using namespace gce;


BEGIN_TEST( FreeList )

    Test const a(1), b(2), c(3), d(4), e(5), f(6), g(7);

    Console::Log("================ FREELIST TEST ===================", Console::GREEN);
    FreeList<Test> fl(5);
    
    uint64 index1 = fl.Put(a);
    uint64 index2 = fl.Put(b);
    uint64 index3 = fl.Put(c);
    uint64 index4 = fl.Put(d);
    uint64 index5 = fl.Put(e);
    uint64 index6 = fl.Put(f);
    uint64 index7 = fl.Put(g);

    for (Test& t : fl)
    {
        Console::Log(t.logA);
    }

    Console::Log("================ REMOVE TEST ===================", Console::GREEN);

    fl.Remove(index2);
    fl.Remove(index4);
    fl.Remove(index6);

    for (Test& t : fl)
    {
        Console::Log(t.logA);
    }

    Console::Log("================ RESIZE TEST ===================", Console::GREEN);

    fl.IsEmpty(3);
    fl.Resize(2);
    fl.Capacity();
    fl.Size();

    for (Test& t : fl)
    {
        Console::Log(t.logA);
    }

    Console::Log("================ CLEAR TEST ===================", Console::GREEN);

    fl.Clear();
    Console::Log(fl.IsEmpty(0u), Console::CYAN);

    for (Test& t : fl)
    {
        Console::Log(t.logA);
    }

END_TEST_MAIN_FUNCTION( FreeList )

struct Test // sizeof(Test)  MUST BE GREATER THAN 4
{
    Test(float32 a) : logA(a)
    {
        Console::Log("Constructor called", Console::GREEN);
    }

    //Test(Test const& other)
    //   {
    //	Console::Log("Copy Constructor called", Console::CYAN);
    //   }

    //Test(Test&& other) noexcept
    //{
    //	Console::Log("Move Constructor called", Console::MAGENTA);
    //}

    ~Test()
    {
        Console::Log("Destructor called", Console::RED);
    }

    float32 logA = 0;
    float32 logB = 0;
};

END_TEST( FreeList )


#endif