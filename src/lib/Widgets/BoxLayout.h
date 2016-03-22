#pragma once

#include <vector>
#include "Widget.h"

namespace pugi {
	class xml_node;
}

/*
	BoxLayout
	Basic layout which stacks children to it's parent in specified orientation.
*/
class BoxLayout : public Widget
{
public:
	enum Orientation {
		BL_Horizontal,
		BL_Vertical
	};

	BoxLayout(pugi::xml_node n): Widget(n), orientation(BL_Vertical){
		parseOrientation();
	};

	// Draw children
	virtual bool Draw(NVGcontext *context, float deltaTime) override;
	virtual void Tick(float dt) override;

	virtual void SizeChanged(float width, float height) override;
	virtual void PositionChanged(float x, float y) override;

	virtual void CursorMoved(double x, double y);

private:

	Orientation orientation;

	int childCount;

	void parseOrientation();
	void calculateLayout();

};