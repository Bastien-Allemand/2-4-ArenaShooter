#ifndef CORE_BUFFER_H_INCLUDED  
#define CORE_BUFFER_H_INCLUDED  
#include "Data.h"  
#include "define.h"  

namespace gce
{
	class Buffer : public Data
	{
	public:
		Buffer(void const* buffer, uint64 size);
		void Open(void* buffer, uint64 size);
		~Buffer();
		void Close() override; // No actual closing needed for buffer
		bool IsOpened() override;

		uint64 GetSize() const override;

		ubyte ReadByte() override;

		uint16 LEReadUint16() override;
		uint32 LEReadUint32() override;
		uint64 LEReadUint64() override;
		
		int8 LEReadInt8() override;
		int16 LEReadInt16() override;
		int32 LEReadInt32() override;
		int64 LEReadInt64() override;

		// No BERead methods for Buffer, as it uses little-endian by default // temp not implemented
		uint16 BEReadUint16() override { ASSERT(false, "Buffer does not support BERead methods", ERROR_); return 0; }
		uint32 BEReadUint32() override { ASSERT(false, "Buffer does not support BERead methods", ERROR_); return 0; }
		uint64 BEReadUint64() override { ASSERT(false, "Buffer does not support BERead methods", ERROR_); return 0; }
		int8 BEReadInt8() override { ASSERT(false, "Buffer does not support BERead methods", ERROR_); return 0; }
		int16 BEReadInt16() override { ASSERT(false, "Buffer does not support BERead methods", ERROR_); return 0; }
		int32 BEReadInt32() override { ASSERT(false, "Buffer does not support BERead methods", ERROR_); return 0; }
		int64 BEReadInt64() override { ASSERT(false, "Buffer does not support BERead methods", ERROR_); return 0; }

		String FullRead() override;
		uint64 Read(uint32 size);

		void SetCursor(uint64 position) override;
		void MoveCursor(int64 offset) override;
		uint64 GetCursor() const override;

		bool IsEnd() const override;

	private:
		void* M_PDATA = nullptr;
		uint64 m_size = 0;
		uint64 m_cursor = 0;
	};
}
#endif
