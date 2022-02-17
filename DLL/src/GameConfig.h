#pragma once

#include <cstdint>
#include "Patcher.h"

namespace GameConfig
{
	void Initialize();
	
	uint32_t GetValue(const char* appName, const char* keyName, uint32_t def);
	void SetValue(const char* appName, const char* keyName, uint32_t new_val);
	
	void GetStringValue(const char* appName, const char* keyName, const char* def, char *buffer);
	void SetStringValue(const char* appName, const char* keyName, char *buffer);
}
