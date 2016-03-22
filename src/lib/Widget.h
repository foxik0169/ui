/*
	UI Widget
	---------


	@FoXik0169
*/

#pragma once

#include "Types.h"
#include "pugixml\pugixml.hpp"
#include <map>

struct GLFWwindow;
struct NVGcontext;

class Application;
class Layout;
class Style;

namespace pugi {
	class xml_node;
	class xml_document;
}


/*
	Base class for all widgets.
	Extend this class to define new types of widgets.

	Default behaviour is to fill parent widget/layout with any painting thats done in draw()
*/
class Widget  
{

public:
	enum EventState {
		ES_Normal,
		ES_Hover,
		ES_Pressed,
		ES_Disabled
	};

	Widget(pugi::xml_node node);
	~Widget();

	const char* GetID();

	Widget *GetParent();
	Widget *GetFirstChild();
	Widget *GetLastChild();
	Widget *GetNext();

	const char* GetType();

	virtual bool Draw(NVGcontext *context, float deltaTime);
	virtual void Tick(float deltaTime);

	// Recalculates bounds
	virtual void SizeChanged(float width, float height);
	virtual void PositionChanged(float x, float y);

	// Mouse events
	virtual void CursorMoved(double x, double y);
	virtual void MouseEnter();
	virtual void MouseLeave();

	void MouseClickedInternal(double x, double y);
	void CursorMovedInternal(double x, double y);

	EventState GetState() { return event_state; }

	static std::map<EventState, std::string> StateMap;

protected:

	pugi::xml_node node;
	const pugi::xml_document &document;
	
	Rect bounds;

	double mouseX;
	double mouseY;

	bool mouseInsideWidgetBounds();

	EventState event_state;

private:

	Style *style;

};