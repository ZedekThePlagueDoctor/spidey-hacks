#include "ScopedUnprotect.h"
#include <windows.h>

namespace SpideyZ
{
	static const LPVOID SPID_CODE_START = (LPVOID)0x00401000;
	static const SIZE_T SPID_CODE_SIZE = 0x02A0B000;

    ScopedUnprotect::ScopedUnprotect() : ScopedUnprotect(SPID_CODE_START, SPID_CODE_SIZE) {}

    ScopedUnprotect::ScopedUnprotect(LPVOID addr, SIZE_T length) : addr_(addr), length_(length)
    {
        VirtualProtect(addr_, length_, PAGE_EXECUTE_READWRITE, &oldProtect_);
    }

    ScopedUnprotect::~ScopedUnprotect()
    {
        VirtualProtect(addr_, length_, oldProtect_, &oldProtect_);
    }
}
