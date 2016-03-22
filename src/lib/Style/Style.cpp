#include "PCH.h"
#include "StyleManager.h"
#include "WidgetManager.h"
#include "Style.h"
#include <algorithm>
#include "Animation.h"

Style::Style(std::vector<std::pair<pugi::xml_node, StyleManager::StyleInstructionDelegate>> inst, pugi::xml_node style): element(nullptr), style(style)
{
	instructions = inst;
}

void Style::Tick(float dt)
{

	_animations.erase(
	std::remove_if(
			_animations.begin(),
			_animations.end(),
			[dt](Animation *a) -> bool {
				if (a->IsDone()) {
					delete a;
					return true;
				}
				a->Tick(dt);
				return false;
			}
		),
	_animations.end()
	);

}

void Style::Draw(NVGcontext * context, float deltaTime, Rect bounds)
{
	_ASSERTE(element != nullptr);

	for (auto inst : instructions)
	{
		inst.second(this, context, inst.first, deltaTime, bounds);
	}

}

void Style::SetNode(pugi::xml_node node)
{
	element = node;

	attributes.clear();

	for (auto a : style.children("attribute"))
	{
		pugi::xml_attribute node_attr = node.attribute(a.attribute("name").as_string());
		if (!node_attr) {
			node_attr = node.append_attribute(a.attribute("name").as_string());
			node_attr.set_value(a.attribute("default").value());
		}
		attributes[a.attribute("name").as_string()] = node_attr;
	}

}

void Style::EventStateChanged(Widget::EventState from, Widget::EventState to)
{
	if (pugi::xml_node anim = getAnimation(from, to))
	{
		for (auto trans : anim.children("transition"))
		{
			const char* a_name = trans.attribute("attribute").as_string();
			pugi::xml_attribute a_attr = attributes[a_name];
			_animations.erase(
				std::remove_if(
						_animations.begin(),
						_animations.end(),
						[a_attr](Animation *a) -> bool {
							return a_attr == a->GetAttribute();
						}
				),
			_animations.end()
			);
			_animations.push_back(new Animation(a_attr, style.child(Widget::StateMap[to].c_str()).child(a_name).attribute("value").as_uint(), trans.attribute("time").as_float()));
		}
	}
	else {
		for (auto child : style.child(Widget::StateMap[to].c_str()).children())
		{
			auto attr = attributes.find(child.name());
			if (attr != attributes.end())
			{
				attr->second.set_value(child.attribute("value").value());
			}
		}
	}
}

pugi::xml_node Style::GetElement()
{
	return element;
}

pugi::xml_node Style::getAnimation(Widget::EventState from, Widget::EventState to)
{
	for (auto child : style.children("animation"))
	{
		if (Widget::StateMap[from] == child.attribute("from").as_string() && Widget::StateMap[to] == child.attribute("to").as_string())
		{
			return child;
		}
	}
	return pugi::xml_node(nullptr);
}
