#include <cstdio>
#include <cstring>
#include <stdio.h>
#include <windows.h>

#include "GameConfig.h"
#include "Logger.h"
#include "Patcher.h"
#include "SpideyConstants.h"

namespace Graphics
{
	uint32_t preferredWidth = 0;
	uint32_t preferredHeight = 0;
	uint32_t preferredBPP = 0;
	bool b_Windowed = false;
	
	bool DummyMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) { return true; }
	void CustomExit(int errorCode) { Logger::Log("EXIT FROM ERROR CODE: %d\n", errorCode); }
	
	//---------------------------------
	// Init (nothing for now)
	//---------------------------------
	
	void Initialize() {}
		
	//---------------------------------
	// Patch values!
	//---------------------------------
	
	void Patch(SpideyZ::Patcher patcher)
	{
		preferredWidth = GameConfig::GetValue("Graphics", "VideoWidth", 0);
		preferredHeight = GameConfig::GetValue("Graphics", "VideoHeight", 0);
		preferredBPP = GameConfig::GetValue("Graphics", "VideoBPP", 32);
		
		if (GameConfig::GetValue("Graphics", "WindowedMode", 0))
			b_Windowed = true;
		
		if (preferredWidth <= 0 || preferredHeight <= 0)
			return;
			
		Logger::Log("Patching video res: %dx%d, %dBPP\n", preferredWidth, preferredHeight, preferredBPP);
		
		// CreateDirectDraw
		patcher.WriteInt32((void *) 0x004FF37D, preferredWidth);
		patcher.WriteInt32((void *) 0x004FF382, preferredHeight);
		patcher.WriteInt32((void *) 0x004FF27D, preferredBPP);
		
		// WinMain
		patcher.WriteInt32((void *) 0x005159E1, preferredWidth);
		patcher.WriteInt32((void *) 0x005159F2, preferredHeight);
		
		// Shell_DoShell
		patcher.WriteInt32((void *) 0x004A1AA4, preferredHeight);
		patcher.WriteInt32((void *) 0x004A1AA9, preferredWidth);
		patcher.WriteInt8((void *) 0x004A1AA2, preferredBPP);
		
		// Spidey.cfg reading, fallback values
		patcher.WriteInt32((void *) 0x005157CF, preferredWidth);
		patcher.WriteInt32((void *) 0x005157D9, preferredHeight);
		patcher.WriteInt32((void *) 0x005157E3, preferredBPP);
		
		// g_videoWidthB, g_videoHeightB
		patcher.WriteInt32((void *) 0x00568154, preferredWidth);
		patcher.WriteInt32((void *) 0x00568158, preferredHeight);
		
		// Patch jump to fullscreen, always use windowed
		if (b_Windowed)
		{
			Logger::Log("Windowed mode! EXPERIMENTAL!\n");
			
			// Replace all 0x10 quit messages with our own
			// (Not sure if this makes a difference or not)
			patcher.WriteNOPs((void *) 0x00515CDC, 6);
			patcher.WriteCall((void *) 0x00515CDC, (void *) DummyMessage);
			
			patcher.WriteJmp((void *) 0x0052A101, (void *) CustomExit);
		
			// NOP out setting of debug flags to 0
			patcher.WriteNOPs((void *) 0x005159DA, 6);
			
			// and set our own!
			patcher.WriteInt32((void *) 0x02E0988C, 1);
			
			// g_PreferWindowedMode
			patcher.WriteInt8((void *) 0x00515BAA, 1);
		}
	}
}
