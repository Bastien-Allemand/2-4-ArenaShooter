#include "File.h"
#include <iostream>
#include <bitset>
#include <cstdio>

namespace gce
{
//////////////////////////////////////////////////////////////
/// @brief Opens a file with the specified path and type.
/// @param path Path to the file to open.
/// @param type Opening mode of the file.
//////////////////////////////////////////////////////////////
File::File()
{
}
File::File(String const& path, Type type)
{
	Open(path, type);
}
bool File::Open(String const& path, Type type)
{
	m_openType = type;
	char8 const* mode = "   "; // ini
	switch (type)
	{
	case File::READ_ONLY:
		mode = "rb";
		break;
	case File::READ_AND_WRITE:
		mode = "rb+";
		break;
	case File::OVER_WRITE:
		mode = "wb";
		break;
	case File::READ_AND_APPEND:
		mode = "ab+";
		break;
	default:
		ASSERT(false, "Invalid file type specified.",ERROR_);
	}


	if (m_pContent != nullptr)
		freopen_s(&m_pContent, path.c_str(), mode, m_pContent);
	else
		fopen_s(&m_pContent, path.c_str(), mode);
	return true;
}
//////////////////////////////////////////////////////////////////////////
/// @brief Destroys the File object and closes the file if it is open.
//////////////////////////////////////////////////////////////////////////
File::~File() 
{
	if (m_pContent != nullptr)
		Close();
}
///////////////////////////////////////////////////////////////////////////////////
/// @brief Closes the file if it is currently open.
/// @return True if the file was successfully closed, false if it was not open.
///////////////////////////////////////////////////////////////////////////////////
void File::Close()
{
	ASSERT(m_pContent != nullptr, "File pointer is null. Cannot close file",ERROR_);

	fclose(m_pContent);
	m_pContent = nullptr;
}

/////////////////////////////////////////
/// @brief Gets the size of the file.
/// @return Size of the file in bytes.
/////////////////////////////////////////
uint64 File::GetSize() const
{
	ASSERT(m_pContent != nullptr, "File pointer is null. Cannot get size.",ERROR_);

	long old = ftell(m_pContent);

	fseek(m_pContent, 0, SEEK_END);
	uint64 size = ftell(m_pContent);

	fseek(m_pContent, old, SEEK_SET);

	return size;
}

/////////////////////////////////////////////////////////////////////////
/// @brief Reads a line from the file.
/// @return String of the line read, including the newline character.
/////////////////////////////////////////////////////////////////////////
String File::ReadLine()
{
	ASSERT(m_openType != Type::OVER_WRITE, "Cannot read line in OVER_WRITE mode.",ERROR_);
	ASSERT(m_pContent != nullptr, "File pointer is null. Cannot read line.",ERROR_);

	String line;
	char8 ch;
	while (fread(&ch, sizeof(char8), 1, m_pContent) == 1 && ch != '\n' && ch != '\0')
	{
		if (ch == '\r') continue;
		line += ch;
	}

	return line;
}

//////////////////////////////////////////
/// @brief Reads 1 byte from the file.
/// @return Byte read.
//////////////////////////////////////////
ubyte File::ReadByte()
{
	ASSERT(m_openType != Type::OVER_WRITE, "ReadByte not allowed in OVER_WRITE mode.",ERROR_);
	ASSERT(m_pContent != nullptr, "File pointer is null. Cannot read byte.",ERROR_);
	ubyte byte = 0;
	ASSERT(fread(&byte, sizeof(ubyte), 1, m_pContent) == 1, "Failed to read",ERROR_)

	return byte;
}

//----------------------------------------------------------------------------------in Big-endian---------------------------------------------------------------//

//////////////////////////////////////////////////////////////
/// @brief Reads an unsigned 16-bit integer from the file in Big-endian.
/// @return 16-bit unsigned integer value.
//////////////////////////////////////////////////////////////
uint16 File::BEReadUint16()
{
	ASSERT(m_openType != Type::OVER_WRITE, "ReadUint16 not allowed in OVER_WRITE mode.",ERROR_);
	ASSERT(m_pContent != nullptr, "File pointer is null. Cannot read uint16.",ERROR_);
	ubyte bytes[2];
	ASSERT(fread(bytes, sizeof(ubyte), 2, m_pContent) == 2, "Failed to read",ERROR_);

	return (bytes[0] << 8) | bytes[1];
}

//////////////////////////////////////////////////////////////
/// @brief Reads an unsigned 32-bit integer from the file in Big-endian.
/// @return 32-bit unsigned integer value.
//////////////////////////////////////////////////////////////
uint32 File::BEReadUint32()
{
	ASSERT(m_openType != Type::OVER_WRITE, "ReadUint32 not allowed in OVER_WRITE mode.",ERROR_);
	ASSERT(m_pContent != nullptr, "File pointer is null. Cannot read uint32.",ERROR_);
	ubyte bytes[4];
	ASSERT(fread(bytes, sizeof(ubyte), 4, m_pContent) == 4, "Failed to read", ERROR_);

	return (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
}

//////////////////////////////////////////////////////////////
/// @brief Reads an unsigned 64-bit integer from the file in Big-endian.
/// @return 64-bit unsigned integer value.
//////////////////////////////////////////////////////////////
uint64 File::BEReadUint64()
{
	ASSERT(m_openType != Type::OVER_WRITE, "ReadUint64 not allowed in OVER_WRITE mode.", ERROR_);
	ASSERT(m_pContent != nullptr, "File pointer is null. Cannot read uint64.", ERROR_);
	ubyte bytes[8];
	ASSERT(fread(bytes, sizeof(ubyte), 8, m_pContent) == 8, "Failed to read", ERROR_);
	return
		(static_cast<uint64>(bytes[0]) << 56) | (static_cast<uint64>(bytes[1]) << 48) |
		(static_cast<uint64>(bytes[2]) << 40) | (static_cast<uint64>(bytes[3]) << 32) |
		(static_cast<uint64>(bytes[4]) << 24) | (static_cast<uint64>(bytes[5]) << 16) |
		(static_cast<uint64>(bytes[6]) << 8) | static_cast<uint64>(bytes[7]);
}

//////////////////////////////////////////////////////////
/// @brief Reads a signed 8-bit integer from the file in Big-endian.
/// @return 8-bit signed integer value.
//////////////////////////////////////////////////////////
int8 File::BEReadInt8()
{
	ASSERT(m_openType != Type::OVER_WRITE, "ReadInt8 not allowed in OVER_WRITE mode.", ERROR_);
	ASSERT(m_pContent != nullptr, "File pointer is null. Cannot read int8.", ERROR_);
	ubyte byte;
	ASSERT(fread(&byte, sizeof(ubyte), 1, m_pContent) == 1, "Failed to read", ERROR_);
	return static_cast<int8>(byte);
}

///////////////////////////////////////////////////////////
/// @brief Reads a signed 16-bit integer from the file in Big-endian.
/// @return 16-bit signed integer value.
///////////////////////////////////////////////////////////
int16 File::BEReadInt16()
{
	ASSERT(m_openType != Type::OVER_WRITE, "ReadInt16 not allowed in OVER_WRITE mode.", ERROR_);
	ASSERT(m_pContent != nullptr, "File pointer is null. Cannot read int16.", ERROR_);
	ubyte bytes[2];
	ASSERT(fread(bytes, sizeof(ubyte), 2, m_pContent) == 2, "Failed to read", ERROR_);
	return static_cast<int16>((bytes[0] << 8) | bytes[1]);
}

///////////////////////////////////////////////////////////
/// @brief Reads a signed 32-bit integer from the file in Big-endian.
/// @return 32-bit signed integer value.
///////////////////////////////////////////////////////////
int32 File::BEReadInt32()
{
	ASSERT(m_openType != Type::OVER_WRITE, "ReadInt32 not allowed in OVER_WRITE mode.", ERROR_);
	ASSERT(m_pContent != nullptr, "File pointer is null. Cannot read int32.", ERROR_);
	ubyte bytes[4];
	ASSERT(fread(bytes, sizeof(ubyte), 4, m_pContent) == 4, "Failed to read", ERROR_);
	return static_cast<int32>(bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
}

///////////////////////////////////////////////////////////
/// @brief Reads a signed 64-bit integer from the file in Big-endian.
/// @return 64-bit signed integer value.
///////////////////////////////////////////////////////////
int64 File::BEReadInt64()
{
	ASSERT(m_openType != Type::OVER_WRITE, "ReadInt64 not allowed in OVER_WRITE mode.", ERROR_);
	ASSERT(m_pContent != nullptr, "File pointer is null. Cannot read int64.", ERROR_);
	ubyte bytes[8];
	ASSERT(fread(bytes, sizeof(ubyte), 8, m_pContent) == 8, "Failed to read", ERROR_);
	return
		(static_cast<int64>(bytes[0]) << 56) | (static_cast<int64>(bytes[1]) << 48) |
		(static_cast<int64>(bytes[2]) << 40) | (static_cast<int64>(bytes[3]) << 32) |
		(static_cast<int64>(bytes[4]) << 24) | (static_cast<int64>(bytes[5]) << 16) |
		(static_cast<int64>(bytes[6]) << 8) | static_cast<int64>(bytes[7]);
}

//----------------------------------------------------------------------------------in little-endian---------------------------------------------------------------//

//////////////////////////////////////////////////////////////
/// @brief Reads an unsigned 16-bit integer from the file in little-endian.
/// @return 16-bit unsigned integer value.
//////////////////////////////////////////////////////////////
uint16 File::LEReadUint16()
{
	ASSERT(m_openType != Type::OVER_WRITE, "ReadUint16 not allowed in OVER_WRITE mode.", ERROR_);
	ASSERT(m_pContent != nullptr, "File pointer is null. Cannot read uint16.", ERROR_);
	ubyte bytes[2];
	ASSERT(fread(bytes, sizeof(ubyte), 2, m_pContent) == 2, "Failed to read", ERROR_);

	return static_cast<uint16>(bytes[0] | (bytes[1] << 8));
}

//////////////////////////////////////////////////////////////
/// @brief Reads an unsigned 32-bit integer from the file in little-endian.
/// @return 32-bit unsigned integer value.
//////////////////////////////////////////////////////////////
uint32 File::LEReadUint32()
{
	ASSERT(m_openType != Type::OVER_WRITE, "ReadUint32 not allowed in OVER_WRITE mode.", ERROR_);
	ASSERT(m_pContent != nullptr, "File pointer is null. Cannot read uint32.", ERROR_);
	ubyte bytes[4];
	ASSERT(fread(bytes, sizeof(ubyte), 4, m_pContent) == 4, "Failed to read", ERROR_);

	return static_cast<uint32>(
		(bytes[0]) |
		(bytes[1] << 8) |
		(bytes[2] << 16) |
		(bytes[3] << 24));
}

//////////////////////////////////////////////////////////////
/// @brief Reads an unsigned 64-bit integer from the file in little-endian.
/// @return 64-bit unsigned integer value.
//////////////////////////////////////////////////////////////
uint64 File::LEReadUint64()
{
	ASSERT(m_openType != Type::OVER_WRITE, "ReadUint64 not allowed in OVER_WRITE mode.", ERROR_);
	ASSERT(m_pContent != nullptr, "File pointer is null. Cannot read uint64.", ERROR_);
	ubyte bytes[8];
	ASSERT(fread(bytes, sizeof(ubyte), 8, m_pContent) == 8, "Failed to read", ERROR_);
	return
		(static_cast<uint64>(bytes[0])) |
		(static_cast<uint64>(bytes[1]) << 8) |
		(static_cast<uint64>(bytes[2]) << 16) |
		(static_cast<uint64>(bytes[3]) << 24) |
		(static_cast<uint64>(bytes[4]) << 32) |
		(static_cast<uint64>(bytes[5]) << 40) |
		(static_cast<uint64>(bytes[6]) << 48) |
		(static_cast<uint64>(bytes[7]) << 56);
}

//////////////////////////////////////////////////////////
/// @brief Reads a signed 8-bit integer from the file in little-endian.
/// @return 8-bit signed integer value.
//////////////////////////////////////////////////////////
int8 File::LEReadInt8()
{
	ASSERT(m_openType != Type::OVER_WRITE, "ReadInt8 not allowed in OVER_WRITE mode.", ERROR_);
	ASSERT(m_pContent != nullptr, "File pointer is null. Cannot read int8.", ERROR_);
	ubyte byte;
	ASSERT(fread(&byte, sizeof(ubyte), 1, m_pContent) == 1, "Failed to read", ERROR_);
	return static_cast<int8>(byte);
}

///////////////////////////////////////////////////////////
/// @brief Reads a signed 16-bit integer from the file in little-endian.
/// @return 16-bit signed integer value.
///////////////////////////////////////////////////////////
int16 File::LEReadInt16()
{
	ASSERT(m_openType != Type::OVER_WRITE, "ReadInt16 not allowed in OVER_WRITE mode.", ERROR_);
	ASSERT(m_pContent != nullptr, "File pointer is null. Cannot read int16.", ERROR_);
	ubyte bytes[2];
	ASSERT(fread(bytes, sizeof(ubyte), 2, m_pContent) == 2, "Failed to read", ERROR_);
	int16 value = 0;
	value |= bytes[0];
	value |= (bytes[1] << 8);
	return static_cast<int16>(value);

}

///////////////////////////////////////////////////////////
/// @brief Reads a signed 32-bit integer from the file in little-endian.
/// @return 32-bit signed integer value.
///////////////////////////////////////////////////////////
int32 File::LEReadInt32()
{
	ASSERT(m_openType != Type::OVER_WRITE, "ReadInt32 not allowed in OVER_WRITE mode.", ERROR_);
	ASSERT(m_pContent != nullptr, "File pointer is null. Cannot read int32.", ERROR_);
	ubyte bytes[4];
	ASSERT(fread(bytes, sizeof(ubyte), 4, m_pContent) == 4, "Failed to read", ERROR_);
	return static_cast<int32>(
		(bytes[0]) |
		(bytes[1] << 8) |
		(bytes[2] << 16) |
		(bytes[3] << 24));
}

///////////////////////////////////////////////////////////
/// @brief Reads a signed 64-bit integer from the file in little-endian.
/// @return 64-bit signed integer value.
///////////////////////////////////////////////////////////
int64 File::LEReadInt64()
{
	ASSERT(m_openType != Type::OVER_WRITE, "ReadInt64 not allowed in OVER_WRITE mode.", ERROR_);
	ASSERT(m_pContent != nullptr, "File pointer is null. Cannot read int64.", ERROR_);
	ubyte bytes[8];
	ASSERT(fread(bytes, sizeof(ubyte), 8, m_pContent) == 8, "Failed to read", ERROR_);
	return
		(static_cast<int64>(bytes[0])) |
		(static_cast<int64>(bytes[1]) << 8) |
		(static_cast<int64>(bytes[2]) << 16) |
		(static_cast<int64>(bytes[3]) << 24) |
		(static_cast<int64>(bytes[4]) << 32) |
		(static_cast<int64>(bytes[5]) << 40) |
		(static_cast<int64>(bytes[6]) << 48) |
		(static_cast<int64>(bytes[7]) << 56);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Reads the entire content of the file as a string.
/// @return String containing the full content of the file, or an empty string if the file is not open.
///////////////////////////////////////////////////////////////////////////////////////////////////////////
String File::FullRead()
{
	ASSERT(m_openType != Type::OVER_WRITE, "FullRead not allowed in OVER_WRITE mode.", ERROR_);
	ASSERT(m_pContent != nullptr, "File pointer is null. Cannot read content.", ERROR_);


	String content;
	char8 ch;
	// Read until EOF
	while (fread(&ch, sizeof(char8), 1, m_pContent) == 1)
	{
		content += ch;
	}
	return content;
}

///////////////////////////////////////////
/// @brief Writes a string to the file.
/// @param insert String to write.
/// @return Number of bytes written.
///////////////////////////////////////////
size_t File::Write(String const& insert)
{
	ASSERT(m_openType != Type::READ_ONLY, "Write(String) not allowed in READ_ONLY mode.", ERROR_);

	size_t written = fwrite(insert.c_str(), sizeof(char8), insert.size(), m_pContent);
	return written;
}

//////////////////////////////////////////////////
/// @brief Writes a binary string to the file.
/// @param buffer Binary string to write.
//////////////////////////////////////////////////
void File::Write(void* data, size_t byteCount)
{
	ASSERT(m_openType != Type::READ_ONLY, "Write(void*) not allowed in READ_ONLY mode.", ERROR_);
	ASSERT(m_pContent != nullptr, "File pointer is null. Cannot write data.", ERROR_);
	size_t size = fwrite(data, 1, byteCount, m_pContent);
}

////////////////////////////////////////////////////
/// @brief Deletes a file at the specified path.
////////////////////////////////////////////////////
bool File::DeleteFile(char8 const* path)
{
	if (remove(path) != 0) return false;
	else return true;
}

////////////////////////////////////////////////////////////////////
/// @brief Rewinds the file cursor to the beginning of the file.
////////////////////////////////////////////////////////////////////
void File::RewindCursor(){	rewind(m_pContent);}

//////////////////////////////////////////////////////////////
/// @brief Sets the file cursor to the specified position.
/// @param position Cursor position.
//////////////////////////////////////////////////////////////
void File::SetCursor(uint64 position)
{
	ASSERT(m_pContent != nullptr, "File pointer is null. Cannot set cursor position.", ERROR_);
	fseek(m_pContent, position, SEEK_SET);
}

///////////////////////////////////////////////////////
/// @brief Moves the file cursor by a given offset.
/// @param offset Offset to add to the cursor.
///////////////////////////////////////////////////////
void File::MoveCursor(int64 offset)
{
	ASSERT(m_pContent != nullptr, "File pointer is null. Cannot move cursor.", ERROR_);
	fseek(m_pContent, offset, SEEK_CUR);
}

////////////////////////////////////////////////////////////
/// @brief Gets the current cursor position in the file.
/// @return Cursor position.
////////////////////////////////////////////////////////////
uint64 File::GetCursor() const
{
	ASSERT(m_pContent != nullptr, "File pointer is null. Cannot get cursor position.", ERROR_);

	int64 pos = ftell(m_pContent);
	if (pos < 0)
	{
		std::cerr << "Error getting cursor position\n";
		return 0;
	}
	return static_cast<uint64>(pos);
}

////////////////////////////////////////////////////////////////////////
/// @brief Indicates if the end of the file has been reached.
/// @return True if the end of the file is reached, false otherwise.
////////////////////////////////////////////////////////////////////////
bool File::IsEnd() const
{
	if (m_pContent == nullptr)
		return true;
	return feof(m_pContent) != 0;
}

}