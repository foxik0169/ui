#include "Widget.h"
#include "Reflection\WidgetManager.h"
#include "Style\Style.h"
#include "Style\StyleManager.h"
#include "pugixml\pugixml.hpp"
#include "Reflection\Markup.h"

std::map<Widget::EventState,std::string> Widget::StateMap = { {  ES_Normal, "normal" },{ ES_Hover, "hover" },{ ES_Pressed, "pressed" },{ ES_Disabled, "disabled" } };

Widget::Widget(pugi::xml_node node) : node(node), document(Markup::GetDocument()), style(nullptr), event_state(ES_Normal)
{
	style = StyleManager::GetSingleton()->ReflectStyle(node);
}

Widget::~Widget()
{
	delete style;
}

bool Widget::Draw(NVGcontext *context, float deltaTime)
{
	if (style)
	{
		style->Draw(context, deltaTime, bounds);
		return true;
	}
	return false;
}

void Widget::Tick(float deltaTime)
{
	if (style) style->Tick(deltaTime);
}

bool Widget::mouseInsideWidgetBounds()
{
	return bounds.ContainsPoint(mouseX, mouseY);
}

void Widget::CursorMoved(double x, double y)
{

}

void Widget::MouseEnter()
{
	if (style) style->EventStateChanged(ES_Normal, event_state);
	printf("Hover on '%i'\r", node.hash_value());
}

void Widget::MouseLeave()
{
	if (style) style->EventStateChanged(ES_Hover, event_state);
}

void Widget::CursorMovedInternal(double x, double y)
{
	mouseX = x;
	mouseY = y;

	if (event_state != ES_Disabled)
	{
		if (event_state != ES_Hover && bounds.ContainsPoint(x, y)) {
			event_state = ES_Hover;
			MouseEnter();
		}
		else if (event_state != ES_Normal && !bounds.ContainsPoint(x,y)) {
			event_state = ES_Normal;
			MouseLeave();
		}
	}

	CursorMoved(x,y);
}

const char* Widget::GetID()
{
	return node.attribute("id").as_string();
}

Widget *Widget::GetParent()
{
	return WidgetManager::Reflect(node.parent());
}

Widget *Widget::GetFirstChild()
{
	return WidgetManager::Reflect(node.first_child());
}

Widget *Widget::GetLastChild()
{
	return WidgetManager::Reflect(node.last_child());
}

Widget *Widget::GetNext()
{
	return WidgetManager::Reflect(node.next_sibling());
}

const char *Widget::GetType()
{
	return node.name();
}

void Widget::SizeChanged(float width, float height)
{
	bounds.size = Point(width, height);
}

void Widget::PositionChanged(float x, float y)
{
	bounds.position = Point(x, y);
}