#include "Markup.h"
#include "pugixml\pugixml.hpp"
#include "WidgetManager.h"
#include "nanovg\nanovg.h"

pugi::xml_document Markup::document;
pugi::xml_node Markup::root;

Point Markup::size;
bool Markup::dirty = true;

void Markup::Load(const char *filename)
{
	document.load_file(filename);
	root = document.first_child();
}

void Markup::Draw(NVGcontext * context, int width, int height, float pxRatio, float deltaTime)
{
	nvgBeginFrame(context, width, height, pxRatio);
	WidgetManager::Reflect(root)->Draw(context, deltaTime);
	nvgEndFrame(context);
}

void Markup::Tick(float dt)
{
	if (dirty) SizeChanged(size.x,size.y);
	WidgetManager::Reflect(root)->Tick(dt);
}

void Markup::SizeChanged(int width, int height)
{
	size.x = width;
	size.y = height;
	WidgetManager::Reflect(root)->SizeChanged(width, height);
}

void Markup::CursorMoved(double x, double y)
{
	WidgetManager::Reflect(root)->CursorMovedInternal(x, y);
}

void Markup::AddWidget(const char * parent_id, const char * type, std::map<std::string, std::string> attr)
{
	pugi::xml_node parent = document.find_child_by_attribute("id", parent_id);
	if (!parent) return;

	pugi::xml_node newWidget = parent.append_child(type);

	for (auto a : attr)
	{
		newWidget.append_attribute(a.first.c_str()).set_value(a.second.c_str());
	}

	dirty = true;
}

const pugi::xml_document& Markup::GetDocument()
{
	return document;
}