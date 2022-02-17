#include <cstdio>
#include <windows.h>

#include "Patcher.h"
#include "SpideyConstants.h"
#include "Logger.h"
#include "MenuSystem.h"
#include "FileSystem.h"
#include "GameConfig.h"
#include "Graphics.h"

static SpideyZ::Patcher g_patcher = SpideyZ::Patcher();

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// Don't play intro movies, they're slow
int PlayMovieNull(int a, char b, int c, float d) { return 1; }

// Main code called when the DLL is injected
BOOL APIENTRY DllMain( HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:

			printf("SpideyZ dll injected\n");
			
			GameConfig::Initialize();
			
			Logger::Initialize();
			Logger::PatchHandlers(g_patcher);
			Logger::Log("Patching resolution...\n");
			
			Graphics::Initialize();
			Graphics::Patch(g_patcher);
			
			// No movies, please and thank you
			if (GameConfig::GetValue("Debug", "SkipMovies", 1))
			{
				g_patcher.WriteCall((void *) 0x00455CF1, (void *) PlayMovieNull);
				g_patcher.WriteCall((void *) 0x00455CFE, (void *) PlayMovieNull);
				g_patcher.WriteCall((void *) 0x00455D0C, (void *) PlayMovieNull);
				g_patcher.WriteCall((void *) 0x00455D1A, (void *) PlayMovieNull);
				
				// Fix stack pointer
				g_patcher.WriteInt8((void *) 0x00455D21, 2);
			}
			
			MenuSystem::Initialize();
			MenuSystem::Patch(g_patcher);
			
			FileSystem::Initialize();
			FileSystem::PatchHandlers(g_patcher);
			
			// (Mouse acquire junk at 0x0050A8C4, patch in future?)

			break;
	}
	
	return TRUE;
}
