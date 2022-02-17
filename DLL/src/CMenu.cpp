//----------------------------------------
//
//	CMenu
//		Scrolling menu?
//
//----------------------------------------

#include "SpideyConstants.h"
#include "MenuSystem.h"
#include "CMenu.h"

namespace MenuSystem
{
	typedef int __fastcall CMenu_Update_Call(CMenu *menu, int edx);
	typedef int __fastcall CMenu_Display_Call(CMenu *menu, int edx);
	typedef int __fastcall CMenu_Zoom_Call(CMenu *menu, int edx, int zoom_level);
	typedef CMenu* __fastcall CMenu_CTor_Call(CMenu *menu, int edx, int x, int y, char subtype, uint16_t scale_x, uint16_t scale_y, int unk_b);
	
	CMenu_CTor_Call *CMenu_CTor = (CMenu_CTor_Call *)(ADDR_CMenu_CTor);
	CMenu_Zoom_Call *CMenu_Zoom = (CMenu_Zoom_Call *)(ADDR_CMenu_Zoom);
	CMenu_Update_Call *CMenu_Update = (CMenu_Update_Call *)(ADDR_CMenu_Update);
	CMenu_Display_Call *CMenu_Display = (CMenu_Display_Call *)(ADDR_CMenu_Display);
	
	//------------------------------
	// Initialize the menu
	//------------------------------
	
	CMenu* MenuSystem::CMenu::Initialize(int x, int y, char subtype, uint16_t scale_x, uint16_t scale_y, int unk_b)
	{
		return CMenu_CTor(this, 0, x, y, subtype, scale_x, scale_y, unk_b);
	}
	
	//------------------------------
	// Update the menu
	//------------------------------
	
	int MenuSystem::CMenu::Update()
	{
		return CMenu_Update(this, 0);
	}
	
	//------------------------------
	// Zoom, plays an animation
	//------------------------------
	
	int MenuSystem::CMenu::Zoom(int zoom_level)
	{
		return CMenu_Zoom(this, 0, zoom_level);
	}
	
	//------------------------------
	// Displays the menu, call in scene render
	//------------------------------
	
	int MenuSystem::CMenu::Display()
	{
		return CMenu_Display(this, 0);
	}
	
	//------------------------
	// Custom version for adding entry to a menu item
	// (This is a rewrite of stock code)
	//------------------------
	
	int MenuSystem::CMenu::AddEntry(char *text)
	{
		CMenuLine *line = &(lines[lines_max]);
		
		line -> text = text;
		
		// What do these do?
		line -> unk_b = 1;
		line -> unk_a = 0;
		
		int new_width = TextWidth(line -> text);
		
		if (new_width > current_width)
			current_width = new_width + width_val_a;
			
		entries_added ++;
		lines_max ++;
		
		return new_width;
	}
	
	// Also allow specifying a constant string!
	int MenuSystem::CMenu::AddEntry(const char *text)
	{
		char *copy_char = new char[strlen(text)];
		strcpy(copy_char, text);
		
		return AddEntry(copy_char);
	}
}
