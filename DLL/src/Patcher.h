#pragma once

#include "SpideyZ.h"

#include <stdint.h>
#include <cstring>
#include <float.h>
#include <string>

namespace SpideyZ
{
	class SpideyZ_API Patcher final
	{
	public:
		static bool ApplyPatch(const uint8_t * src, uint8_t *dest, uint32_t size);
		
		// Writes an arbitrary type at the given address. 
		// The amount of bytes overwritten will be equal to the size of the type.
        template<typename T>
        bool Write(void *addr, T value) const
        {
            return ApplyPatch(reinterpret_cast<const uint8_t *>(&value), reinterpret_cast<uint8_t *>(addr), sizeof(T));
        }
        
        // Writes a jmp
        template<uint8_t instruction>
        bool WriteJmpVariant(void *addr, const void *jumpDestination) const
        {
            uint8_t buffer[5];
            uint32_t jmpSize = reinterpret_cast<const uint8_t *>(jumpDestination) - reinterpret_cast<uint8_t *>(addr) - 5;

            buffer[0] = instruction;
            memcpy(&buffer[1], &jmpSize, sizeof(jmpSize));

            return ApplyPatch(buffer, reinterpret_cast<uint8_t *>(addr), 5);
        }
        
        // 4-bytes
        bool WriteFloat(void *addr, float value) const;
        
        // 4-bytes
        bool WriteInt32(void *addr, uint32_t value) const;
        
        // 2-bytes
        bool WriteInt16(void *addr, uint16_t value) const;
        
        // 1-byte
        bool WriteInt8(void *addr, uint8_t value) const;
        
        // Jmp to somewhere else
        bool WriteJmp(void *addr, const void *jumpDestination) const;
        
        // Writes call instruction at address
        bool WriteCall(void *addr, const void *calleeAddress) const;
        
        // Writes the contents of an array of bytes
		bool WriteArray(void *addr, const uint8_t *value, uint32_t length) const;
		
		// Writes NOP instructions at a given address.
		bool WriteNOPs(void *addr, uint32_t length) const;
		
		// Writes 0x00's at given address
		bool WriteNull(void *addr, uint32_t length) const;
		
		// Write string at address
		bool WriteString(void *addr, std::string value) const;
		
		static int DummyFunc();
	};
}
