#pragma once
#include <map>
#include <set>
#include <string>
#include <functional>
#include "Widget.h"
#include "pugixml/pugixml.hpp"

class WidgetManager
{
public:
	typedef std::map<std::string, std::function<Widget* (const pugi::xml_node &element)>> WidgetConstuctionMap;
	typedef std::map<const pugi::xml_node, Widget*> WidgetPointerMap;

	template<typename T>
	void registerWidget(const char *name)
	{
		_ASSERTE(constructMap.find(name) == constructMap.end());
		constructMap.insert(std::make_pair(name, [](const pugi::xml_node &element) {return new T(element); }));
	}

	static Widget* Reflect(pugi::xml_node element);

	static WidgetManager *getSingleton() 
	{ 
		if (!singleton) Init();
		return singleton; 
	};

	WidgetManager();
	~WidgetManager();

private:
	static void Init();

	static WidgetManager *singleton;
	WidgetConstuctionMap constructMap;
	WidgetPointerMap widgetHeap;
};

