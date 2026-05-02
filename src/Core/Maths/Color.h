#ifndef CORE_MATHS_COLOR_H_INCLUDED
#define CORE_MATHS_COLOR_H_INCLUDED

#include "define.h"

// Color Presets are defined in Color.cpp
// Reminder : For Alpha, 0 is fully transparent and 255 is opaque

namespace gce {


struct Color
{
	uint8 r, g, b, a;

	static Color Clear;
	static Color Black;
	static Color White;
	static Color Red;
	static Color Blue;
	static Color Green;
	static Color Yellow;
	static Color Magenta;
	static Color Cyan;

	void Set(Color newColor);
	void Set(uint8 r, uint8 g, uint8 b, uint8 a);

	void Transparent();
	void Opaque();
};


}

#endif