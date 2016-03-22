/*
UI Button
---------

@FoXik0169
*/

#pragma once

#include "Types.h"
#include "Widget.h"

struct GLFWwindow;
struct NVGcontext;

/*
Button widget
*/
class Button : public Widget
{
public:

	Button(pugi::xml_node node);
	~Button();

	virtual bool Draw(NVGcontext *context, float deltaTime) override;

	void SetColor(Color color);
	Color GetColor();

	Color bgColor;

protected:

private:

};