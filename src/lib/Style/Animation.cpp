#include "Animation.h"
#include "nanovg\nanovg.h"
#include <math.h>

Animation::Animation(pugi::xml_attribute attrib, double destValue, float time): current_time(0.0f), isColor(false), dest_time(time), isDone(false)
{
	from_d = attrib.as_double();
	to_d = destValue;
	attribute = attrib;

	// debug
	// debug = malloc(1024000);
}

Animation::Animation(pugi::xml_attribute attrib, unsigned int destValue, float time) : current_time(0.0f), isColor(true), dest_time(time), isDone(false)
{
	from_c = Color(attrib.as_uint());
	to_c = Color(destValue);
	attribute = attrib;

	// debug
	// debug = malloc(1024000);
}

void Animation::Tick(float dt)
{
	if (isDone) return;
	if (current_time > dest_time) isDone = true;
	current_time += dt;

	float current_amount = Clamp(current_time / dest_time);

	Color c = Color::Lerp(from_c, to_c, current_amount);

	if (isColor) attribute.set_value(c.rgba);
	else TLerp<double>(from_d, to_d, current_amount);

}
