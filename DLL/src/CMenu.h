//----------------------------------------
//
//	CMenu
//		Scrolling menu?
//
//----------------------------------------

#pragma once

#include "SpideyZ.h"
#include "MenuSystem.h"
#include "Patcher.h"

namespace MenuSystem
{
	struct CMenuLine
	{
		char *text;
		uint16_t val_a;
		uint16_t val_b;
		uint32_t unk;
		uint8_t unk_a;
		uint8_t unk_b;
		uint8_t unk_c;
		uint8_t unk_d;
		uint32_t unk_e;
		uint8_t bytes_b[4];
		uint8_t valuez_a;
		uint8_t valuez_b;
		uint8_t length_maybe;
		uint8_t valuez_d;
		uint32_t value;
	};

	struct CMenu
	{
		uint32_t *vtbl;
		uint32_t *ptr_to;
		uint16_t current_width;
		uint16_t text_val_b;
		uint16_t width_val_a;
		uint16_t size_val_b;
		uint8_t unk_a;
		uint8_t unk_b;
		uint8_t text_justify;
		uint8_t unk_d;
		uint8_t lines_current;
		uint8_t lines_min;			// Minimum line to show on-screen
		uint8_t byte_c;
		uint8_t byte_d;
		uint8_t byte_e;
		uint8_t byte_f;
		uint8_t lines_max;
		uint8_t lines_on_screen;
		uint32_t val_a;
		uint32_t val_b;
		uint32_t entry_x;
		uint32_t entry_y;
		uint32_t val_d;
		uint16_t val_da;
		uint16_t entries_added;
		uint32_t valuesb[2];
		CMenuLine lines[40];
		
		CMenu* Initialize(int x, int y, char subtype, uint16_t scale_x, uint16_t scale_y, int unk_b);
		int Update();
		int Display();
		int Zoom(int zoom_level);
		int AddEntry(char* text);
		int AddEntry(const char* text);
	};
}
