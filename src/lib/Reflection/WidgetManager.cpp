#include "WidgetManager.h"
#include "Button.h"
#include "BoxLayout.h"

WidgetManager *WidgetManager::singleton = nullptr;

Widget * WidgetManager::Reflect(pugi::xml_node element)
{
	WidgetManager* ins = getSingleton();
	WidgetPointerMap::iterator wpmi = ins->widgetHeap.find(element);
	if (wpmi != ins->widgetHeap.end())
	{
		return wpmi->second;
	}
	else {
		WidgetConstuctionMap::iterator i = ins->constructMap.find(element.name());
		_ASSERTE(i != getSingleton()->constructMap.end());
		Widget *w = i->second(element);
		ins->widgetHeap.insert(std::make_pair(element, w));
		return w;
	}
}

void WidgetManager::Init()
{
	singleton = new WidgetManager();
	singleton->registerWidget<Button>("button");
	singleton->registerWidget<BoxLayout>("boxlayout");
}

WidgetManager::WidgetManager(): constructMap()
{

}

WidgetManager::~WidgetManager()
{

}

