#include "settings.h"


void ShapeSettings::SetColor(int r, int g, int b) 
{ 
	color.r = r;
	color.g = g;
	color.b = b;
};

CircleSettings::CircleSettings(float x, float y, float r)
{
    SetX(x);
    SetY(y);
    this->r = r;
};

RectangleSettings::RectangleSettings(float x, float y, int w, int h)
{
    SetX(x);
    SetY(y);
    width = w;
    height = h;
};