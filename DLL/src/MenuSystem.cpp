#include "SpideyConstants.h"
#include "Logger.h"
#include "MenuSystem.h"
#include "CMenu.h"
#include "GameConfig.h"

#define NUM_SOUNDS			30

// Sound 29 is cheat / pickup sound

namespace MenuSystem
{
	SpideyZ::Patcher g_patcher;
	
	typedef int __cdecl Pause_Call(int amt);
	typedef bool PCGfx_BeginScene_Call();
	typedef int PCGfx_End_Call(int val);
	typedef int Db_FlipClear_Call();
	typedef int Mess_TextWidth_Call(char *text);
	typedef int Mess_DrawText_Call(int x, int y, char *text);
	typedef int Mess_SetCurrentFontCall(const char* font_name);
	typedef int Mess_Update_Call();
	typedef int Pad_Update_Call();
	typedef int ActivateCheat_Call(int cheat_index);
	typedef int SFX_Play_Call(int soundIndex, uint16_t b, int c);
	typedef bool PCSHELL_CheckTriggers_Call(int mask, int a, int b);
	typedef int CalcPolyBufferEnd_Call();
	typedef int __cdecl CItem_New_Call(int itemSize);

	CalcPolyBufferEnd_Call *CalcPolyBufferEnd = (CalcPolyBufferEnd_Call *)(ADDR_CalcPolyBufferEnd);
	PCGfx_BeginScene_Call *PCGfx_BeginScene = (PCGfx_BeginScene_Call *)(ADDR_PCGfx_BeginScene);
	PCGfx_End_Call *PCGfx_EndScene = (PCGfx_End_Call *)(ADDR_PCGfx_EndScene);
	Db_FlipClear_Call *Db_FlipClear = (Db_FlipClear_Call *)(ADDR_Db_FlipClear);
	Pad_Update_Call *Pad_Update = (Pad_Update_Call *)(ADDR_Pad_Update);
	PCSHELL_CheckTriggers_Call *PCShell_CheckTriggers = (PCSHELL_CheckTriggers_Call *)(ADDR_PCShell_CheckTriggers);
	Mess_SetCurrentFontCall *Mess_SetCurrentFont = (Mess_SetCurrentFontCall *)(ADDR_Mess_SetCurrentFont);
	Mess_DrawText_Call *Mess_DrawText = (Mess_DrawText_Call *)(ADDR_Mess_DrawText);
	Mess_TextWidth_Call *Mess_TextWidth = (Mess_TextWidth_Call *)(ADDR_Mess_TextWidth);
	Mess_Update_Call *Mess_Update = (Mess_Update_Call *)(ADDR_Mess_Update);
	CItem_New_Call *CItem_New = (CItem_New_Call *)(ADDR_CItem_New);
	Pause_Call *Pause = (Pause_Call *)(ADDR_Pause);
	SFX_Play_Call *SFX_Play = (SFX_Play_Call *)(ADDR_SFX_Play);
	ActivateCheat_Call *ActivateCheat = (ActivateCheat_Call *)(ADDR_ActivateCheat);
	
	//------------------------
	// Create CMenu
	//------------------------
	
	CMenu *CreateCMenu(int x, int y, char subtype, uint16_t scale_x, uint16_t scale_y, int unk_b)
	{
		CMenu *menu = (CMenu *) CItem_New(sizeof(CMenu));
		menu -> Initialize(x, y, subtype, scale_x, scale_y, unk_b);
		
		return menu;
	}
	
	//------------------------
	// Normal font
	//------------------------
	
	void NormalFont()
	{
		SetMessageScaleF(1.0);
		Mess_SetCurrentFont("sp_fnt00.fnt");
	}
	
	//------------------------
	// Small font
	//------------------------
	
	void SmallFont()
	{
		SetMessageScaleF(1.0);
		Mess_SetCurrentFont("sp_fnt02.fnt");
	}
	
	//------------------------
	// Big font
	//------------------------
	
	void BigFont()
	{
		SetMessageScaleF(1.0);
		Mess_SetCurrentFont("font_big.fnt");
	}
	
