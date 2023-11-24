#include "RGBA_Color.h"

RGBA_Color::RGBA_Color(int red, int green, int blue, int opacity)
    : __opacity(opacity),
      __opacityf(opacity / 255.0f),
      __red(red),
      __green(green),
      __blue(blue),
      __redf(red / 255.0f),
      __greenf(green / 255.0f),
      __bluef(blue / 255.0f)
{
}

RGBA_Color::RGBA_Color(float redf, float greenf, float bluef, float opacity)
    : __opacity(static_cast<int>(opacity * 255)),
      __opacityf(opacity),
      __red(static_cast<int>(redf * 255)),
      __green(static_cast<int>(greenf * 255)),
      __blue(static_cast<int>(bluef * 255)),
      __redf(redf),
      __greenf(greenf),
      __bluef(bluef)
{
}

float RGBA_Color::opacity() const { return __opacity; }
float RGBA_Color::opacityf() const { return __opacityf; }

int RGBA_Color::red() const { return __red; }
int RGBA_Color::green() const { return __green; }
int RGBA_Color::blue() const { return __blue; }

float RGBA_Color::redf() const { return __redf; }
float RGBA_Color::greenf() const { return __greenf; }
float RGBA_Color::bluef() const { return __bluef; }
