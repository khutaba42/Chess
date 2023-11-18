#include "Color.h"

Color::Color(int red, int green, int blue, int opacity)
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

Color::Color(float redf, float greenf, float bluef, float opacity)
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

float Color::opacity() const { return __opacity; }
float Color::opacityf() const { return __opacityf; }

int Color::red() const { return __red; }
int Color::green() const { return __green; }
int Color::blue() const { return __blue; }

float Color::redf() const { return __redf; }
float Color::greenf() const { return __greenf; }
float Color::bluef() const { return __bluef; }