	//------------------------
	// Set message scale
	//------------------------
	
	int16_t SetMessageScale(uint16_t scale)
	{
		g_patcher.WriteInt16((void *) ADDR_CurrentMessageScale, scale);
		return scale;
	}
	
	int16_t SetMessageScaleF(float scale)
	{
		int16_t new_scale = (int16_t) (scale * 256.0);
		return SetMessageScale(new_scale);
	}
	
	//------------------------
	// Set message color
	//------------------------
	
	int SetMessageRGB(uint8_t r, uint8_t g, uint8_t b)
	{
		g_patcher.WriteInt32((void *) ADDR_CurrentMessageR, r);
		g_patcher.WriteInt32((void *) ADDR_CurrentMessageG, g);
		g_patcher.WriteInt32((void *) ADDR_CurrentMessageB, b);
		
		return r;
	}
	
	//------------------------
	// Draw text!
	//------------------------
	
	int DrawText(int x, int y, char *text)
	{
		return Mess_DrawText(x, y, text);
	}
	
	// Allow const
	
	int DrawText(int x, int y, const char *text)
	{
		char *copied = new char[strlen(text)+1];
		strcpy(copied, text);
		
		return Mess_DrawText(x, y, copied);
	}
	
	//------------------------
	// Get text width
	//------------------------
	
	int TextWidth(char* text)
	{
		return Mess_TextWidth(text);
	}
	
	//------------------------
	// Currently drawing scene?
	//------------------------
	
	bool DrawingScene()
	{
		return (bool *) ADDR_DrawingScene;
	}
	
	//------------------------
	// Prepare menu system!
	//	(Should probably log here)
	//------------------------
	
	void Initialize() {}
	
	//------------------------
	// Custom SFX menu
	//------------------------
	
	int Shell_SFXMusic_Custom()
	{
		NormalFont();
		
		CMenu *menu = CreateCMenu(270, 90, 1, 256, 256, 13);

		for (int i=0; i<NUM_SOUNDS; i++)
		{
			char *text_to_add = new char[200];
			sprintf(text_to_add, "sound %d", i);
			
			menu -> AddEntry(text_to_add);
		}
		
		menu -> width_val_a = 5;
		menu -> lines_on_screen = 5;
		
		menu -> Zoom(1);
		
		char *to_draw = new char[256];
		sprintf(to_draw, "-");
		
		while (true)
		{
			Db_FlipClear();
			CalcPolyBufferEnd();
			
			//--------
			// -- RENDER HANDLING
			
			if (!DrawingScene())
				PCGfx_BeginScene();
			
			menu -> Display();
			
			SmallFont();
			SetMessageRGB(255, 255, 0);
			SetMessageScaleF(0.5);
			
			sprintf(to_draw, "CUR: %d, MIN: %d, MAX: %d, C: %d", menu -> lines_current, menu -> lines_min, menu -> lines_max, menu -> byte_d);
			DrawText(200, 32, to_draw);

			if (DrawingScene())
				PCGfx_EndScene(1);
				
			//--------
			// -- LOGIC HANDLING
			
			Mess_Update();
			Pad_Update();
			
			if (PCShell_CheckTriggers(TRIG_ENTER_A, 1, 1))
			{
				// cheat sound
				SFX_Play(29, 0x3FFF, 0);
				
				ActivateCheat(1);
			}
			
			else if (PCShell_CheckTriggers(TRIG_ESC_A, 1, 1))
				break;
			
			NormalFont();
			menu -> Update();
		}
		
		return 1;
	}
	
	//------------------------
	// Patch menu-related things
	//------------------------
	
	void Patch(SpideyZ::Patcher patcher)
	{
		g_patcher = patcher;
		
		// Write jump to our new shell menu
		if (GameConfig::GetValue("Debug", "CustomSFXMenu", 0))
			patcher.WriteJmp((void *) ADDR_Shell_SFXMusic, (void *) Shell_SFXMusic_Custom);
	}
}
