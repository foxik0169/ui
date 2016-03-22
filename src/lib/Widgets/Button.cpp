#include "PCH.h"
#include "Button.h"
#include "pugixml\pugixml.hpp"

Button::Button(pugi::xml_node node): Widget(node)
{
	bgColor = GetColor();
}

Button::~Button()
{
}

bool Button::Draw(NVGcontext *context, float deltaTime)
{
	if (Widget::Draw(context, deltaTime)) return true;

	// Draw the default button here 
	nvgBeginPath(context);

	nvgRoundedRect(context, bounds.position.x, bounds.position.y, bounds.size.x - 4, bounds.size.y - 4, 20);
	nvgStrokeWidth(context, 2);
	nvgStrokeColor(context, nvgRGB(100, 100, 100));
	nvgFillColor(context, nvgRGB(240, 240, 240));

	if (mouseInsideWidgetBounds())
	{
		nvgFillColor(context, nvgRGB(220, 220, 220));
	}

	nvgFill(context);
	nvgStroke(context);

	return true;
}

void Button::SetColor(Color c)
{
	node.attribute("color").set_value(0xF00D00);
}

Color Button::GetColor()
{
	unsigned int color = node.attribute("color").as_uint();
	return Color(color);
}

