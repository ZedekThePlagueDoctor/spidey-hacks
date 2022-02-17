#include "Logger.h"
#include "Patcher.h"
#include "SpideyConstants.h"
#include "GameConfig.h"

#include <windows.h>
#include <iostream>
#include <cstdio>
#include <stdio.h>

bool l_UseConsole, l_DebugOutput;
FILE *CON, *f_logger;
HANDLE consoleHandle;

namespace Logger
{
	//------------------------
	// Prepare the logger
	//------------------------
	
	void Initialize()
	{
		if (GameConfig::GetValue("Logger", "Console", 0))
			l_UseConsole = true;
			
		if (GameConfig::GetValue("Logger", "WriteFile", 1))
			l_DebugOutput = true;

		// Create console?
		if (l_UseConsole)
		{
			AllocConsole();
			freopen_s(&CON, "CONOUT$", "w", stdout);
			freopen_s(&CON, "CONOUT$", "w", stderr);
			std::cout.clear();
			std::cerr.clear();
			
			consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		}
		
		// Write to debug file?
		if (l_DebugOutput)
		{
			Log("Logger file output initialized.\n");
			fopen_s(&f_logger, "debug.txt", "w");
			
			if (!f_logger)
				Log("Failed to create debug.txt file.\n");
		}
	}
	
	//------------------------
	// Prints message to console and log!
	//------------------------
	
	void CoreLog(const char* to_log)
	{
		// Output to console
		if (l_UseConsole)
		{
			printf("[%s] %s", "LOG", to_log);
		}
			
		// Also write to .txt file!
		if (l_DebugOutput && f_logger)
		{
			fputs(to_log, f_logger);
			fflush(f_logger);
		}
	}
	
	//------------------------
	// Prints log message, with arguments!
	// Essentially PrintF wrapper
	//------------------------
	
	void Log(const char* Format, ...)
	{
		char *final_buffer = new char[2000];
		
		va_list args;
		va_start(args, Format);
		vsnprintf(final_buffer, 2000, Format, args);
		va_end(args);
		
		CoreLog(final_buffer);
	}
	
	void Log(char* Format, ...)
	{
		char *final_buffer = new char[2000];
		
		va_list args;
		va_start(args, Format);
		vsnprintf(final_buffer, 2000, Format, args);
		va_end(args);
		
		CoreLog(final_buffer);
	}

	//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	/*
	void HackedPrint(const char* format, ...)
	{
		// Probably an assert instead of a print, ignore for now
		int32_t ptr = (int32_t) format;
		
		if (ptr < SPIDEY_DATA_START || ptr > SPIDEY_DATA_END)
			return;
			
		if (strlen(format) <= 0)
			return;
	
		char *to_output = new char[2000];
		
		va_list args;
		
		va_start(args, format);
		vsprintf(to_output, format, args);
		va_end(args);

		Log(to_output);
	}
	*/

	//------------------------
	// Patch jump to our custom function
	//------------------------
	
	void PatchHandlers(SpideyZ::Patcher patcher)
	{
		// SpideyPrint is a really iffy function, sometimes assert
		// and sometimes print, let's ignore it for the moment
		//~ patcher.WriteJmp((void *) ADDR_SpideyPrint, (void *) HackedPrint);
	}
}
