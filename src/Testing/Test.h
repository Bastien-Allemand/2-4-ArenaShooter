#include <windows.h>
#include "define.h"
#include <Console.h>

#include STRINGIFY( CAT_3( Test, TEST, .hpp ) )

#define EXPAND(X) X##1
#define XEXPAND(X) EXPAND(X)

#if XEXPAND( SUBTEST ) == false

#define RUN_TEST \
    int32 APIENTRY WinMain( HINSTANCE, HINSTANCE, PSTR, int32 ) \
    { \
        gce::Console::Init(); \
        CAT_4( Test, TEST, _, SUBTEST )::Run(); \
        return 0; \
    }

#else

#define RUN_TEST \
    int32 APIENTRY WinMain( HINSTANCE, HINSTANCE, PSTR, int32 ) \
    { \
        gce::Console::Init(); \
        CAT_2( Test, TEST )::Run(); \
        return 0; \
    }

#endif
