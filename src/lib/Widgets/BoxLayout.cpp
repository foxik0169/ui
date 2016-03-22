#include "PCH.h"
#include "WidgetManager.h"
#include "BoxLayout.h"

bool BoxLayout::Draw(NVGcontext * context, float deltaTime)
{
	// Layouts should call draw events on their children.

	for (auto child : node.children())
	{
		WidgetManager::Reflect(child)->Draw(context, deltaTime);
	}

	return true;
}

void BoxLayout::parseOrientation()
{
	const char *o_string = node.attribute("orientation").as_string("vertical");
	std::string orient(o_string);

	if (orient == "horizontal")
	{
		orientation = BL_Horizontal;
	}
}

void BoxLayout::Tick(float dt)
{
	Widget::Tick(dt);

	for (auto child : node.children())
	{
		WidgetManager::Reflect(child)->Tick(dt);
	}
}

void BoxLayout::calculateLayout()
{
	size_t children_count = std::distance(node.children().begin(), node.children().end());

	int index = 0;
	float margin = node.attribute("margin").as_float(0.0f);

	for (auto child : node.children())
	{
		if (orientation == BL_Vertical)
		{
			Rect newBounds = bounds;
			newBounds.position.y += (bounds.size.y / children_count) * index;
			newBounds.size.y = bounds.size.y / children_count;
			newBounds = newBounds - margin;

			WidgetManager::Reflect(child)->SizeChanged(newBounds.size.x, newBounds.size.y);
			WidgetManager::Reflect(child)->PositionChanged(newBounds.position.x, newBounds.position.y);
		}
		else {
			Rect newBounds = bounds;
			newBounds.position.x += (bounds.size.x / children_count) * index;
			newBounds.size.x = bounds.size.x / children_count;

			WidgetManager::Reflect(child)->SizeChanged(newBounds.size.x, newBounds.size.y);
			WidgetManager::Reflect(child)->PositionChanged(newBounds.position.x, newBounds.position.y);
		}
		index++;
	}

}

void BoxLayout::SizeChanged(float width, float height)
{
	Widget::SizeChanged(width, height);

	/*
		Recalculate layout and update children bounds;
	*/

	calculateLayout();
}

void BoxLayout::PositionChanged(float x, float y)
{
	Widget::PositionChanged(x, y);

	/*
		Recalculate layout and update children bounds;
	*/

	calculateLayout();
}

void BoxLayout::CursorMoved(double x, double y)
{
	for (auto child : node.children())
	{
		WidgetManager::Reflect(child)->CursorMovedInternal(x, y);
	}
}


