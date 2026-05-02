#ifndef TEST_THREAD_HPP_INCLUDED
#define TEST_THREAD_HPP_INCLUDED

#include <Thread/Thread.h>
#include <Render.h>
#include <Engine.h>
#include "define.h"
using namespace gce;


BEGIN_TEST( Thread )

    //The two thread goes at the same time and one crash after the other one even if the main thread done 

    int time1 = 2000;
    int time2 = 5000;

    Thread thread1;

    thread1.LaunchThread(&time1, Test);

    thread1.LaunchThread(&time1, Test);

    Sleep(1000);

    assert(thread1.CloseThread());

    thread1.LaunchThread(&time1, Test);

    Thread thread2(&time2, Test);

    Sleep(1000);

END_TEST_MAIN_FUNCTION( Thread )

static unsigned long Test(void* pTime)
{
    Sleep(*(int*)pTime);

    Console::Log("Thread terminé",Console::SUCCESS);

    return 0;
}

END_TEST( Thread )


#endif