#pragma once

#include "SpideyZ.h"
#include <windows.h>

namespace SpideyZ
{
    /// A class that will make protected memory (such as code segments) writable while it is in scope.
    class SpideyZ_API ScopedUnprotect final
    {
    private:
        DWORD oldProtect_;
        LPVOID addr_;
        SIZE_T length_;
    public:
        ScopedUnprotect();
        ScopedUnprotect(LPVOID addr, SIZE_T length);
        ~ScopedUnprotect();
    };
}
