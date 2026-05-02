#ifndef TEST_BUFFER_HPP_INCLUDED
#define TEST_BUFFER_HPP_INCLUDED

#include <File.h>
#include <Buffer.h>
#include "define.h"
using namespace gce;


BEGIN_TEST( Buffer )

    Data* file;
    file = new File(RES_PATH"res/Test/BONJOUR.txt", File::OVER_WRITE);
    file->Write("Test THIS BUFFER");
    file->Close();
    file = new File(RES_PATH"res/Test/BONJOUR.txt",File::READ_ONLY);

    String buffer = file->FullRead();
    file->Close();
    file = new Buffer(buffer.data(), buffer.size());
    file->Write(buffer);
    file->SetCursor(0);
    buffer = file->FullRead();
    file->Close();
    file = new File(RES_PATH"res/Test/BONJOUR.txt", File::OVER_WRITE);
    file->Write(buffer);
    file->Close();

END_TEST_MAIN_FUNCTION( Buffer )
END_TEST( Buffer )


#endif