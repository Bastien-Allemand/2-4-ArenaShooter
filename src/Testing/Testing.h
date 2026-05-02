#ifndef TESTING_TESTING_H_INCLUDED
#define TESTING_TESTING_H_INCLUDED

#include <Console.h>
#include "define.h"
using namespace gce;


template <typename ...Args, class C, typename rT>
bool SelfTest( cstr const name, rT(C::*method)(Args...), C start, Args... args, C const& expected )
{
    (start.*method)(args...);
    Console::Log( name, Console::TEST, "  -  " );
    if ( start == expected )
    {
        Console::Log( "SUCCESS", Console::GREEN );
        return true;
    }
    Console::Log( "FAILED", Console::RED );
    return false;
}

template <typename ...Args, class C, typename rT>
bool ReturnTest( cstr const name, rT(C::*method)(Args...) const, C start, Args... args, rT const& expected )
{
    rT const result = (start.*method)(args...);
    Console::Log( name, Console::TEST, "  -  " );
    if ( result == expected )
    {
        Console::Log( "SUCCESS", Console::GREEN );
        return true;
    }
    Console::Log( "FAILED", Console::RED );
    return false;
}

template <typename ...Args, class C, typename rT>
bool ReturnTest( cstr const name, rT const&(C::*method)(Args...) const, C start, Args... args, rT const& expected )
{
    rT const& result = (start.*method)(args...);
    Console::Log( name, Console::TEST, "  -  " );
    if ( result == expected )
    {
        Console::Log( "SUCCESS", Console::GREEN );
        return true;
    }
    Console::Log( "FAILED", Console::RED );
    return false;
}

inline bool CustomTest( cstr const name, bool(*function)() )
{
    Console::Log( name, Console::TEST, "  -  " );
    if ( function() )
    {
        Console::Log( "SUCCESS", Console::GREEN );
        return true;
    }
    Console::Log( "FAILED", Console::RED );
    return false;
}


#endif