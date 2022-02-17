#define NTDDI_VERSION 0x0600000

#include <cstdio>
#include <cstring>
#include <stdio.h>
#include <windows.h>
#include <shlobj.h>

#include "GameConfig.h"
#include "Logger.h"
#include "SpideyConstants.h"

namespace GameConfig
{
	static char inipath[MAX_PATH];
	
	//---------------------------------
	// Set up INI path, and other things
	//---------------------------------
	
	void Initialize()
	{
		GetCurrentDirectoryA(MAX_PATH, inipath);
		strcat_s(inipath, MAX_PATH, "\\SpideyZ.ini");
	}
	
	//---------------------------------
	// Get a value from the INI
	//---------------------------------
	
	uint32_t GetValue(const char* appName, const char* keyName, uint32_t def)
	{
		return GetPrivateProfileIntA(appName, keyName, def, inipath);
	}
	
	//---------------------------------
	// Set a value from the INI
	//---------------------------------
	
	void SetValue(const char* appName, const char* keyName, uint32_t new_value)
	{
		char new_string[12];
		
		sprintf(new_string, "%d\n", new_value);
		
		WritePrivateProfileString(appName, keyName, new_string, inipath);
	}
	
	//---------------------------------
	// Get a string value from the INI
	//---------------------------------
	
	void GetStringValue(const char* appName, const char* keyName, const char* def, char* buffer)
	{
		GetPrivateProfileString(appName, keyName, def, buffer, 32, inipath);
	}
	
	//---------------------------------
	// Set a string value from the INI
	//---------------------------------
	
	void SetStringValue(const char* appName, const char* keyName, char* buffer)
	{
		WritePrivateProfileString(appName, keyName, buffer, inipath);
	}
}
