#pragma once

// The opacity (alpha) parameter is a number between 0.0 (fully transparent) and 1.0 (fully opaque).
class RGBA_Color
{
public:
    RGBA_Color(int red, int green, int blue, int opacity = 255);
    RGBA_Color(float redf, float greenf, float bluef, float opacity = 1.0f);
    RGBA_Color(const RGBA_Color&) = default;
    ~RGBA_Color() = default;
    float opacity() const, opacityf() const;
    int red() const, green() const, blue() const;
    float redf() const, greenf() const, bluef() const;
private:
    int __opacity;
    float __opacityf;
    int __red, __green, __blue;
    float __redf, __greenf, __bluef;
};
