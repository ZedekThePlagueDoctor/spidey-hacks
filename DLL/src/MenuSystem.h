#pragma once

#include "SpideyZ.h"
#include "Patcher.h"

namespace MenuSystem
{
	// All masks for pad funcs
	
	enum TriggerButtons
	{
	  TRIG_UP = 0x01,
	  TRIG_DOWN = 0x02,
	  TRIG_LEFT = 0x04,
	  TRIG_RIGHT = 0x08,
	  TRIG_ENTER_A = 0x10,
	  TRIG_ESC_A = 0x20,
	  TRIG_ESC_B = 0x40,
	  TRIG_MOUSE_LEFT = 0x100,
	  TRIG_MOUSE_RIGHT = 0x200,
	  TRIG_UNK_A = 0x1000,
	  TRIG_UNK_B = 0x2000,
	  TRIG_UNK_C = 0x4000,
	  TRIG_UNK_D = 0x8000,
	  TRIG_UNK_E = 0x10000,
	  TRIG_UNK_F = 0x20000,
	  TRIG_UNK_G = 0x40000,
	  TRIG_NUMPAD_PLUS = 0x100000,
	  TRIG_NUMPAD_MINUS = 0x200000,
	  TRIG_ENTER_B = 0x1000000,
	  TRIG_ESC_C = 0x2000000,
	  TRIG_SPACE = 0x4000000
	};

	void Initialize();
	void Patch(SpideyZ::Patcher patcher);
	int TextWidth(char* text);
	int DrawText(int x, int y, char *text);
	int DrawText(int x, int y, const char *text);
	int SetMessageRGB(uint8_t r, uint8_t g, uint8_t b);
	int16_t SetMessageScale(uint16_t scale);
	int16_t SetMessageScaleF(float scale);
}
