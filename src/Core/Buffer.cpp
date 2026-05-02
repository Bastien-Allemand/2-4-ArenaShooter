#include "Buffer.h"

namespace gce
{
////////////////////////////////////////////////////////////
/// @brief Constructs a Buffer from a memory buffer.
/// @param buffer Pointer to the memory buffer.
/// @param size Size of the buffer in bytes.
////////////////////////////////////////////////////////////
Buffer::Buffer(void const* buffer, uint64 size)
{
	Open(const_cast<void*>(buffer), size);
}

void Buffer::Open(void* buffer, size_t size)
{
	ASSERT(buffer != nullptr, "buffer is empty", ERROR_);

		m_size = size;
	m_cursor = 0; // Reset cursor to the beginning

	void* newBuffer = malloc(size);
	std::memcpy(newBuffer, buffer, size);
	M_PDATA = newBuffer;
}
//////////////////////////////////////////////////////////////
/// @brief Destroys the Buffer and releases resources.
//////////////////////////////////////////////////////////////
Buffer::~Buffer()
{
	Close();
}

///////////////////////////////////////////////////////////////
/// @brief Closes the buffer and releases the data pointer.
/// @return Always returns true.
///////////////////////////////////////////////////////////////
void Buffer::Close()
{
	if (M_PDATA) {
		free(const_cast<void*>(M_PDATA));
		M_PDATA = nullptr;
	}
}

////////////////////////////////////////////
/// @brief Gets the size of the buffer.
/// @return Size of the buffer in bytes.
////////////////////////////////////////////
uint64 Buffer::GetSize() const
{
	ASSERT(M_PDATA != nullptr,"buffer is empty", ERROR_);
	return m_size;
}

/////////////////////////////////////////////////////
/// @brief Reads one byte from the buffer.
/// @return The byte read, or 0 if out of bounds.
/////////////////////////////////////////////////////
ubyte Buffer::ReadByte()
{
	ASSERT(M_PDATA != nullptr, "buffer empty", ERROR_);
	if (m_cursor >= m_size)
	{
		ASSERT(false, "cursor out of range", WARNING);
		return 0;
	}

	ubyte byte = static_cast<ubyte const*>(M_PDATA)[m_cursor];
	m_cursor++;
	return byte;
}

////////////////////////////////////////////////////////////////////////
/// @brief Reads an unsigned 16-bit integer from the buffer.
/// @return The 16-bit unsigned integer read, or 0 if out of bounds.
////////////////////////////////////////////////////////////////////////
uint16 Buffer::LEReadUint16()
{
	ASSERT(M_PDATA != nullptr, "buffer empty", ERROR_);
	if (m_cursor + sizeof(uint16) > m_size)
	{
		ASSERT(false, "cursor out of range", WARNING);
		return 0;
	}

	uint16 value = *reinterpret_cast<uint16 const*>(static_cast<ubyte const*>(M_PDATA) + m_cursor);
	m_cursor += sizeof(uint16);
	return value;
}

////////////////////////////////////////////////////////////////////////
/// @brief Reads an unsigned 32-bit integer from the buffer.
/// @return The 32-bit unsigned integer read, or 0 if out of bounds.
////////////////////////////////////////////////////////////////////////
uint32 Buffer::LEReadUint32()
{

	ASSERT(M_PDATA != nullptr, "buffer empty", ERROR_);
	if (m_cursor + sizeof(uint32) > m_size)
	{
		ASSERT(false, "cursor out of range", WARNING);
		return 0;
	}
	uint32 value = *reinterpret_cast<uint32 const*>(static_cast<ubyte const*>(M_PDATA) + m_cursor);
	m_cursor += sizeof(uint32);
	return value;
}

////////////////////////////////////////////////////////////////////////
/// @brief Reads an unsigned 64-bit integer from the buffer.
/// @return The 64-bit unsigned integer read, or 0 if out of bounds.
////////////////////////////////////////////////////////////////////////
uint64 Buffer::LEReadUint64()
{
	ASSERT(M_PDATA != nullptr, "buffer empty", ERROR_);
	if (m_cursor + sizeof(uint64) > m_size)
	{
		ASSERT(false, "cursor out of range", WARNING);
		return 0;
	}
	uint64 value = *reinterpret_cast<uint64 const*>(static_cast<ubyte const*>(M_PDATA) + m_cursor);
	m_cursor += sizeof(uint64);
	return value;
}

/////////////////////////////////////////////////////////////////////
/// @brief Reads a signed 8-bit integer from the buffer.
/// @return The 8-bit signed integer read, or 0 if out of bounds.
/////////////////////////////////////////////////////////////////////
int8 Buffer::LEReadInt8()
{
	ASSERT(M_PDATA != nullptr, "buffer empty", ERROR_);
	if (m_cursor >= m_size)
	{
		ASSERT(false, "cursor out of range", WARNING);
		return 0;
	}
	int8 value = static_cast<int8 const*>(M_PDATA)[m_cursor];
	m_cursor++;
	return value;
}

//////////////////////////////////////////////////////////////////////
/// @brief Reads a signed 16-bit integer from the buffer.
/// @return The 16-bit signed integer read, or 0 if out of bounds.
//////////////////////////////////////////////////////////////////////
int16 Buffer::LEReadInt16()
{
	ASSERT(M_PDATA != nullptr, "buffer empty", ERROR_);
	if (m_cursor + sizeof(int16) > m_size)
	{
		ASSERT(false, "cursor out of range", WARNING);
		return 0;
	}
	int16 value = *reinterpret_cast<int16 const*>(static_cast<ubyte const*>(M_PDATA) + m_cursor);
	m_cursor += sizeof(int16);
	return value;
}

//////////////////////////////////////////////////////////////////////
/// @brief Reads a signed 32-bit integer from the buffer.
/// @return The 32-bit signed integer read, or 0 if out of bounds.
//////////////////////////////////////////////////////////////////////
int32 Buffer::LEReadInt32()
{
	ASSERT(M_PDATA != nullptr, "buffer empty", ERROR_);
	if (m_cursor + sizeof(int32) > m_size)
	{
		ASSERT(false, "cursor out of range", WARNING);
		return 0;
	}
	int32 value = *reinterpret_cast<int32 const*>(static_cast<ubyte const*>(M_PDATA) + m_cursor);
	m_cursor += sizeof(int32);
	return value;
}

//////////////////////////////////////////////////////////////////////
/// @brief Reads a signed 64-bit integer from the buffer.
/// @return The 64-bit signed integer read, or 0 if out of bounds.
//////////////////////////////////////////////////////////////////////
int64 Buffer::LEReadInt64()
{
	ASSERT(M_PDATA != nullptr, "buffer empty", ERROR_);
	if (m_cursor + sizeof(int64) > m_size)
	{
		ASSERT(false, "cursor out of range", WARNING);
		return 0;
	}
	int64 value = *reinterpret_cast<int64 const*>(static_cast<ubyte const*>(M_PDATA) + m_cursor);
	m_cursor += sizeof(int64);
	return value;
}

/////////////////////////////////////////////////////////////////////////////////////
/// @brief Reads the entire buffer as a string.
/// @return String containing the full buffer content, or empty if not available.
/////////////////////////////////////////////////////////////////////////////////////
String Buffer::FullRead()
{
	ASSERT(M_PDATA != nullptr, "buffer empty", ERROR_);

	String content(static_cast<char8 const*>(M_PDATA), m_size);
	m_cursor = m_size; // Move cursor to the end after reading
	return content;
}

/////////////////////////////////////////////////////////////////
/// @brief Reads a specified number of bytes from the buffer.
/// @param count Number of bytes to read.
/// @return Number of bytes read, or 0 if out of bounds.
/////////////////////////////////////////////////////////////////
uint64 Buffer::Read(uint32 count)
{
	ASSERT(M_PDATA != nullptr, "buffer empty", ERROR_);
	if (m_cursor + count > m_size)
	{
		ASSERT(false, "cursor out of range", WARNING);
		return 0;
	}

	uint64 bytesRead = count;

	m_cursor += count;

	return bytesRead; // Placeholder, adjust according to your buffer type
}
///////////////////////////////////////////////////////////////////////
/// @brief Sets the cursor to the specified position in the buffer.
/// @param position New cursor position.
///////////////////////////////////////////////////////////////////////
void Buffer::SetCursor(uint64 position)
{
	if (M_PDATA == nullptr || position > m_size)
		return; // or throw an error
	m_cursor = position;
}

////////////////////////////////////////////////////////
/// @brief Moves the cursor by the specified offset.
/// @param offset Offset to add to the cursor.
////////////////////////////////////////////////////////
void Buffer::MoveCursor(int64 offset)
{
	if (M_PDATA == nullptr || m_cursor + offset > m_size)
		return; // or throw an error
	m_cursor += offset;
}

//////////////////////////////////////////////////////////////
/// @brief Gets the current cursor position in the buffer.
/// @return Current cursor position.
//////////////////////////////////////////////////////////////
uint64 Buffer::GetCursor() const
{
	if (M_PDATA == nullptr)
		return 0; // or throw an error
	return m_cursor;
}

//////////////////////////////////////////////////////////////////////////
/// @brief Checks if the end of the buffer has been reached.
/// @return True if the cursor is at or past the end, false otherwise.
//////////////////////////////////////////////////////////////////////////
bool Buffer::IsEnd() const
{
	return M_PDATA == nullptr || m_cursor >= m_size;
}

bool Buffer::IsOpened()
{
	return M_PDATA != nullptr;
};
}