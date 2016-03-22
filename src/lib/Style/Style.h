#pragma once

#include <vector>
#include <list>
#include "Types.h"
#include "StyleManager.h"
#include "Widget.h"
#include "pugixml\pugixml.hpp"
#include <memory>

struct NVGcontext;
class Animation;

namespace pugi {
	class xml_node;
}

class Style
{

public:

	Style(std::vector<std::pair<pugi::xml_node, StyleManager::StyleInstructionDelegate>>, pugi::xml_node);
	void Draw(NVGcontext *context, float deltaTime, Rect bounds);
	
	// Handle property animations
	void Tick(float dt);

	void SetNode(pugi::xml_node);

	void EventStateChanged(Widget::EventState from, Widget::EventState to);

	pugi::xml_node GetElement();
	std::map<std::string, pugi::xml_attribute> GetAttributes() { return attributes; }

	unsigned int GetUIntValue(pugi::xml_attribute node) {
		if (StyleManager::isBinding(node))
		{
			return GetAttributes()[&node.as_string()[1]].as_uint();
		}
		return node.as_uint();
	};

	int GetIntValue(pugi::xml_attribute node) {
		if (StyleManager::isBinding(node))
		{
			return GetAttributes()[&node.as_string()[1]].as_int();
		}
		return node.as_int();
	};

	const char *GetStringValue(pugi::xml_attribute node) {
		if (StyleManager::isBinding(node))
		{
			return GetAttributes()[&node.as_string()[1]].as_string();
		}
		return node.as_string();
	};

private:
	std::vector<std::pair<pugi::xml_node, StyleManager::StyleInstructionDelegate>> instructions;
	std::map<std::string, pugi::xml_attribute> attributes;

	std::vector<Animation *> _animations;

	pugi::xml_node element;
	pugi::xml_node style;

	pugi::xml_node getAnimation(Widget::EventState from, Widget::EventState to);

};