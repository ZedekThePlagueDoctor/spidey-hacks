#pragma once

#include "SpideyZ.h"
#include "Patcher.h"

#include <stdint.h>
#include <cstring>
#include <float.h>
#include <string>
#include <windows.h>
#include <iostream>

namespace Logger
{
	void Log(const char* Format, ...);
	void Log(char* Format, ...);
	void Initialize();
	void PatchHandlers(SpideyZ::Patcher patcher);
}
