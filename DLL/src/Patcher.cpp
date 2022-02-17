#include "Patcher.h"
#include "ScopedUnprotect.h"
#include <algorithm>
#include <vector>
#include <float.h>

namespace SpideyZ
{
	// Dummy func for importer to pick up
	int Patcher::DummyFunc() { return 0; }
	
	bool Patcher::ApplyPatch(const uint8_t *src, uint8_t *dest, uint32_t size)
	{
		ScopedUnprotect memUnprotect(dest, size);
		memcpy(dest, src, size);

		return true;
	}
	
	bool Patcher::WriteNOPs(void *addr, uint32_t length) const
	{
		if (length == 0)
			return true;

		std::vector<uint8_t> nopArray(length);
		memset(&(nopArray[0]), 0x90, length);
		return ApplyPatch(&nopArray[0], reinterpret_cast<uint8_t *>(addr), nopArray.size());
	}
	
	bool Patcher::WriteNull(void *addr, uint32_t length) const
	{
		if (length == 0)
			return true;

		std::vector<uint8_t> nullArray(length);
		memset(&(nullArray[0]), 0x00, length);
		return ApplyPatch(&nullArray[0], reinterpret_cast<uint8_t *>(addr), nullArray.size());
	}
	
	// Write float at a position
	bool Patcher::WriteFloat(void *addr, float value) const
	{
        return Write(addr, value);
	}
	
	// Write Int32 at a position
	bool Patcher::WriteInt32(void *addr, uint32_t value) const
	{
        return Write(addr, value);
	}
	
	// Write Int16 at a position
	bool Patcher::WriteInt16(void *addr, uint16_t value) const
	{
        return Write(addr, value);
	}
	
	// Write Int8 at a position
	bool Patcher::WriteInt8(void *addr, uint8_t value) const
	{
        return Write(addr, value);
	}
	
	// Write array of bytes
	bool Patcher::WriteArray(void *addr, const uint8_t *value, uint32_t length) const
	{
		if (length == 0)
			return true;

		return ApplyPatch(value, reinterpret_cast<uint8_t *>(addr), length);
	}
	
	// Write a char array
	bool Patcher::WriteString(void *addr, const std::string value) const
	{
		const char *cstr = value.c_str();
		ScopedUnprotect memUnprotect(addr, strlen(cstr) + 1);
		
		memcpy(addr, cstr, strlen(cstr) + 1);
		
		return true;
	}
	
	// Write a jmp to a new address
	bool Patcher::WriteJmp(void *addr, const void *jumpDestination) const
	{
		return WriteJmpVariant<0xE9>(addr, jumpDestination); //0xE9 is the x86 JMP instruction
	}
	
	// Call a function
	bool Patcher::WriteCall(void *addr, const void *calleeAddress) const
	{
		return WriteJmpVariant<0xE8>(addr, calleeAddress); //0xE8 is the x86 CALL instruction
	}
}
