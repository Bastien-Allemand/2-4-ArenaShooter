#ifndef TEST_FILE_HPP_INCLUDED
#define TEST_FILE_HPP_INCLUDED

#include <File.h>
#include "define.h"
using namespace gce;


BEGIN_TEST( File )

    File file(RES_PATH"res/Test/Test.txt", File::READ_ONLY);
                       
    std::string testing = "0";
                       
    if (!file.IsOpened())
        testing += ", 2"; // File is not opened
                       
    char test = file.ReadByte();
    if (test != 'T')
        testing += ", 3"; // First byte is not 'T'
                       
    int test2 = file.GetCursor();
    if (test2 != 1)
        testing += ", 4"; // Cursor is not at position 1
                       
    // file.AddCursor(1);
    // if (file.GetCursor() != 2)
        // testing += ", 5"; // Failed to add cursor position by 1
                       
    uint64 size = file.GetSize();
    if (size != 46)
        testing += ", 6"; // File is empty
                       
    file.SetCursor(size);
                   	    
    if (file.GetCursor() != size)
        testing += ", 7";
                       
    if (file.IsEnd())
        testing += ", 8"; // File is not at end
                       
    file.SetCursor(0);
    std::string line = file.ReadLine();
    if (line != "Test1\n")
        testing += ", 9"; // Failed to read line
                       
    file.Close();

END_TEST_MAIN_FUNCTION( File )
END_TEST( File )


BEGIN_SUBTEST( File, Binary )

    Data* data;
    data = new File(RES_PATH"/res/Test/caow.png", File::READ_ONLY);
    std::string testing = "0";
    std::string buffer;
    uint64 ZEZE = data->GetSize();

    for (uint64 i = 0; i < ZEZE; i++)
    {
        buffer += data->ReadByte();
    }
    data->Close();

    File file(RES_PATH"/res/Test/Test1.font", File::OVER_WRITE);

    uint64 EER = file.Write(buffer + "BAHAHHAHA");
    file.Close();

END_SUBTEST( Binary )


BEGIN_SUBTEST( File, ReadAndWriteBinaryConverter )

    String path = RES_PATH"res/Test/Test1.txt";
    File file(path,File::Type::READ_ONLY);

    file.Write("iyuyi");

    file.Close();

    Data* Data;
    Data = new File(RES_PATH"res/Test/Test1.txt",File::READ_ONLY);

    char buffer[5];
    file.Read<char>(buffer,5);

    file.Close();

END_SUBTEST( ReadAndWriteBinaryConverter )


#endif