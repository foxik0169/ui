#include "Types.h"
#include <math.h>

float Clamp(float n, float min, float max)
{
	return fmaxf(min, (fminf(n, max)));
}

Color::Color(FColor &c)
{
	r = floorf(c.r * 255);
	g = floorf(c.g * 255);
	b = floorf(c.b * 255);
	a = floorf(c.a * 255);
}

FColor::FColor(Color &c)
{
	r = c.r / 255.0f;
	g = c.g / 255.0f;
	b = c.b / 255.0f;
	a = c.a / 255.0f;
}

Color Color::Lerp(Color from, Color to, float amount)
{
	//  (from + (to - from) * amount)

	FColor ffrom(from);
	FColor fto(to);

	float rl = TLerp<float>(ffrom.r, fto.r, amount);
	float gl = TLerp<float>(ffrom.g, fto.g, amount);
	float bl = TLerp<float>(ffrom.b, fto.b, amount);
	float al = TLerp<float>(ffrom.a, fto.a, amount);

	return Color(FColor(rl,gl,bl,al));
}